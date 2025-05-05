#ifndef SERIAL_READ_H
#define SERIAL_READ_H

#include <serial_cpp/serial.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <re2/re2.h>

int serial_read(const std::string& portName);

// std::string dataBuffer;
inline int distance_read;
inline int time_read;

#endif //SERIAL_READ_H

