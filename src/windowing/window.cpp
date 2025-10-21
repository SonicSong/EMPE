#include "window.h"
#include <iomanip>
#include <sstream>
#include <glibmm/main.h>

MainWindow::MainWindow()
    : m_box(Gtk::Orientation::VERTICAL),
      m_button_box(Gtk::Orientation::HORIZONTAL),
      m_label_box(Gtk::Orientation::HORIZONTAL),
      m_counter_box(Gtk::Orientation::HORIZONTAL),
      m_button("Start"),
      m_settings_button("Port settings"),
      m_graph_button("Show Graph"),
      m_counter_start_button("Start Counter"),
      m_distance_label("Distance: 0"),
      m_distance_label2("Distance 2: 0"),
      m_time_label("Time: 00:00:00"),
      m_time_label2("Time: 00:00:00"),
      m_about_button("About"),
      m_settings_window(nullptr),
      m_graph_window(nullptr),
      m_licenses_window(nullptr),  // Explicitly initialize to nullptr
      is_running(false) {

    m_box.set_margin(10);
    m_box.set_spacing(10);

    // Set spacing for button and label boxes
    m_button_box.set_spacing(10);
    m_label_box.set_spacing(10);

    // Connect signals
    m_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_button_clicked));
    m_settings_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_settings_button_clicked));

    auto& settings = SettingsManager::getInstance();
    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &MainWindow::update_labels),
        static_cast<guint>(16)
    );

    // Add buttons to the horizontal button box
    m_button_box.append(m_button);
    m_button_box.append(m_settings_button);

    // Connect graph button signal
    m_graph_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::create_graph));

    // Add graph button to button box
    m_button_box.append(m_graph_button);

    m_button_box.append(m_about_button);

    m_about_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_about_button_clicked));


    // Add labels to the horizontal label box
    m_label_box.append(m_distance_label);
    m_label_box.append(m_time_label);
    m_label_box2.append(m_distance_label2);
    m_label_box2.append(m_time_label2);

    if (SettingsManager::getInstance().getSecondPort()) {
        m_label_box2.show();
    } else {
        m_label_box2.hide();
    }

    // Add the horizontal boxes to the main vertical box
    m_box.append(m_button_box);
    m_box.append(m_label_box);
    m_box.append(m_label_box2);

    // Initialize second LiDAR visibility
    update_second_lidar_visibility();

    // Setup counter UI elements
    m_counter_box.set_spacing(10);
    m_threshold_label.set_text("Threshold:");
    m_window_label.set_text("Time Window (s):");
    // m_crossings_label.set_text("Crossings: 0");
    m_counter_time_label.set_text("");  // Initialize the counter time label

    // Configure spin buttons
    m_threshold_spin.set_range(0, 1000);
    m_threshold_spin.set_increments(1, 10);
    m_threshold_spin.set_value(100);

    m_window_spin.set_range(1, 3600);
    m_window_spin.set_increments(1, 10);
    m_window_spin.set_value(60);

    // Configure reading frequency control
    m_reading_freq_box.set_orientation(Gtk::Orientation::HORIZONTAL);
    m_reading_freq_box.set_spacing(10);
    m_reading_freq_label.set_text("Readings per second:");
    m_reading_freq_spin.set_range(1, 50);
    m_reading_freq_spin.set_increments(1, 5);
    m_reading_freq_spin.set_value(settings.getReadoutsFromLidar());
    m_reading_freq_spin.signal_value_changed().connect(
        sigc::mem_fun(*this, &MainWindow::on_reading_freq_changed));

    m_reading_freq_box.append(m_reading_freq_label);
    m_reading_freq_box.append(m_reading_freq_spin);

    // Add reading frequency control to main box
    m_box.append(m_reading_freq_box);

    // Connect counter signals
    m_counter_start_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_counter_start_clicked));
    m_threshold_spin.signal_value_changed().connect(
        sigc::mem_fun(*this, &MainWindow::on_threshold_changed));
    m_window_spin.signal_value_changed().connect(
        sigc::mem_fun(*this, &MainWindow::on_window_changed));

    // Set the callback for counter time updates
    m_counter.setRemainingTimeCallback(
        sigc::mem_fun(*this, &MainWindow::update_counter_time));

    // Add counter elements to counter box
    m_counter_box.append(m_threshold_label);
    m_counter_box.append(m_threshold_spin);
    m_counter_box.append(m_window_label);
    m_counter_box.append(m_window_spin);
    m_counter_box.append(m_counter_start_button);
    // m_counter_box.append(m_crossings_label);

    // Add the counter time label to the UI
    m_box.append(m_counter_time_label);

    // Add counter box to main box after the existing elements
    m_box.append(m_counter_box);

    set_child(m_box);
}

MainWindow::~MainWindow() {
    running = false;
    is_running = false;
    if (connection_thread.joinable()) {
        connection_thread.join();
    }
    delete m_settings_window;
    delete m_graph_window;
    delete m_licenses_window; // Make sure we clean up the licenses window
}

bool MainWindow::update_labels() {
    int distance, time;
    if (ThreadSafeQueue::getInstance().try_pop_device(distance, time, 0)) {
        // Store the data point
        // data_points.emplace_back(distance, time);

        // Update counter with new distance value
        m_counter.updateValue(distance);

        // Update crossings label
        // std::stringstream crossings_str;
        // crossings_str << "Crossings: " << m_counter.getThresholdCrossings();
        // m_crossings_label.set_text(crossings_str.str());

        // Update distance label
        std::stringstream distance_str;
        distance_str << "LiDAR " << 0 << " - Distance: " << distance;
        m_distance_label.set_text(distance_str.str());

        // Format time as HH:MM:SS
        int hours = time / 3600;
        int minutes = (time % 3600) / 60;
        int seconds = time % 60;

        std::stringstream time_str;
        time_str << " Time: "
                << std::setfill('0') << std::setw(2) << hours << ":"
                << std::setfill('0') << std::setw(2) << minutes << ":"
                << std::setfill('0') << std::setw(2) << seconds;
        m_time_label.set_text(time_str.str());
    }

    if (SettingsManager::getInstance().getSecondPort()) {
        int distance2, time2;
        if (ThreadSafeQueue::getInstance().try_pop_device(distance2, time2, 1)) {
            // Store the data point for second LiDAR
            // data_points2.emplace_back(distance2, time2);

            // Update counter with new distance value for second LiDAR
            m_counter.updateValue(distance2);

            // Update distance label for second LiDAR
            std::stringstream distance_str2;
            distance_str2 << "LiDAR " << 1 << " - Distance: " << distance2;
            m_distance_label2.set_text(distance_str2.str());

            // Format time as HH:MM:SS for second LiDAR
            int hours = time2 / 3600;
            int minutes = (time2 % 3600) / 60;
            int seconds = time2 % 60;

            std::stringstream time_str2;
            time_str2 << " Time: "
                    << std::setfill('0') << std::setw(2) << hours << ":"
                    << std::setfill('0') << std::setw(2) << minutes << ":"
                    << std::setfill('0') << std::setw(2) << seconds;
            m_time_label2.set_text(time_str2.str());
        }
    }
    return true;
}

void MainWindow::on_button_clicked() {
    if (!is_running) {
        is_running = true;
        running = true;

        // Start the connection in a separate thread
        connection_thread = std::thread([this]() {
            try {
                connection_init();
            } catch (const std::exception& e) {
                std::cerr << "Connection error: " << e.what() << std::endl;
                is_running = false;
                running = false;
            }
        });

        m_button.set_label("Stop");
    } else {
        running = false;
        is_running = false;
        if (connection_thread.joinable()) {
            connection_thread.join();
        }
        m_button.set_label("Start");
    }
}

void MainWindow::on_settings_button_clicked() {
    if (!m_settings_window) {
        m_settings_window = new SettingsWindow();
    }

    // Connect to the settings window hide signal to update visibility when settings are closed
    m_settings_window->signal_hide().connect([this]() {
        // Update second LiDAR visibility when settings window is closed
        update_second_lidar_visibility();
    });

    m_settings_window->show();
}

void MainWindow::on_about_button_clicked() {
    if (!m_licenses_window) {
        m_licenses_window = new Licenses();
        m_licenses_window->signal_hide().connect(
            sigc::mem_fun(*this, &MainWindow::on_licenses_window_hide));
    }
    m_licenses_window->show();
}

void MainWindow::on_licenses_window_hide() {
    delete m_licenses_window;
    m_licenses_window = nullptr;
}

// void MainWindow::create_graph() {
//     if (!m_graph_window) {
//         int distance, time;
//         if (ThreadSafeQueue::getInstance().try_pop_device(distance, time, 0)) {
//             global_start_time_first_s_window = time;
//         }
//
//         // Set second LiDAR start time if second port is enabled
//         if (SettingsManager::getInstance().getSecondPort()) {
//             int distance2, time2;
//             if (ThreadSafeQueue::getInstance().try_pop_device(distance2, time2, 1)) {
//                 global_start_time_second_s_window = time2;
//             }
//         }
//
//         std::cout << "Value assigned: global_start_time_first_s_window = "
//                   << global_start_time_first_s_window << std::endl;
//         m_graph_window = new GraphWindow();
//         m_graph_window->signal_hide().connect(
//             sigc::mem_fun(*this, &MainWindow::on_graph_window_hide));
//         m_graph_window->show();
//     } else {
//         m_graph_window->present();
//     }
// }

// TODO: Either refactor this or think about better solution as using while and trying to pop data isn't the best solution as it freezes the UI if no data is available

void MainWindow::create_graph() {
    if (!m_graph_window) {
        int distance, time;
        bool has_valid_data = false;
        int retry_count = 0;
        const int max_retries = 10;  // Try up to 10 times
        const int retry_delay_ms = 50;  // Wait 50ms between retries

        // Try to get valid data with retries
        while (retry_count < max_retries && !has_valid_data) {
            if (ThreadSafeQueue::getInstance().try_pop_device(distance, time, 0)) {
                if (time > 0) {
                    global_start_time_first_s_window = time;
                    has_valid_data = true;
                    break;
                }
            }

            if (!has_valid_data) {
                std::this_thread::sleep_for(std::chrono::milliseconds(retry_delay_ms));
                retry_count++;
            }
        }

        // Set second LiDAR start time if second port is enabled
        if (SettingsManager::getInstance().getSecondPort()) {
            int distance2, time2;
            retry_count = 0;

            while (retry_count < max_retries) {
                if (ThreadSafeQueue::getInstance().try_pop_device(distance2, time2, 1)) {
                    if (time2 > 0) {
                        global_start_time_second_s_window = time2;
                        break;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(retry_delay_ms));
                retry_count++;
            }
        }

        // Only create window if we have valid data
        if (!has_valid_data) {
            std::cerr << "No valid data available to create graph after "
                      << max_retries << " retries" << std::endl;
            return;
        }

        std::cout << "Value assigned: global_start_time_first_s_window = "
                  << global_start_time_first_s_window << std::endl;
        m_graph_window = new GraphWindow();
        m_graph_window->signal_hide().connect(
            sigc::mem_fun(*this, &MainWindow::on_graph_window_hide));
        m_graph_window->show();
    } else {
        m_graph_window->present();
    }
}



void MainWindow::on_graph_window_hide() {
    if (m_graph_window) {
        delete m_graph_window;
        m_graph_window = nullptr;

        global_start_time_first_s_window = 0;
        global_start_time_second_s_window = 0;
        std::cout << "Value reset: global_start_time_first_s_window = "
                  << global_start_time_first_s_window << std::endl;
    }
}

void MainWindow::update_counter_time(const std::string& time_text) {
    // Use Glib::signal_idle to safely update the UI from a non-main thread
    auto update_func = [this, time_text]() {
        m_counter_time_label.set_text(time_text);
        return false; // Return false to run only once
    };

    Glib::signal_idle().connect(update_func);
}

void MainWindow::on_counter_start_clicked() {
    if (m_counter_start_button.get_label() == "Start Counter") {
        m_counter.setCounterThreshold(m_threshold_spin.get_value());
        m_counter.setTimeWindow(std::chrono::seconds(static_cast<int>(m_window_spin.get_value())));
        m_counter.startCounter();
        m_counter_start_button.set_label("Stop Counter");
    } else {
        m_counter.stopCounter();
        m_counter_start_button.set_label("Start Counter");
        // Clear the counter time label when stopping the counter
        m_counter_time_label.set_text("");
    }
}

void MainWindow::on_threshold_changed() {
    m_counter.setCounterThreshold(m_threshold_spin.get_value());
}

void MainWindow::on_window_changed() {
    m_counter.setTimeWindow(std::chrono::seconds(static_cast<int>(m_window_spin.get_value())));
}

void MainWindow::on_reading_freq_changed() {
    // Update the reading frequency in the settings manager
    SettingsManager::getInstance().setReadoutsFromLidar(m_reading_freq_spin.get_value());
}

void MainWindow::update_reading_freq() {

}


void MainWindow::update_second_lidar_visibility() {
    if (SettingsManager::getInstance().getSecondPort()) {
        m_label_box2.show();
    } else {
        m_label_box2.hide();
    }
}
