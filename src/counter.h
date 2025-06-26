#ifndef COUNTER_H
#define COUNTER_H

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <deque>
#include <functional>
#include <sstream>
#include <iomanip>

class Counter {
public:
    Counter();
    ~Counter();

    // Counter configuration
    void setCounterThreshold(double threshold);
    void setTimeWindow(std::chrono::seconds window);

    // Counter operations
    void startCounter();
    void stopCounter();
    int getThresholdCrossings();
    void updateValue(double value);

    // Set callback for time remaining updates
    void setRemainingTimeCallback(std::function<void(const std::string&)> callback);

private:
    void cleanupThread();

    double threshold_;
    std::chrono::seconds window_;
    std::atomic<bool> running_;
    std::atomic<int> crossings_;
    std::chrono::steady_clock::time_point lastUpdateTime_;
    std::deque<std::chrono::steady_clock::time_point> crossingTimes_;
    std::mutex mtx_;
    bool wasBelow_;
    std::thread cleanup_thread_;
    std::atomic<bool> stop_flag_;
    std::function<void(const std::string&)> remainingTimeCallback_;
};

#endif
