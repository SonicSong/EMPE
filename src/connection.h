#ifndef CONNECTION_H
#define CONNECTION_H

#include <atomic>
#include <vector>

#include "serial_read.h"
#include "serial_search.h"
#include "ThreadSafeQueue.h"
#include "atomic_vars.h"
#include "settings_manager.h"

void stop_reading(int signal);

std::vector<DeviceInfo> connection_search();

int connection_init();

extern std::atomic<bool> running_second_lidar;

#endif