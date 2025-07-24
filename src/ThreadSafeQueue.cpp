#include "ThreadSafeQueue.h"

void ThreadSafeQueue::push(int distance, int time, int deviceId) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(LidarData(distance, time, deviceId));
    cond.notify_one();
}

bool ThreadSafeQueue::try_pop(int& distance, int& time) {
    std::lock_guard<std::mutex> lock(mutex);
    if (queue.empty()) return false;

    LidarData data = queue.front();
    distance = data.distance;
    time = data.time;
    queue.pop();
    return true;
}

bool ThreadSafeQueue::try_pop_device(int& distance, int& time, int deviceId) {
    std::lock_guard<std::mutex> lock(mutex);

    // Find the first data point from the specified device
    std::queue<LidarData> temp;
    bool found = false;

    while (!queue.empty() && !found) {
        LidarData data = queue.front();
        queue.pop();

        if (data.deviceId == deviceId && !found) {
            distance = data.distance;
            time = data.time;
            found = true;
        } else {
            temp.push(data); // Keep other data points
        }
    }

    // Put back any data we removed but didn't use
    while (!temp.empty()) {
        queue.push(temp.front());
        temp.pop();
    }

    return found;
}

bool ThreadSafeQueue::has_data_from_device(int deviceId) const {
    std::lock_guard<std::mutex> lock(mutex);

    std::queue<LidarData> temp = queue; // Make a copy to search
    while (!temp.empty()) {
        if (temp.front().deviceId == deviceId) {
            return true;
        }
        temp.pop();
    }

    return false;
}