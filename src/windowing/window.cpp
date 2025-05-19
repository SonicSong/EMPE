#include "window.h"
#include <iomanip>
#include <sstream>
#include <glibmm/main.h>

MainWindow::MainWindow() 
    : m_box(Gtk::Orientation::VERTICAL),
      m_button_box(Gtk::Orientation::HORIZONTAL),
      m_label_box(Gtk::Orientation::HORIZONTAL),
      m_button("Start"),
      m_settings_button("Port settings"),
      m_save_button("Save Data"),
      m_graph_button("Show Graph"),
      m_distance_label("Distance: 0"),
      m_time_label("Time: 00:00:00"),
      m_settings_window(nullptr),
      m_graph_window(nullptr),
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
    m_save_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::save_data));

    // Update using the interval from settings
    auto& settings = SettingsManager::getInstance();
    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &MainWindow::update_labels),
        static_cast<guint>(settings.getInterval())
    );

    // Add buttons to the horizontal button box
    m_button_box.append(m_button);
    m_button_box.append(m_settings_button);
    m_button_box.append(m_save_button);

    // Connect graph button signal
    m_graph_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::create_graph));

    // Add graph button to button box
    m_button_box.append(m_graph_button);

    // Add labels to the horizontal label box
    m_label_box.append(m_distance_label);
    m_label_box.append(m_time_label);

    // Add the horizontal boxes to the main vertical box
    m_box.append(m_button_box);
    m_box.append(m_label_box);

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
}

bool MainWindow::update_labels() {
    int distance, time;
    if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
        // Store the data point
        data_points.emplace_back(distance, time);

        // Update distance label
        std::stringstream distance_str;
        distance_str << "Distance: " << distance;
        m_distance_label.set_text(distance_str.str());

        // Format time as HH:MM:SS
        int hours = time / 3600;
        int minutes = (time % 3600) / 60;
        int seconds = time % 60;

        std::stringstream time_str;
        time_str << "Time: "
                << std::setfill('0') << std::setw(2) << hours << ":"
                << std::setfill('0') << std::setw(2) << minutes << ":"
                << std::setfill('0') << std::setw(2) << seconds;
        m_time_label.set_text(time_str.str());
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
    m_settings_window->show();
}

void MainWindow::save_data() {
    if (data_points.empty()) {
        std::cerr << "No data to save" << std::endl;
        return;
    }

    // Get current time for filename
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream filename;
    filename << "measurement_" << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S") << ".csv";

    try {
        std::ofstream file(filename.str());
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename.str() << std::endl;
            return;
        }

        // Write CSV header
        file << "Distance,Time(s)\n";

        // Write data points
        for (const auto& [distance, time] : data_points) {
            // Split time into seconds and milliseconds
            int seconds = time / 1000;
            int milliseconds = time % 1000;

            file << distance << ","
                 << seconds << "."
                 << std::setfill('0') << std::setw(3) << milliseconds
                 << "\n";
        }

        file.close();
        std::cout << "Data saved to: " << filename.str() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error saving data: " << e.what() << std::endl;
    }
}

//TODO: Fix saving data.

void MainWindow::on_about_button_clicked() {
    std::cout << "Nie ma About" << std::endl;
}


void MainWindow::create_graph() {
    if (data_points.empty()) {
        std::cerr << "No data to display" << std::endl;
        return;
    }

    if (!m_graph_window) {
        m_graph_window = new GraphWindow(data_points);
        m_graph_window->signal_hide().connect(
            sigc::bind(sigc::mem_fun(*this, &MainWindow::on_graph_window_hide)));
    }
    m_graph_window->show();
}

void MainWindow::on_graph_window_hide() {
    delete m_graph_window;
    m_graph_window = nullptr;
}

void MainWindow::select_port_button() {
    std::cout << "NIE MA PORTU" << std::endl;
}