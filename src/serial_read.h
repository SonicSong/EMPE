#ifndef SERIAL_READ_H
#define SERIAL_READ_H

#include <serial_cpp/serial.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <re2/re2.h>
#include "ThreadSafeQueue.h"
#include "atomic_vars.h"


void serial_read(const std::string& portName);

#endif //SERIAL_READ_H