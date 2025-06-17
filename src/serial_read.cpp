#include "serial_read.h"

void serial_read(const std::string& portName, int baudrate) {
    try {
        RE2 pattern("YY(\\d+)T(\\d+)E", RE2::Quiet);
        ThreadSafeQueue& queue = ThreadSafeQueue::getInstance();
        int distance_read = 0, time_read = 0;  // Declare here

        std::string buffer;
        buffer.reserve(256); // Pre-allocate buffer to avoid reallocations


        // Open serial port with specified settings
        serial_cpp::Serial serial(portName, baudrate,
            serial_cpp::Timeout::simpleTimeout(1));

        if (!serial.isOpen()) {
            std::cerr << "Failed to open serial port!" << std::endl;
            throw std::runtime_error("Failed to open serial port!");
        }

        // std::cout << "Serial port opened successfully. Reading data..." << std::endl;

        const size_t chunk_size = 16; // Smaller chunks for more frequent processing
        std::string chunk;
        chunk.reserve(chunk_size);

        while (running.load()) {
            chunk = serial.read(chunk_size);
            if (!chunk.empty()) {
                buffer += chunk;

                size_t pos;
                while ((pos = buffer.find('E')) != std::string::npos) {
                    std::string message = buffer.substr(0, pos + 1);
                    if (RE2::PartialMatch(message, pattern, &distance_read, &time_read)) {
                        queue.push(distance_read, time_read);
                    }
                    buffer.erase(0, pos + 1);
                }

                // Prevent buffer from growing too large if no valid message is found
                if (buffer.size() > 1024) {
                    size_t lastY = buffer.rfind('Y');
                    if (lastY != std::string::npos) {
                        buffer = buffer.substr(lastY);
                    } else {
                        buffer.clear();
                    }
                }
            }
        }


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}
