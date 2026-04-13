# 2026 Capstone Embedded Project

Todo: Will add readme when everything works

## Setup

### Prerequisites
- STM32CubeMX
- CMake
- arm-none-eabi-gcc

### Setup
1. Clone the repo
```bash
   git clone --recurse-submodules 
```
2. Open `2026_GPS_Monitor.ioc` in STM32CubeMX and click **Generate Code**
3. Build:
```bash
   cmake -B build
   cmake --build build
```