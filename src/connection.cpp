#include "connection.h"

void stop_reading(int /*signal*/) {
    running = false;
    running_second_lidar = false;
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
    running_second_lidar = false;

    auto& settings = SettingsManager::getInstance();
    std::string selectedPort = settings.getPort();
    int baudRate = settings.getBaudRate();
    bool secondPortEnabled = settings.getSecondPort();

    std::string selectedPort2;
    int baudRate2 = 0;


    if (secondPortEnabled) {
        selectedPort2 = settings.getPort2();
        baudRate2 = settings.getBaudRate2();
    }

    if (selectedPort.empty()) {
        std::cerr << "No ports selected" << std::endl;
        return 1;
    }

    // Start reading thread for the first LiDAR
    std::thread read_data_thread(serial_read, selectedPort, baudRate, 0); // DeviceId 0 for first LiDAR

    // If second LiDAR is enabled, start a second thread
    std::thread read_data_thread2;
    if (secondPortEnabled && !selectedPort2.empty()) {
        running_second_lidar = true;
        read_data_thread2 = std::thread(serial_read, selectedPort2, baudRate2, 1); // DeviceId 1 for second LiDAR
    } else if (secondPortEnabled) {
        throw std::runtime_error("Second LiDAR enabled but no port selected");
    }

    // Don't detach the thread, we'll join it later for clean shutdown
    auto start_time = std::chrono::steady_clock::now();
    const auto TIMEOUT = std::chrono::seconds(10);

    int distance, time;
    bool got_initial_data = false;

    // Wait for initial data from first LiDAR
    while (running) {
        if (ThreadSafeQueue::getInstance().try_pop_device(distance, time, 0)) {
            got_initial_data = true;
            global_start_time_one = time;  // Store the initial time for first LiDAR
            break;
        }

        if (std::chrono::steady_clock::now() - start_time > TIMEOUT) {
            running = false;
            running_second_lidar = false;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }


    if (got_initial_data && secondPortEnabled && running_second_lidar) {
        start_time = std::chrono::steady_clock::now();

        while (running && running_second_lidar) {
            if (ThreadSafeQueue::getInstance().try_pop_device(distance, time, 1)) {
                global_start_time_two = time;  // Store the initial time for second LiDAR
                break;
            }

            if (std::chrono::steady_clock::now() - start_time > TIMEOUT) {
                running_second_lidar = false;
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    if (!got_initial_data) {
        running = false;
        running_second_lidar = false;
        read_data_thread.join();  // Wait for thread to finish

        if (secondPortEnabled && read_data_thread2.joinable()) {
            read_data_thread2.join();
        }

        throw std::runtime_error("No data received within timeout");
    }

    // Clean shutdown
    if (read_data_thread.joinable()) {
        read_data_thread.join();  // Wait for first thread to finish
    }

    if (secondPortEnabled && read_data_thread2.joinable()) {
        read_data_thread2.join();  // Wait for second thread to finish
    }

    return 0;
}
