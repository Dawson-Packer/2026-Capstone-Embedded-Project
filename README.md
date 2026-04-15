# 2026 Capstone Embedded Project

Firmware for the device outlined below, to track temperature, time, and location data of produce
during air transit.

## Device BOM
* 1x [Blues Notecarrier CX (onboard STM32L433CCU6)](https://blues.com/notecarrier-cx/)
* 1x [Blues Notecard Cellular](https://blues.com/products/notecard/notecard-cellular/)
* 1x 3.6V 1500mAh NiMH Power pack

## App Specifications
* Periodic measurement (~45 minute interval)
* Periodic transmit (1/day)
* Local storage (SD Card)
* 

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
