cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(BcmCpp C CXX ASM)

# BCM C++ (OpenBSW) - Taktflow Systems
# POSIX-only build using FreeRTOS POSIX adapter

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

set(BUILD_TARGET_PLATFORM "POSIX" CACHE STRING "Target Platform")
set(BUILD_TARGET_RTOS "FREERTOS" CACHE STRING "Target RTOS")

# Disable OpenBSW's own executables — we provide our own
set(BUILD_EXECUTABLE "" CACHE STRING "" FORCE)

# Tell OpenBSW to build its POSIX platform
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

# OpenBSW as subdirectory (libraries only, no executable)
set(OPENBSW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../private/vendor/openbsw-main")
add_subdirectory(${OPENBSW_DIR} ${CMAKE_CURRENT_BINARY_DIR}/openbsw EXCLUDE_FROM_ALL)

# ETL profile
target_include_directories(etl BEFORE INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/etl_profile)

# Shared UDS/transport configuration (from shared/)
set(SHARED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../shared")
add_subdirectory(${SHARED_DIR}/udsConfiguration ${CMAKE_CURRENT_BINARY_DIR}/udsConfiguration)
add_subdirectory(${SHARED_DIR}/transportConfiguration ${CMAKE_CURRENT_BINARY_DIR}/transportConfiguration)
add_subdirectory(${SHARED_DIR}/logger ${CMAKE_CURRENT_BINARY_DIR}/loggerStub)

# BCM-specific configuration injections
add_subdirectory(asyncCoreConfiguration)
add_subdirectory(asyncBinding)
add_subdirectory(configuration)
add_subdirectory(platforms/posix)

# RTOS aliases
add_library(asyncPlatform ALIAS asyncFreeRtos)
add_library(osRtos ALIAS freeRtos)
add_library(asyncRtosImpl ALIAS asyncFreeRtosImpl)

# BCM application
add_subdirectory(application)
