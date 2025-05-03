cmake_minimum_required(VERSION 3.30)
project(EMPE)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0)

include(FetchContent)
FetchContent_Declare(
        serial_cpp
        GIT_REPOSITORY https://github.com/ami-iit/serial_cpp.git
        GIT_TAG        v1.3.3 # or use the tag or commit you prefer
)

FetchContent_MakeAvailable(serial_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(EMPE main.cpp
        src/serial_read.h
        src/serial_read.cpp
        src/serial_search.h
        src/serial_search.cpp
        src/window.h
        src/window.cpp
)

target_compile_features(EMPE PRIVATE cxx_std_20)
target_sources(EMPE PRIVATE main.cpp)

target_include_directories(EMPE PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_directories(EMPE PRIVATE ${GTKMM_LIBRARY_DIRS})

# Link Qt targets properly
target_link_libraries(EMPE PRIVATE ${GTKMM_LIBRARIES}
        serial_cpp::serial_cpp
)