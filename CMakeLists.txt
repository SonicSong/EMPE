cmake_minimum_required(VERSION 3.30)
project(EMPE)

set(CMAKE_CXX_STANDARD 17)

# Platform detection
if(WIN32)
    set(BUILD_WINDOWS TRUE)
elseif(APPLE)
    set(BUILD_MACOS TRUE)
elseif(UNIX AND NOT APPLE)
    set(BUILD_LINUX TRUE)
endif()

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0)
include(FetchContent)

FetchContent_Declare(
        serial_cpp
        GIT_REPOSITORY https://github.com/ami-iit/serial_cpp.git
        GIT_TAG        v1.3.3
)

FetchContent_Declare(
        re2
        GIT_REPOSITORY https://github.com/google/re2.git
        GIT_TAG  2024-07-02
)

FetchContent_Declare(
        gtkchart
        GIT_REPOSITORY https://github.com/lundmar/gtkchart.git
        GIT_TAG master
)

FetchContent_MakeAvailable(serial_cpp re2)

FetchContent_GetProperties(gtkchart)
if(NOT gtkchart_POPULATED)
    FetchContent_MakeAvailable(gtkchart)
    # Create a CMake library target for gtkchart
    add_library(gtkchart STATIC
            ${gtkchart_SOURCE_DIR}/src/gtkchart.c
            ${gtkchart_SOURCE_DIR}/src/gtkchart.h
    )

    # Set include directories
    target_include_directories(gtkchart PUBLIC
            ${gtkchart_SOURCE_DIR}/src
    )

    # Find GTK4 dependencies
#    if(NOT BUILD_WINDOWS)
        pkg_check_modules(GTK4 REQUIRED gtk4)
#    else()
#        # For Windows, set up GTK4 paths
#        set(GTK4_INCLUDE_DIRS "$ENV{GTK4_INCLUDE_DIR}" CACHE PATH "GTK4 include directories")
#        set(GTK4_LIBRARY_DIRS "$ENV{GTK4_LIB_DIR}" CACHE PATH "GTK4 library directories")
#        set(GTK4_LIBRARIES "gtk-4.0" CACHE STRING "GTK4 libraries")
#
#        # Check if the paths are set
#        if(NOT GTK4_INCLUDE_DIRS OR NOT GTK4_LIBRARY_DIRS)
#            message(WARNING "GTK4 paths not set. Please set GTK4_INCLUDE_DIR and GTK4_LIB_DIR environment variables")
#        endif()
#    endif()

    # Link GTK4 to gtkchart
    target_include_directories(gtkchart PRIVATE ${GTK4_INCLUDE_DIRS})
    target_link_directories(gtkchart PRIVATE ${GTK4_LIBRARY_DIRS})
    target_link_libraries(gtkchart PRIVATE ${GTK4_LIBRARIES})
endif()

set(CMAKE_CXX_STANDARD 20)

# Define source files
set(SOURCE_FILES
        main.cpp
        src/serial_read.h
        src/serial_read.cpp
        src/serial_search.h
        src/serial_search.cpp
        src/ThreadSafeQueue.h
        src/ThreadSafeQueue.cpp
        src/atomic_vars.cpp
        src/atomic_vars.h
        src/connection.cpp
        src/connection.h
        src/windowing/window.h
        src/windowing/window.cpp
        src/windowing/settings_window.cpp
        src/windowing/settings_window.h
        src/windowing/licenses.cpp
        src/windowing/licenses.h
        src/settings_manager.h
        src/settings_manager.cpp
        src/windowing/graph_window.cpp
        src/windowing/graph_window.h
        src/counter.cpp
        src/counter.h
)

add_executable(EMPE ${SOURCE_FILES})

# Windows-specific settings
if(BUILD_WINDOWS)
    # Add WIN32 to make it a Windows GUI application (no console)
    set_target_properties(EMPE PROPERTIES WIN32_EXECUTABLE TRUE)

    # Define WINDOWS for preprocessor
    target_compile_definitions(EMPE PRIVATE WIN32 _WINDOWS)
endif()

target_compile_features(EMPE PRIVATE cxx_std_20)

# Include and link directories
target_include_directories(EMPE PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_directories(EMPE PRIVATE ${GTKMM_LIBRARY_DIRS})

# Link libraries
target_link_libraries(EMPE PRIVATE
        ${GTKMM_LIBRARIES}
        serial_cpp::serial_cpp
        re2::re2
        gtkchart
)