#include "serial_read.h"

void serial_read(ThreadSafeQueue& queue, const std::string& portName) {
    try {
        RE2 pattern("YY(\\d+)T(\\d+)E", RE2::Quiet);

        // Open serial port with specified settings
        serial_cpp::Serial serial(portName, 115200,
            serial_cpp::Timeout::simpleTimeout(1));

        if (!serial.isOpen()) {
            std::cerr << "Failed to open serial port!" << std::endl;
            throw std::runtime_error("Failed to open serial port!");
        }

        std::cout << "Serial port opened successfully. Reading data..." << std::endl;

        // Continuous reading loop
        while (true) {
            std::string data = serial.read(128); // Try to read up to 1024 bytes

            if (!data.empty()) {
                std::cout << "Received: " << data << std::endl;
                std::cout << "Distance: " << distance_read << " Time: " << time_read  << std::endl;
                if (RE2::PartialMatch(data, pattern, &distance_read, &time_read)) {
                        queue.push(distance_read, time_read);
                }
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}