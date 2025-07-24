#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

struct LidarData {
    int distance;
    int time;
    int deviceId;  // 0 for first device, 1 for second device

    LidarData(int d, int t, int id) : distance(d), time(t), deviceId(id) {}
};

class ThreadSafeQueue {
private:
    std::queue<LidarData> queue;
    mutable std::mutex mutex;
    std::condition_variable cond;

public:
    static ThreadSafeQueue& getInstance() {
        static ThreadSafeQueue instance;
        return instance;
    }

    // Push with device ID (0 = first device, 1 = second device)
    void push(int distance, int time, int deviceId = 0);

    // Try to pop any data point (from either device)
    bool try_pop(int& distance, int& time);

    // Try to pop data from a specific device
    bool try_pop_device(int& distance, int& time, int deviceId);

    // Check if queue has data from a specific device
    bool has_data_from_device(int deviceId) const;

private:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
};

#endif  // THREADSAFEQUEUE_H