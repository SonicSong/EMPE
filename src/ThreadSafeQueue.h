#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadSafeQueue {
private:
    std::queue<std::pair<int, int>> queue;
    mutable std::mutex mutex;
    std::condition_variable cond;

public:
    void push(int distance, int time);
    bool try_pop(int& distance, int& time);
};

#endif  // THREADSAFEQUEUE_H