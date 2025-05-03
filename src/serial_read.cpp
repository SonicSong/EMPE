#include "serial_read.h"

int serial_read(const std::string& portName) {
    try {
        // Open serial port with specified settings
        serial_cpp::Serial serial(portName, 115200,
            serial_cpp::Timeout::simpleTimeout(1));

        if (!serial.isOpen()) {
            std::cerr << "Failed to open serial port!" << std::endl;
            return 1;
        }

        std::cout << "Serial port opened successfully. Reading data..." << std::endl;

        std::regex pattern("(\\d{2})(\\d{3})([A-Z])(\\d{5})E\\s*");

        // Continuous reading loop
        while (true) {
            std::string data = serial.read(128); // Try to read up to 1024 bytes
            if (!data.empty()) {
                std::string transformed = std::regex_replace(
                    data,
                    pattern,
                    "$1: $2 T: $3$4"
                );


                std::cout << "Original: " << data << "Transformed: " << transformed << std::endl;

            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}