#include "serial_search.h"

std::vector<DeviceInfo> search_ports(const std::string& baseDir) {
    std::vector<DeviceInfo> devices;

    // Use serial_cpp for cross-platform port detection
    std::vector<serial_cpp::PortInfo> ports = serial_cpp::list_ports();

    for (const auto& port : ports) {
        DeviceInfo device;
        device.path = port.port;
        device.description = port.description;
        device.hardware_id = port.hardware_id;
        devices.push_back(device);
    }

    // If no ports were found and we're on Linux/macOS, try the fallback method
    #ifdef __linux__ || defined(__APPLE__)
    if (devices.empty() && !baseDir.empty()) {
        std::cerr << "No ports found with serial_cpp, trying filesystem fallback method..." << std::endl;

        std::vector<std::pair<std::string, std::string>> patterns = {
            {"ttyUSB", "Linux USB serial port"},
            {"ttyACM", "Linux ACM serial port"},  // Added ACM ports
            {"tty.usbserial", "macOS USB serial port"},
            {"cu.", "macOS Bluetooth serial port"}
        };

        try {
            for (const auto& entry : std::filesystem::directory_iterator(baseDir)) {
                if (entry.is_regular_file()) continue;

                std::string filename = entry.path().filename().string();

                for (const auto& [pattern, desc] : patterns) {
                    if (filename.find(pattern) == 0) {
                        devices.push_back({entry.path().string(), desc, ""});
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error searching ports: " << e.what() << std::endl;
        }
    }
    #endif


    return devices;
}

