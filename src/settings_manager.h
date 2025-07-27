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
    void setViewportHeight(int height) { m_viewport_height = height; }
    void setAutoScrollViewport(bool autoScroll) { m_auto_scroll_viewport = autoScroll; }
    void setAutoViewportHeight(bool autoHeight) { m_auto_viewport_height = autoHeight; }

    void setSecondPort(bool secondPortEnabled) { m_second_port_enabled = secondPortEnabled; }
    void setPort2(const std::string& port) {selectedSecondPort = port;}
    void setBaudRate2(int baud) { selectedSecondBaudRate = baud; }

    std::string getPort() const { return selectedPort; }
    int getBaudRate() const { return selectedBaudRate; }
    int getViewportWidth() const { return m_viewport_width; }
    int getViewportHeight() const { return m_viewport_height; }
    bool getAutoScrollViewport() const { return m_auto_scroll_viewport; }
    bool getAutoViewportHeight() const { return m_auto_viewport_height; }

    bool getSecondPort() const { return m_second_port_enabled; }

    std::string getPort2() const { return selectedSecondPort; }
    int getBaudRate2() const { return selectedSecondBaudRate; }

    void setReadoutsFromLidar(int readouts) { m_readouts_from_lidar = readouts; }
    int getReadoutsFromLidar() const { return m_readouts_from_lidar; }

    // Common baud rates
    static const std::vector<int> availableBaudRates;

private:
    SettingsManager() : selectedPort(""), selectedBaudRate(115200), m_viewport_width(5000), m_viewport_height(100), m_auto_scroll_viewport(false), m_auto_viewport_height(false),
                       m_second_port_enabled(false), selectedSecondPort(""), selectedSecondBaudRate(115200), m_readouts_from_lidar(10) {}

    std::string selectedPort;
    int selectedBaudRate;
    int m_viewport_width;
    int m_viewport_height;

    int m_readouts_from_lidar;

    bool m_auto_scroll_viewport;
    bool m_auto_viewport_height;

    bool m_second_port_enabled;
    std::string selectedSecondPort;
    int selectedSecondBaudRate;
};

#endif // SETTINGS_MANAGER_H