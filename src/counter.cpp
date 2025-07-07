#include "counter.h"


Counter::Counter() :
    threshold_(0.0),
    window_(std::chrono::seconds(60)),
    running_(false),
    crossings_(0),
    lastUpdateTime_(std::chrono::steady_clock::now()),
    wasBelow_(false),
    stop_flag_(false),
    remainingTimeCallback_(nullptr) {
}

Counter::~Counter() {
    stopCounter();
}

void Counter::setCounterThreshold(double threshold) {
    std::lock_guard<std::mutex> lock(mtx_);
    threshold_ = threshold;
}

void Counter::setTimeWindow(std::chrono::seconds window) {
    std::lock_guard<std::mutex> lock(mtx_);
    window_ = window;
}

void Counter::startCounter() {
    if (!running_.exchange(true)) {
        // Reset crossings when starting
        crossings_ = 0;
        wasBelow_ = false;
        stop_flag_ = false;

        // Set the start time when explicitly starting the counter
        lastUpdateTime_ = std::chrono::steady_clock::now();

        // Start cleanup thread
        cleanup_thread_ = std::thread(&Counter::cleanupThread, this);

        // Initialize the UI immediately with the full time window
        if (remainingTimeCallback_) {
            int minutes = static_cast<int>(window_.count() / 60);
            int seconds = static_cast<int>(window_.count() % 60);

            std::stringstream ss;
            ss << "Time window: "
               << std::setfill('0') << std::setw(2) << minutes << ":"
               << std::setfill('0') << std::setw(2) << seconds
               << " | Crossings: 0";

            remainingTimeCallback_(ss.str());
        }
    }
}

void Counter::stopCounter() {
    if (running_.exchange(false)) {
        stop_flag_ = true;
        if (cleanup_thread_.joinable()) {
            cleanup_thread_.join();
        }

        // Keep showing the last state of the counter (don't clear the UI)
        // The UI will be updated through the regular callback mechanism
    }
}

int Counter::getThresholdCrossings() {
    return crossings_.load();
}

void Counter::updateValue(double value) {
    if (!running_) {
        return;
    }

    std::lock_guard<std::mutex> lock(mtx_);
    auto now = std::chrono::steady_clock::now();

    // Important: Do NOT update lastUpdateTime_ here
    // This prevents the counter timer from resetting when new distance values come in

    // Check if the value is below the threshold
    bool isBelow = value < threshold_;

    // Count a crossing when transitioning from above to below threshold
    if (isBelow && !wasBelow_) {
        // Record the crossing time
        crossingTimes_.push_back(now);
        crossings_++;
    }

    // Update the previous state
    wasBelow_ = isBelow;
}

void Counter::setRemainingTimeCallback(std::function<void(const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(mtx_);
    remainingTimeCallback_ = callback;
}

void Counter::cleanupThread() {
    while (!stop_flag_) {
        // Sleep for a short time to avoid busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::string timeRemainingStr;
        int currentCrossings;

        {
            std::lock_guard<std::mutex> lock(mtx_);
            auto now = std::chrono::steady_clock::now();

            // Remove crossings that are outside the window
            while (!crossingTimes_.empty()) {
                auto oldest = crossingTimes_.front();
                auto age = std::chrono::duration_cast<std::chrono::seconds>(now - oldest);

                if (age > window_) {
                    crossingTimes_.pop_front();
                    // if (crossings_ > 0) {
                    //     crossings_--;
                    // }
                } else {
                    // If the oldest is still within window, the rest are too
                    break;
                }
            }

            // Calculate the remaining time in MM:SS format
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastUpdateTime_);
            auto remaining = window_ - elapsed;

            currentCrossings = crossings_.load();

            // Format the time as MM:SS, ensuring it's displayed even when expired
            int minutes, seconds;
            if (remaining.count() > 0) {
                // Use remaining time when still counting down
                minutes = static_cast<int>(remaining.count() / 60);
                seconds = static_cast<int>(remaining.count() % 60);
            } else {
                // Display 00:00 when time is expired
                minutes = 0;
                seconds = 0;
                stop_flag_ = true;
            }

            std::stringstream ss;
            ss << "Time window: "
               << std::setfill('0') << std::setw(2) << minutes << ":"
               << std::setfill('0') << std::setw(2) << seconds
               << " | Crossings: " << currentCrossings;

            timeRemainingStr = ss.str();
        }

        // Call the callback with the time remaining string - outside the lock
        if (remainingTimeCallback_) {
            remainingTimeCallback_(timeRemainingStr);
        }
    }

    // Send one final update when the thread is stopping
    if (remainingTimeCallback_) {
        int currentCrossings = crossings_.load();
        std::stringstream ss;
        ss << "Time window: 00:00 | Crossings: " << currentCrossings;
        remainingTimeCallback_(ss.str());
    }
}


//TODO: Add a method to stop the counter when the reading is stopped