# LVGL X-Plane Six Pack Gauges for Raspberry Pi

> **⚠️ Disclaimer: This project is a work in progress. Not production ready yet!**

This project provides an **ultralight, LVGL-based instrument panel** for displaying X-Plane "six pack" flight gauges (altimeter, airspeed, attitude, etc.) on a Raspberry Pi Zero 2 W or other Linux SBCs. It is designed for high performance and low resource usage, making it ideal for embedded systems and DIY cockpit builders.

## Features

- **LVGL-powered GUI** for smooth, customizable instrument rendering
- Designed for **X-Plane flight simulator** data integration
- Supports **Raspberry Pi Zero 2 W** (and other Linux SBCs)
- Configurable for **framebuffer (fbdev)** or **SDL** display backends
- Modular codebase for adding more gauges or custom panels
- Optional system and performance monitoring

## Hardware Requirements

- Raspberry Pi Zero 2 W (or any Linux SBC)
- Supported display (HDMI, DSI, or SPI)
- Network connection to X-Plane simulator

## Software Requirements

- Raspberry Pi OS (Bookworm or later) or other Linux
- LVGL (included as submodule or source)
- SDL2 (if using SDL backend)
- CMake 3.12+ and GCC toolchain

## Getting Started

### 1. Clone the Repository

```sh
git clone --recursive https://github.com/GitHub.1090MHz/X-PiLotView.git
cd X-PiLotView
```

If you forgot `--recursive`, run:
```sh
git submodule update --init --recursive
```

### 2. Install Dependencies

```sh
sudo apt update
sudo apt install build-essential cmake git libsdl2-dev libsdl2-image-dev libpng-dev libjpeg-dev libfreetype6-dev
```

### 3. Build the Project

```sh
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### 4. Run

```sh
./bin/main
```

## Configuration

- **Display Backend:**  
  Edit `lv_conf.h` to select `LV_USE_LINUX_FBDEV` (for direct framebuffer) or `LV_USE_SDL` (for windowed/X11).
- **Gauges:**  
  Add or modify gauge widgets in the `main` application code.
- **Performance Monitoring:**  
  Enable `LV_USE_SYSMON` and `LV_USE_PERF_MONITOR` in `lv_conf.h` for on-screen FPS/CPU stats.

## X-Plane Integration

This project is designed to receive data from X-Plane via network (UDP).  
See the `main` code and documentation for details on connecting to X-Plane and mapping datarefs to gauges.

## Credits

- [LVGL](https://lvgl.io/) - Embedded graphics library
- [X-Plane](https://www.x-plane.com/) - Flight simulator

## License

MIT License (see [LICENSE](LICENSE) file for details)

---

**Note:**  
This project was originally generated using the LVGL Project Creator.  
For original LVGL simulator documentation, see [`README_LVGL.md`](README_LVGL.md).
