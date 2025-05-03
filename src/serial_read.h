#ifndef SERIAL_READ_H
#define SERIAL_READ_H

#include <serial_cpp/serial.h>
#include <string>
#include <regex>
#include <iostream>
#include <cstdio>

int serial_read(const std::string& portName);

#endif //SERIAL_READ_H
