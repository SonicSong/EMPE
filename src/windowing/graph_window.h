#ifndef GRAPH_WINDOW_H
#define GRAPH_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkchart.h>
#include <iostream>
#include <thread>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <glibmm/main.h>
#include <glibmm/wrap.h>
#include <gtk/gtk.h>
#include <gtkmm/button.h>
#include <gtk-4.0/gtk/gtkwidget.h>

#include "../settings_manager.h"
#include "../atomic_vars.h"
#include "../ThreadSafeQueue.h"

extern int global_start_time_one;
extern int global_start_time_two;  // Added for second LiDAR

class GraphWindow : public Gtk::Window {
public:
    explicit GraphWindow();
    ~GraphWindow() override;

private:
    Gtk::Box m_box;
    GtkChart* m_chart;     // First LiDAR chart
    GtkChart* m_chart2;    // Second LiDAR chart
    std::atomic<bool> m_running;
    std::thread m_update_thread;
    GMutex m_mutex;
    Gtk::Button m_close_window;
    Gtk::Button m_save_button;
    Gtk::Button m_save_button2;  // Second save button for second LiDAR
    // double current_max_x;
    double current_max_y;
    double current_max_y_2;

    struct Point {
        double x;
        double y;
        GtkChart* chart;
    };

    static gboolean plot_point_callback(gpointer user_data);
    void update_thread_function();
    void setup_chart();
    void setup_second_chart(); // New method to setup the second chart
    std::deque<std::pair<double, double>> m_data_points;  // First LiDAR data
    std::deque<std::pair<double, double>> m_data_points2; // Second LiDAR data
    void on_close_clicked();
    void on_save_clicked(int chartIndex = 0);  // Modified to accept chart index
    void on_save_clicked_chart2();  // Wrapper for second chart
};

#endif //GRAPH_WINDOW_H