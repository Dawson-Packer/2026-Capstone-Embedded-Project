cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME 2026_GPS_Monitor)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here

    # Core/Src/notecard.c
    # Core/Src/measure.c
    # Core/Src/time.c
    # Core/Src/logger.c
    # Core/Src/sdcard.c

    Core/Src/retarget.c

    App/app.cpp
    App/Modules/Power/power.cpp
    App/Modules/Measure/measure.cpp
    App/Modules/Transmit/transmit.cpp
    App/Modules/Filesystem/filesystem.cpp

    App/Drivers/sdcard.cpp
    App/Drivers/notecard.cpp

    # cube-ide-sd-card
    FATFS/Target/user_diskio_spi.c
    FATFS/Target/user_diskio_spi.h

    # note-c
    note-c/n_atof.c
    note-c/n_b64.c
    note-c/n_cjson_helpers.c
    note-c/n_cjson.c
    note-c/n_cobs.c
    note-c/n_const.c
    note-c/n_ftoa.c
    note-c/n_helpers.c
    note-c/n_hooks.c
    note-c/n_i2c.c
    note-c/n_md5.c
    note-c/n_printf.c
    note-c/n_request.c
    note-c/n_serial.c
    note-c/n_str.c
    note-c/n_ua.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    App
    App/Drivers
    # App/Middleware
    # App/Modules/Filesystem
    # App/Utils

    FATFS/Target
    note-c
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    "-u" "_printf_float"
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary
    $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    ${CMAKE_PROJECT_NAME}.bin
)
