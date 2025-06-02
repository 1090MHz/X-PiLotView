
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
  #ifdef _MSC_VER
#include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif
#include "lvgl/lvgl.h"
#include "lvgl/src/drivers/evdev/lv_evdev.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

extern void freertos_main(void);

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init(480, 800);

#if LV_USE_OS == LV_OS_NONE

  /* Run the default demo */
  /* To try a different demo or example, replace this with one of: */
  /* - lv_demo_benchmark(); */
  /* - lv_demo_stress(); */
  /* - lv_example_label_1(); */
  /* - etc. */
  lv_demo_widgets();

  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_timer_handler();
#ifdef _MSC_VER
    Sleep(5);
#else
    usleep(5 * 1000);
#endif
  }

#elif LV_USE_OS == LV_OS_PTHREAD
    /* Run the default demo */
    lv_demo_widgets();

    /* LVGL will call lv_timer_handler() in a background thread internally */

    while(1) {
        // You can handle your own app logic here if needed
        sleep(1); // Idle main loop
    }

#elif LV_USE_OS == LV_OS_FREERTOS

  /* Run FreeRTOS and create lvgl task */
  freertos_main();

#endif

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

#if LV_USE_SDL
  lv_display_t * disp = lv_sdl_window_create(w, h);

  if (!disp)
  {
    printf("SDL display creation failed!\n");
    exit(1);
  }

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
#endif

#if LV_USE_LINUX_FBDEV
    // Create framebuffer display
  lv_display_t *disp = lv_linux_fbdev_create();
    if (!disp) {
        fprintf(stderr, "Framebuffer display creation failed!\n");
        return NULL;
  }

  lv_display_set_resolution(disp, w, h);

    // Allocate draw buffers (e.g., 40 lines)
    size_t buf_lines = 40;
    size_t buf_size = w * buf_lines;

    static lv_draw_buf_t draw_buf1, draw_buf2;
    static lv_color_t *buf1 = NULL;
    static lv_color_t *buf2 = NULL;

    buf1 = lv_malloc(sizeof(lv_color_t) * buf_size);
    buf2 = lv_malloc(sizeof(lv_color_t) * buf_size);
    if (!buf1 || !buf2) {
        fprintf(stderr, "Draw buffer allocation failed!\n");
        return NULL;
    }

    // stride is width in pixels * bytes per pixel
    uint32_t stride = w * sizeof(lv_color_t);

    // Initialize draw buffers with correct parameter order
    lv_draw_buf_init(&draw_buf1, w, buf_lines, LV_COLOR_FORMAT_NATIVE, stride, buf1, sizeof(lv_color_t) * buf_size);
    lv_draw_buf_init(&draw_buf2, w, buf_lines, LV_COLOR_FORMAT_NATIVE, stride, buf2, sizeof(lv_color_t) * buf_size);

  // Set the display draw buffers
  lv_display_set_draw_buffers(disp, &draw_buf1, &draw_buf2);

    // Set display as default
  lv_display_set_default(disp);

    // Create input device
  lv_indev_t *indev = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/by-path/platform-i2c@0-event");
  if (!indev) {
        fprintf(stderr, "Input device creation failed!\n");
        return NULL;
  }

  lv_indev_set_display(indev, disp);
  lv_indev_set_group(indev, lv_group_get_default());

  return disp;
#endif

  return NULL; /* Return NULL if no display is created */
}
