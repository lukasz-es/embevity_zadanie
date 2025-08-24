cmake_minimum_required(VERSION 3.16)

project(accel_app VERSION 1.0 LANGUAGES C)
set (CMAKE_C_STANDARD 99)

add_executable(accel_app accel_app.c)
add_library(i2c_emulated i2c_emulated.c i2c_emulated.h)
add_library(accelerometer accelerometer.c accelerometer.h ICM_42670_defs.h)
target_link_libraries(accel_app PRIVATE i2c_emulated accelerometer)
