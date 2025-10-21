#include "atomic_vars.h"

std::atomic<bool> running(false);
std::atomic<bool> running_second_lidar(false);

int global_start_time_one = 0;
int global_start_time_two = 0;

int global_start_time_first_s_window = 0;
int global_start_time_second_s_window = 0;