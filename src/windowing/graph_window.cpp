#include "graph_window.h"

//TODO: Possible fix for how much data is displayed? Count elements in vector with data and later just trim to leave the most recent eg. 100 points. Don't use seconds as it's unrelaiable. Use temporary vector to store trimmed data. It should work as continously updated vector. Could be CPU heavy.
// I need to think a bit more about it as it's not stupid but it's not easy. Also need to verify how do I get the data to be displayed if I get it directly from vector that stores the data or from the thread connection_init().
GraphWindow::GraphWindow(const std::vector<std::pair<int, int>>& initial_data)
    : m_box(Gtk::Orientation::VERTICAL),
      m_chart(nullptr),
      m_running(true),
      m_close_window("Close"),
      current_max_x(100.0),
      current_max_y(100.0) {

    set_title("Distance-Time Graph");
    set_default_size(800, 600);
    set_hide_on_close(true);

    g_mutex_init(&m_mutex);

    // Create and setup the chart
    m_chart = GTK_CHART(gtk_chart_new());
    setup_chart();

    // Create a managed Gtk::Widget from the GtkWidget
    Gtk::Widget* chart_widget = Gtk::manage(Glib::wrap(GTK_WIDGET(m_chart), false));
    if (!chart_widget) {
        std::cerr << "Failed to create chart widget" << std::endl;
        return;
    }

    // Make sure the widget is visible
    chart_widget->set_visible(true);
    chart_widget->set_expand(true);  // Allow the chart to expand to fill space

    // Set the chart as the window's child
    //Gtk::Widget* chart_widget = Glib::wrap(GTK_WIDGET(m_chart));

    // Add widgets to box with proper expansion
    m_box.append(*chart_widget);

    // Create close button with proper styling
    m_close_window.set_label("Close");
    m_close_window.get_style_context()->add_class("destructive-action");
    m_close_window.set_halign(Gtk::Align::CENTER);
    m_close_window.set_margin(5);

    // Add close button to box
    m_box.append(m_close_window);

    // Set box as main window content
    set_child(m_box);

    // Connect signal
    m_close_window.signal_clicked().connect(
        sigc::mem_fun(*this, &GraphWindow::on_close_clicked));

    // Start the update thread
    m_update_thread = std::thread(&GraphWindow::update_thread_function, this);
}

GraphWindow::~GraphWindow() {
    m_running = false;
    if (m_update_thread.joinable()) {
        m_update_thread.join();
    }
    g_mutex_clear(&m_mutex);
}

void GraphWindow::setup_chart() {
    gtk_chart_set_type(m_chart, GTK_CHART_TYPE_LINE);
    gtk_chart_set_title(m_chart, "Distance-Time Graph");
    gtk_chart_set_label(m_chart, "Distance Measurement");
    gtk_chart_set_x_label(m_chart, "Time [s]");
    gtk_chart_set_y_label(m_chart, "Distance [units]");
    
    double time_window = static_cast<double>(
        SettingsManager::getInstance().getGraphTimeWindow());
    
    gtk_chart_set_x_max(m_chart, time_window);
    gtk_chart_set_y_max(m_chart, current_max_y);
    gtk_chart_set_width(m_chart, 800);
}

gboolean GraphWindow::plot_point_callback(gpointer user_data) {
    Point* point = static_cast<Point*>(user_data);
    // std::cerr << "Plotting: (" << point->x << ", " << point->y << ")\n";
    gtk_chart_plot_point(point->chart, point->x, point->y);
    delete point;
    return G_SOURCE_REMOVE;
}

// void GraphWindow::update_thread_function() {
//     while (m_running) {
//         int distance, time;
//         if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
//             g_mutex_lock(&m_mutex);
//             std::cerr << "Popped: (" << time << ", " << distance << ")\n";
//
//             double x = static_cast<double>(time);
//             double y = static_cast<double>(distance);
//
//             // Store new point
//             m_data_points.emplace_back(x, y);
//
//             // Get the time window from settings
//             double time_window = static_cast<double>(
//                 SettingsManager::getInstance().getGraphTimeWindow());
//
//             // Remove old points
//             double cutoff_time = x - time_window;
//             while (!m_data_points.empty() && m_data_points.front().first < cutoff_time) {
//                 m_data_points.pop_front();
//             }
//
//             // Update axis ranges
//             current_max_x = x;
//             if (y > current_max_y) {
//                 current_max_y = y * 1.2;
//             }
//
//             // Clear and replot only every N points or when removing old points
//             static int update_counter = 0;
//             if (update_counter++ % 10 == 0) {  // Update every 10 points
//
//                 // Plot all remaining points
//                 for (const auto& point : m_data_points) {
//                     Point* new_point = new Point{point.first, point.second, m_chart};
//                     g_idle_add(plot_point_callback, new_point);
//                 }
//             } else {
//                 // Just plot the new point
//                 Point* new_point = new Point{x, y, m_chart};
//                 g_idle_add(plot_point_callback, new_point);
//             }
//
//             g_mutex_unlock(&m_mutex);
//         }
//
//         std::this_thread::sleep_for(std::chrono::milliseconds(2));
//     }
// }

//TODO: It pops data directly from ThreadSafeQueue. It's good but also it makes it difficult to trim. Could move vector here to be used as temporary storage but need to figure out how to trim old data points on the run and not kill the cpu.

void GraphWindow::update_thread_function() {
    while (m_running) {
        int distance, time;
        if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
            g_mutex_lock(&m_mutex);

            double x = static_cast<double>(time);
            double y = static_cast<double>(distance);

            // Remove old points
            remove_old_points(x);

            // Store new point
            m_data_points.emplace_back(x, y);

            // Update axis ranges
            double time_window = static_cast<double>(
                SettingsManager::getInstance().getGraphTimeWindow());

            // Set x-axis range to show only the time window
            gtk_chart_set_x_max(m_chart, x);
            if (x > time_window) {
                gtk_chart_set_x_max(m_chart, x);
            }

            // Update y-axis if needed
            if (y > current_max_y) {
                current_max_y = y * 1.2;
                gtk_chart_set_y_max(m_chart, current_max_y);
            }

            // Plot the point
            Point* point = new Point{x, y, m_chart};
            g_idle_add(plot_point_callback, point);

            g_mutex_unlock(&m_mutex);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

//FIXME: Actually use remove_old_points and make it usefull to redraw the entire chart with only most recent eg. 100 ponits.
void GraphWindow::remove_old_points(double current_time) {
    double time_window = static_cast<double>(
        SettingsManager::getInstance().getGraphTimeWindow());
    double cutoff_time = current_time - time_window;

    // Remove points older than the time window
    while (!m_data_points.empty() && m_data_points.front().first < cutoff_time) {
        m_data_points.pop_front();
        // std::cerr << "Removed old point NOT" << std::endl;
    }
}

void GraphWindow::on_close_clicked() {
    hide();
}