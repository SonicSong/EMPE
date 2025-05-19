#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <string>
#include <vector>

class SettingsManager {
public:
    static SettingsManager& getInstance() {
        static SettingsManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;

    void setPort(const std::string& port) { selectedPort = port; }
    void setBaudRate(int baud) { selectedBaudRate = baud; }

    std::string getPort() const { return selectedPort; }
    int getBaudRate() const { return selectedBaudRate; }

    void setInterval(int interval) { }

    int getInterval() const { return 4; }

    void setGraphTimeWindow(int seconds) { m_graph_time_window = seconds; }
    int getGraphTimeWindow() const { return m_graph_time_window; }


    // Common baud rates
    static const std::vector<int> availableBaudRates;

private:
    SettingsManager() : selectedPort(""), selectedBaudRate(115200) {} // Default values

    std::string selectedPort;
    int selectedBaudRate;
    int m_graph_time_window = 60; // Default 60 seconds
};

#endif // SETTINGS_MANAGER_H