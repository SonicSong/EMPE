#ifndef SERIAL_SEARCH_H
#define SERIAL_SEARCH_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <serial_cpp/serial.h>



struct DeviceInfo {
    std::string path;
    std::string description;
    std::string hardware_id; // Added hardware_id field
};

std::vector<DeviceInfo> search_ports(const std::string& baseDir = ""); // Made baseDir optional

#endif //SERIAL_SEARCH_H
