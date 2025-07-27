#include "serial_read.h"

void serial_read(const std::string& portName, int baudrate, int deviceId) {
    try {
        RE2 pattern("YY(\\d+)T(\\d+)E", RE2::Quiet);
        ThreadSafeQueue& queue = ThreadSafeQueue::getInstance();
        int distance_read = 0, time_read = 0;  // Declare here

        // Get reading frequency from settings manager
        auto& settings = SettingsManager::getInstance();

        std::string buffer;
        buffer.reserve(256); // Pre-allocate buffer to avoid reallocations

        // Open serial port with specified settings
        serial_cpp::Serial serial(portName, baudrate);
            // serial_cpp::Timeout::simpleTimeout(1)

        if (!serial.isOpen()) {
            // std::cerr << "Failed to open serial port " << portName << " for deviceId=" << deviceId << std::endl;
            throw std::runtime_error("Failed to open serial port!");
        }

        const size_t chunk_size = 16; // Smaller chunks for more frequent processing
        std::string chunk;
        chunk.reserve(chunk_size);

        // For reading frequency control
        auto last_read_time = std::chrono::steady_clock::now();

        while (running.load() && (deviceId == 0 || (deviceId == 1 && running_second_lidar.load()))) {
            // Calculate delay based on reading frequency
            // !!!THIS ONLY SLOWS DOWN READ OUT. NOT ACTUALLY MAKES STOPS IN READING!!!
            // FIXME: Use a more precise timing mechanism
            int readingsPerSecond = settings.getReadoutsFromLidar();
            std::chrono::milliseconds delay(1000 / readingsPerSecond);

            // Check if it's time to read
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_read_time);

            if (elapsed < delay) {
                // Not enough time has passed, sleep for a bit
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }

            // It's time to read
            last_read_time = current_time;

            chunk = serial.read(chunk_size);
            if (!chunk.empty()) {
                buffer += chunk;

                size_t pos;
                while ((pos = buffer.find('E')) != std::string::npos) {
                    std::string message = buffer.substr(0, pos + 1);
                    if (RE2::PartialMatch(message, pattern, &distance_read, &time_read)) {
                        queue.push(distance_read, time_read, deviceId);
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
