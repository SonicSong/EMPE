#ifndef ATOMIC_VARS_H
#define ATOMIC_VARS_H

#include <atomic>

extern std::atomic<bool> running;
extern std::atomic<bool> running_second_lidar;
extern int global_start_time_one;
extern int global_start_time_two;

#endif // ATOMIC_VARS_H