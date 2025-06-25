#ifndef COUNTER_H
#define COUNTER_H

#include <chrono>
#include <deque>
#include <mutex>
#include <atomic>
#include <thread>

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
};

#endif
