#ifndef SERIAL_SEARCH_H
#define SERIAL_SEARCH_H

#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

struct DeviceInfo {
    std::string path;
    std::string description;
};


std::string select_port(const std::vector<DeviceInfo>& devices);
std::vector<DeviceInfo> search_ports(const std::string& baseDir);

#endif //SERIAL_SEARCH_H
