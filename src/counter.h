#ifndef COUNTER_H
#define COUNTER_H

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

// Counter configuration
void setCounterThreshold(double threshold);
void setTimeWindow(std::chrono::seconds window);

// Counter operations
void startCounter();
void stopCounter();
int getThresholdCrossings();
void updateValue(double value);

#endif
