#ifndef SERIAL_READ_H
#define SERIAL_READ_H

#include <serial_cpp/serial.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <re2/re2.h>
#include "ThreadSafeQueue.h"
#include "atomic_vars.h"

void serial_read(const std::string& portName, int baudrate, int deviceId);
extern std::atomic<int> g_readingsPerSecond;

#endif //SERIAL_READ_H