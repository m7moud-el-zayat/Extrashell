cmake_minimum_required(VERSION 3.22)

project(Extrashell VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckIPOSupported)

check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Qt6 REQUIRED COMPONENTS Core Qml DBus)

qt6_standard_project_setup()
qt6_policy(SET QTP0001 NEW)

include(cmake/QmlModuleHelpers.cmake)

add_subdirectory(src/dbus)
add_subdirectory(src/udisks2)
add_subdirectory(src/login1)