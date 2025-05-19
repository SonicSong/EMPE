#include "window.h"
#include <iomanip>
#include <sstream>
#include <glibmm/main.h>

MainWindow::MainWindow() 
    : m_box(Gtk::Orientation::VERTICAL),
      m_button("Start"),
      m_settings_button("Port settings"),
      m_distance_label("Distance: 0"),
      m_time_label("Time: 00:00:00"),
      m_settings_window(nullptr),
      is_running(false) {
    
    m_box.set_margin(10);
    m_box.set_spacing(10);

    m_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_button_clicked));
    m_settings_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_settings_button_clicked));

    // Update at ~60fps for smoother display
    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &MainWindow::update_labels),
        2
    );
    //TODO: Either leave interval to be custom or hardcode it. It only is for display so it can be a bit bigger. For Graph it needs to be more precise so it can't be like with 16ms delay but rather up to 5-10 MAX.

    m_box.append(m_button);
    m_box.append(m_settings_button);
    m_box.append(m_distance_label);
    m_box.append(m_time_label);

    set_child(m_box);
}

MainWindow::~MainWindow() {
    running = false;
    is_running = false;
    if (connection_thread.joinable()) {
        connection_thread.join();
    }
    delete m_settings_window;
}

bool MainWindow::update_labels() {
    int distance, time;
    if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
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
    return true; // Keep the timer running
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
    std::cerr << "NIE MA ZAPISU" << std::endl;
}

void MainWindow::on_about_button_clicked() {
    std::cout << "Nie ma About" << std::endl;
}


void MainWindow::create_graph() {
    std::cout << "NIE MA GRAFIKI" << std::endl;
}

void MainWindow::select_port_button() {
    std::cout << "NIE MA PORTU" << std::endl;
}