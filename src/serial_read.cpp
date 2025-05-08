#include "serial_read.h"

int serial_read(const std::string& portName) {
    try {
        RE2 pattern("YY(\\d+)T(\\d+)E", RE2::Quiet);

        // Open serial port with specified settings
        serial_cpp::Serial serial(portName, 115200,
            serial_cpp::Timeout::simpleTimeout(1));

        if (!serial.isOpen()) {
            std::cerr << "Failed to open serial port!" << std::endl;
            return 1;
        }

        std::cout << "Serial port opened successfully. Reading data..." << std::endl;

        // Continuous reading loop
        while (true) {
            std::string data = serial.read(128); // Try to read up to 1024 bytes

            if (!data.empty()) {
                std::cout << "Received: " << data << std::endl;
                RE2::PartialMatch(data, pattern, &distance_read, &time_read);

                std::cout << "Distance: " << distance_read << " Time: " << time_read  << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

}