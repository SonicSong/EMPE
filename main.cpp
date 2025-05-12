#include <iostream>
#include <gtkmm.h>
#include <thread>
#include <atomic>
#include <csignal>

#include "src/serial_read.h"
#include "src/serial_search.h"
#include "src/ThreadSafeQueue.h"
#include "src/atomic_vars.h"

void stop_reading() {
    running = false;
}

int main(int argc, char **argv) {
    running = true;
    std::vector<DeviceInfo> devices = search_ports("/dev/");

    if (!devices.empty()) {
        std::string selectedPort = select_port(devices);

        if (!selectedPort.empty()) {
            std::cout << selectedPort << std::endl;

            // Start reading thread
            std::thread read_data_thread(serial_read, selectedPort);
            read_data_thread.detach();

            // Set up signal handler for clean shutdown
            signal(SIGINT, __sighandler_t(stop_reading));  // Ctrl+C

            auto start_time = std::chrono::steady_clock::now();
            const auto TIMEOUT = std::chrono::seconds(5);

            int distance, time;
            bool got_initial_data = false;

            // Wait for initial data
            while (running) {
                if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
                    std::cout << "Received: Distance=" << distance
                             << ", Time=" << time << std::endl;
                    got_initial_data = true;
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            if (!got_initial_data) {
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
        }
    } else {
        std::cerr << "No ports found" << std::endl;
        return 1;
    }

    return 0;
}