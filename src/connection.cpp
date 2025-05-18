#include "connection.h"

void stop_reading(int /*signal*/) {
    running = false;
}

std::vector<DeviceInfo> connection_search() {
    #ifdef _WIN32
        std::vector<DeviceInfo> devices = search_ports("COM");  // Windows uses COM ports
    #else
         std::vector<DeviceInfo> devices = search_ports("/dev/");
    #endif

    return devices;
}

int connection_init() {
    running = true;


    auto& settings = SettingsManager::getInstance();
    std::string selectedPort = settings.getPort();
    int baudRate = settings.getBaudRate();


    // Set up platform-specific signal handlers for clean shutdown
    #ifdef _WIN32
        if (!SetConsoleCtrlHandler(windows_ctrl_handler, TRUE)) {
            std::cerr << "Error setting up Windows control handler" << std::endl;
            return 1;
        }
    #else
        struct sigaction sa;
        sa.sa_handler = stop_reading;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);

        if (sigaction(SIGINT, &sa, nullptr) == -1) {
            std::cerr << "Error setting up signal handler" << std::endl;
            return 1;
        }
    #endif

        if (!selectedPort.empty()) {
            // std::cout << selectedPort << std::endl;

            // Start reading thread
            std::thread read_data_thread(serial_read, selectedPort, baudRate);

            // Don't detach the thread, we'll join it later for clean shutdown
            auto start_time = std::chrono::steady_clock::now();
            const auto TIMEOUT = std::chrono::seconds(5);

            int distance, time;
            bool got_initial_data = false;

            // Wait for initial data

            // TODO: It hangs up the thread so GUI isn't responsible.
            // TODO: Make this into a thread so that GTK can have the ability to display it.
            while (running) {
                if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
                    std::cout << "Received: Distance=" << distance
                             << ", Time=" << time << std::endl;
                    got_initial_data = true;
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
            while (running) {
                if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
                    std::cout << "Distance= " << distance
                             << ", Time= " << time << std::endl;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            // Clean shutdown
            read_data_thread.join();  // Wait for thread to finish
        } else {
            std::cerr << "No ports found" << std::endl;
            return 1;
        }

    return 0;

}
