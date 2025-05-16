#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <thread>
#include <atomic>
#include <csignal>

#include "serial_read.h"
#include "serial_search.h"
#include "ThreadSafeQueue.h"
#include "atomic_vars.h"

void stop_reading(int signal);

int connection_init();

#endif