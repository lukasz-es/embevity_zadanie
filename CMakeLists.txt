cmake_minimum_required(VERSION 3.16)

project(accel_app VERSION 1.0 LANGUAGES C)
set (CMAKE_C_STANDARD 99)

add_executable(accel_app src/accel_app.c)
add_library(i2c_emulated src/i2c_emulated.c src/i2c_emulated.h)
add_library(accelerometer src/accelerometer.c src/accelerometer.h src/i2c_emulated.c src/i2c_emulated.h src/ICM_42670_defs.h)
add_library(freefall src/freefall.c src/freefall.h src/accelerometer.c src/accelerometer.h)
add_library(argshelper src/argshelper.c src/argshelper.h)
target_link_libraries(accel_app PRIVATE i2c_emulated accelerometer freefall argshelper)

project(accel_imu_emulator VERSION 1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 14)

add_executable(imu_emu src/imu_emu.cpp)
add_library (imui2c src/imui2c.cpp src/imui2c.h src/ICM_42670_defs.h)
target_link_libraries(imu_emu PRIVATE imui2c i2c_emulated)

project(freefall_ut VERSION 1.0 LANGUAGES C)
set (CMAKE_C_STANDARD 99)
add_executable(freefall_ut src/freefall_ut.c)
target_link_libraries(freefall_ut PRIVATE i2c_emulated accelerometer freefall)
