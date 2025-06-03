#ifndef SERIAL_SEARCH_H
#define SERIAL_SEARCH_H

#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <serial_cpp/serial.h>

#ifdef _WIN32
// Windows.h defines many macros that can conflict with other libraries
// Define these before including Windows.h to prevent conflicts
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX  // Prevents Windows from defining min/max macros
#endif
#include <windows.h>
#endif

struct DeviceInfo {
    std::string path;
    std::string description;
    std::string hardware_id;
};

std::string select_port(const std::vector<DeviceInfo>& devices);
std::vector<DeviceInfo> search_ports(const std::string& baseDir = "");

#endif //SERIAL_SEARCH_H
