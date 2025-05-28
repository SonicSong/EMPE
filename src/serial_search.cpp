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

#ifdef _WIN32
std::vector<DeviceInfo> search_windows_com_ports() {
    std::vector<DeviceInfo> devices;

    // Try COM ports from COM1 to COM16 (common range for most Windows systems)
    for (int i = 1; i <= 16; i++) {
        std::string port = "COM" + std::to_string(i);

        // Try to open the port to check if it exists
        HANDLE hComm = CreateFileA(
            port.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,                          // No sharing
            NULL,                       // No security
            OPEN_EXISTING,              // Open existing port only
            0,                          // Non overlapped I/O
            NULL                        // Null for comm devices
        );

        if (hComm != INVALID_HANDLE_VALUE) {
            // Port exists and is available
            CloseHandle(hComm);
            devices.push_back({port, "Windows COM Port"});
        }
    }

    return devices;
}
#endif

std::vector<DeviceInfo> search_ports(const std::string& baseDir) {
    #ifdef _WIN32
    // For Windows, ignore baseDir and use Windows API to find COM ports
    if (baseDir.find("COM") == 0) {
        return search_windows_com_ports();
    }
    #endif

    std::vector<DeviceInfo> devices;

    // std::cout << baseDir << std::endl;

    std::vector<std::pair<std::string, std::string>> patterns = {
        {"ttyUSB", "Linux USB serial port"},
        {"tty.usbserial", "macOS USB serial port"},
        {"cu.", "macOS Bluetooth serial port"},
        {"COM", "Windows COM port"}
    };

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

