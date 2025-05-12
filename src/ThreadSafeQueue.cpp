#include "ThreadSafeQueue.h"

void ThreadSafeQueue::push(int distance, int time) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(std::make_pair(distance, time));
    cond.notify_one();
}

bool ThreadSafeQueue::try_pop(int& distance, int& time) {
    std::lock_guard<std::mutex> lock(mutex);
    if (queue.empty()) return false;
    distance = queue.front().first;
    time = queue.front().second;
    queue.pop();
    return true;
}