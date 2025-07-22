#include "connection.h"

void stop_reading(int /*signal*/) {
    running = false;
}

std::vector<DeviceInfo> connection_search() {
    // With our improved port searching using serial_cpp, we don't need
    // platform-specific paths anymore
    std::vector<DeviceInfo> devices = search_ports();

    // If no devices found, try platform-specific fallback paths
    if (devices.empty()) {
        #ifdef _WIN32
            devices = search_ports("COM");  // Windows uses COM ports
        #else
            devices = search_ports("/dev/");
        #endif
    }

    return devices;
}

int connection_init() {
    running = true;

    auto& settings = SettingsManager::getInstance();
    std::string selectedPort = settings.getPort();
    int baudRate = settings.getBaudRate();

        if (!selectedPort.empty()) {
            // std::cout << selectedPort << std::endl;

            // Start reading thread
            std::thread read_data_thread(serial_read, selectedPort, baudRate);

            // Don't detach the thread, we'll join it later for clean shutdown
            auto start_time = std::chrono::steady_clock::now();
            const auto TIMEOUT = std::chrono::seconds(10);

            int distance, time;
            bool got_initial_data = false;

            while (running) {
                if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
                    got_initial_data = true;
                    int *initial_time = &global_start_time_one;
                    *initial_time = time;  // Store the initial time
                    break;
                }

                if (std::chrono::steady_clock::now() - start_time > TIMEOUT) {
                    running = false;
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            if (!got_initial_data) {
                running = false;
                read_data_thread.join();  // Wait for thread to finish
                throw std::runtime_error("No data received within timeout");
            }

            // Continuous reading loop
            // while (running) {
            //     // if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
            //     //     // std::cout << "Distance= " << distance
            //     //     //          << ", Time= " << time << std::endl;
            //     // }
            //
            //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
            // }

            // Commented out for testing but without excess loops the program functions faster
            // Now only wrap my head around fixing the initial data as it doesn't get it correctly

            // Clean shutdown
            read_data_thread.join();  // Wait for thread to finish
        } else {
            std::cerr << "No ports found" << std::endl;
            return 1;
        }

    return 0;

}
