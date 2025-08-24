cmake_minimum_required(VERSION 3.16)

project(accel_app VERSION 1.0 LANGUAGES C)
set (CMAKE_C_STANDARD 99)

add_executable(accel_app accel_app.c)
add_library(i2c_emulated i2c_emulated.c i2c_emulated.h)
add_library(accelerometer accelerometer.c accelerometer.h ICM_42670_defs.h)
target_link_libraries(accel_app PRIVATE i2c_emulated accelerometer)

project(accel_imu_emulator VERSION 1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 14)

add_executable(imu_emu imu_emu.cpp)
add_library (imui2c imui2c.cpp imui2c.h)
target_link_libraries(imu_emu PRIVATE imui2c i2c_emulated)
