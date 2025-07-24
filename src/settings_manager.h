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
    void setViewportWidth(int width) { m_viewport_width = width; }
    void setAutoScrollViewport(bool autoScroll) { m_auto_scroll_viewport = autoScroll; }

    void setSecondPort(bool secondPortEnabled) { m_second_port_enabled = secondPortEnabled; }
    void setPort2(const std::string& port) {selectedPort = port;}
    void setBaudRate2(int baud) { selectedSecondBaudRate = baud; }

    std::string getPort() const { return selectedPort; }
    int getBaudRate() const { return selectedBaudRate; }
    int getViewportWidth() const { return m_viewport_width; }
    bool getAutoScrollViewport() const { return m_auto_scroll_viewport; }
    bool getSecondPort() const { return m_second_port_enabled; }

    void setInterval(int interval) { }
    void setSecondInterval(int interval) { }

    int getInterval() const { return 4; }

    // Common baud rates
    static const std::vector<int> availableBaudRates;

private:
    SettingsManager() : selectedPort(""), selectedBaudRate(115200), m_viewport_width(5000), m_auto_scroll_viewport(true) {} // Default values

    std::string selectedPort;
    int selectedBaudRate;
    int m_viewport_width;  // Added viewport width setting
    bool m_auto_scroll_viewport; // Whether to auto-scroll the viewport or just add points

    bool m_second_port_enabled; // Whether to enable a second port for LIDAR
    std::string selectedSecondPort;
    int selectedSecondBaudRate;
};

#endif // SETTINGS_MANAGER_H