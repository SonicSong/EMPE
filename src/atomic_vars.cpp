#include "atomic_vars.h"

std::atomic<bool> running(false);
std::atomic<bool> running_second_lidar(false);

std::atomic<int> g_readingsPerSecond(10); // Default to 10 readings per second

int global_start_time_one = 0;
int global_start_time_two = 0;
