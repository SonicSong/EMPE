#include "serial_search.h"

std::string select_port(const std::vector<DeviceInfo>& devices) {
    if (devices.empty()) {
        std::cout << "No matching devices found." << std::endl;
        return "";
    }

    std::cout << "Available devices:" << std::endl;
    for (size_t i = 0; i < devices.size(); ++i) {
        std::cout << i + 1 << ". " << devices[i].path
                 << " (" << devices[i].description << ")" << std::endl;
    }

    int choice;
    while (true) {
        std::cout << "Select device number (-1 to cancel): ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        if (choice == -1) return "";
        if (choice >= 1 && choice <= static_cast<int>(devices.size())) {
            return devices[choice - 1].path;
            //choice - 1
        }
        std::cout << "Invalid selection. Please try again." << std::endl;
    }
}

std::vector<DeviceInfo> search_ports(const std::string& baseDir) {
    std::vector<DeviceInfo> devices;

    // std::cout << baseDir << std::endl;

    std::vector<std::pair<std::string, std::string>> patterns = {
        {"ttyUSB", "Linux USB serial port"},
        {"tty.usbserial", "macOS USB serial port"},
        {"cu.", "macOS Bluetooth serial port"}
    };

    //TODO: FIX WINDOWS

    try {
        for (const auto& entry : std::filesystem::directory_iterator(baseDir)) {
            if (entry.is_regular_file()) continue;

            std::string filename = entry.path().filename().string();

            for (const auto& [pattern, desc] : patterns) {
                if (filename.find(pattern) == 0) {
                    devices.push_back({entry.path().string(), desc});
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error searching ports: " << e.what() << std::endl;
        return {};
    }

    return devices;
}