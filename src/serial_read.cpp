#include "serial_read.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

int serial_read(const std::string& portName) {
    try {
        // Open serial port with specified settings
        serial_cpp::Serial serial(portName, 115200,
            serial_cpp::Timeout::simpleTimeout(1));

        if (!serial.isOpen()) {
            cerr << "Failed to open serial port!" << endl;
            return 1;
        }

        cout << "Serial port opened successfully. Reading data..." << endl;

        // Continuous reading loop
        while (true) {
            string data = serial.read(128); // Try to read up to 1024 bytes
            if (!data.empty()) {
                cout << "Received: " << data << endl;
            }
        }

    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}