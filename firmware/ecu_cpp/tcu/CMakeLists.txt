cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(TcuCpp C CXX ASM)

# TCU C++ (OpenBSW) - Taktflow Systems
# POSIX-only build using FreeRTOS POSIX adapter

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

set(BUILD_TARGET_PLATFORM "POSIX" CACHE STRING "Target Platform")
set(BUILD_TARGET_RTOS "FREERTOS" CACHE STRING "Target RTOS")

set(BUILD_EXECUTABLE "" CACHE STRING "" FORCE)
set(OPENBSW_PLATFORM "posix" CACHE STRING "" FORCE)

# Enable CAN + transport + UDS support
set(PLATFORM_SUPPORT_CAN ON CACHE BOOL "" FORCE)
set(PLATFORM_SUPPORT_ETHERNET OFF CACHE BOOL "" FORCE)
set(PLATFORM_SUPPORT_STORAGE OFF CACHE BOOL "" FORCE)
set(PLATFORM_SUPPORT_TRANSPORT ON CACHE BOOL "" FORCE)
set(PLATFORM_SUPPORT_UDS ON CACHE BOOL "" FORCE)
set(PLATFORM_SUPPORT_IO OFF CACHE BOOL "" FORCE)
set(PLATFORM_SUPPORT_WATCHDOG OFF CACHE BOOL "" FORCE)
set(PLATFORM_SUPPORT_MPU OFF CACHE BOOL "" FORCE)
set(PLATFORM_SUPPORT_ROM_CHECK OFF CACHE BOOL "" FORCE)

set(OPENBSW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../private/vendor/openbsw-main")
add_subdirectory(${OPENBSW_DIR} ${CMAKE_CURRENT_BINARY_DIR}/openbsw EXCLUDE_FROM_ALL)

target_include_directories(etl BEFORE INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/etl_profile)

# Shared UDS/transport configuration (from shared/)
set(SHARED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../shared")
add_subdirectory(${SHARED_DIR}/udsConfiguration ${CMAKE_CURRENT_BINARY_DIR}/udsConfiguration)
add_subdirectory(${SHARED_DIR}/transportConfiguration ${CMAKE_CURRENT_BINARY_DIR}/transportConfiguration)
add_subdirectory(${SHARED_DIR}/logger ${CMAKE_CURRENT_BINARY_DIR}/loggerStub)

add_subdirectory(asyncCoreConfiguration)
add_subdirectory(asyncBinding)
add_subdirectory(configuration)
add_subdirectory(platforms/posix)

add_library(asyncPlatform ALIAS asyncFreeRtos)
add_library(osRtos ALIAS freeRtos)
add_library(asyncRtosImpl ALIAS asyncFreeRtosImpl)

add_subdirectory(application)
