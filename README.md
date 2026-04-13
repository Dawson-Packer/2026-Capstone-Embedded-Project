# 2026 Capstone Embedded Project

Todo: Will add readme when everything works

## Prerequisites
- STM32CubeMX
- STM32CubeProgrammer
- CMake 3.22+
- Ninja
- arm-none-eabi-gcc

## Setup
1. Clone the repo
```bash
   git clone --recurse-submodules 
```
2. Open `2026_GPS_Monitor.ioc` in STM32CubeMX and click **Generate Code**
3. Build:
```bash
# In DEBUG mode
   cmake --preset Debug
   cmake --build build --preset Debug
# In RELEASE mode
   cmake --preset Release
   cmake --build build --preset Release
```
4. Flash the firmware using STM32CubeProgrammer, using the binary in `build/<Debug|Release>/2026_GPS_Monitor.bin`
