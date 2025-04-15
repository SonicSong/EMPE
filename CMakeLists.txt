cmake_minimum_required(VERSION 3.30)
project(EMPE)

find_package(PkgConfig REQUIRED
        Qt6 COMPONENTS Core Widgets SerialPort)

pkg_check_modules(GTKMM gtkmm-4.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(EMPE main.cpp
        src/serial_read.h
        src/serial_read.cpp)

target_compile_features(EMPE PRIVATE cxx_std_20)

target_sources(EMPE PRIVATE main.cpp)

target_include_directories(EMPE PRIVATE ${GTKMM_INCLUDE_DIRS})

target_link_directories(EMPE PRIVATE ${GTKMM_LIBRARY_DIRS})

target_link_libraries(EMPE PRIVATE ${GTKMM_LIBRARIES}
        Qt6::Core
        Qt6::Widgets
        Qt6::SerialPort
        )