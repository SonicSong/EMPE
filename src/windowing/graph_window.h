#ifndef GRAPH_WINDOW_H
#define GRAPH_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkchart.h>
#include <iostream>
#include <thread>
#include <glibmm/main.h>
#include <glibmm/wrap.h>
#include <gtk/gtk.h>
#include <gtkmm/button.h>
#include <gtk-4.0/gtk/gtkwidget.h>

#include "../settings_manager.h"
#include "../ThreadSafeQueue.h"

class GraphWindow : public Gtk::Window {
public:
    explicit GraphWindow(const std::vector<std::pair<int, int>>& initial_data);
    ~GraphWindow() override;

private:
    Gtk::Box m_box;
    GtkChart* m_chart;
    std::atomic<bool> m_running;
    std::thread m_update_thread;
    GMutex m_mutex;
    Gtk::Button m_close_window;
    double current_max_x;
    double current_max_y;

    struct Point {
        double x;
        double y;
        GtkChart* chart;
    };

    static gboolean plot_point_callback(gpointer user_data);
    void update_thread_function();
    void setup_chart();
    std::deque<std::pair<double, double>> m_data_points;  // Use deque for efficient front removal
    void on_close_clicked();


};

#endif //GRAPH_WINDOW_H