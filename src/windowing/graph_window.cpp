#include "graph_window.h"

//TODO: Possible fix for how much data is displayed? Count elements in vector with data and later just trim to leave the most recent eg. 100 points. Don't use seconds as it's unrelaiable. Use temporary vector to store trimmed data. It should work as continously updated vector. Could be CPU heavy.
// I need to think a bit more about it as it's not stupid but it's not easy. Also need to verify how do I get the data to be displayed if I get it directly from vector that stores the data or from the thread connection_init().
GraphWindow::GraphWindow()
    : m_box(Gtk::Orientation::VERTICAL),
      m_chart(nullptr),
      m_chart2(nullptr),
      m_running(true),
      m_save_button("Save LiDAR 1 data to CSV"),
      m_save_button2("Save LiDAR 2 data to CSV"),
      m_close_window("Close") {

    set_title("EMPE Distance-Time Graph");
    set_default_size(800, 800); // Increased height for two charts
    set_hide_on_close(true);

    g_mutex_init(&m_mutex);

    // Create and setup the first chart
    m_chart = GTK_CHART(gtk_chart_new());
    setup_chart();

    // Create and setup the second chart (if second LiDAR is enabled)
    if (SettingsManager::getInstance().getSecondPort() == true) {
        m_chart2 = GTK_CHART(gtk_chart_new());
        setup_second_chart();
    }

    // Create a managed Gtk::Widget from the first chart GtkWidget
    Gtk::Widget* chart_widget = Gtk::manage(Glib::wrap(GTK_WIDGET(m_chart), false));
    if (!chart_widget) {
        std::cerr << "Failed to create first chart widget" << std::endl;
        return;
    }

    // Make sure the widget is visible
    chart_widget->set_visible(true);
    chart_widget->set_expand(true);  // Allow the chart to expand to fill space
    chart_widget->set_size_request(800, 400);

    // Add first chart to box with proper expansion
    m_box.append(*chart_widget);

    // Add second chart if enabled
    if (SettingsManager::getInstance().getSecondPort() == true) {
        Gtk::Widget* chart_widget2 = Gtk::manage(Glib::wrap(GTK_WIDGET(m_chart2), false));

        if (!chart_widget2) {
            std::cerr << "Failed to create second chart widget" << std::endl;
            return;
        }

        if (chart_widget2) {
            chart_widget2->set_visible(true);
            chart_widget2->set_expand(true);
            chart_widget2->set_size_request(800, 400);
            m_box.append(*chart_widget2);
        } else {
            std::cerr << "Failed to create second chart widget" << std::endl;
        }
    }

    // Create close button with proper styling
    m_close_window.set_label("Close");
    m_close_window.get_style_context()->add_class("destructive-action");
    m_close_window.set_halign(Gtk::Align::CENTER);
    m_close_window.set_margin(5);

    // Setup save button
    m_save_button.set_label("Save LiDAR 1 data to CSV");
    m_save_button.set_halign(Gtk::Align::CENTER);
    m_save_button.set_margin(5);
    m_box.append(m_save_button);

    // Setup second save button (if second LiDAR is enabled)
    if (SettingsManager::getInstance().getSecondPort() == true) {
        m_save_button2.set_label("Save LiDAR 2 data to CSV");
        m_save_button2.set_halign(Gtk::Align::CENTER);
        m_save_button2.set_margin(5);
        m_box.append(m_save_button2);
    }

    // Add buttons to box
    m_box.append(m_close_window);

    // Set box as main window content
    set_child(m_box);

    // Connect signals
    m_close_window.signal_clicked().connect(
        sigc::mem_fun(*this, &GraphWindow::on_close_clicked));

    // Use lambda to call on_save_clicked with chartIndex 0 for first LiDAR
    m_save_button.signal_clicked().connect([this]() {
        on_save_clicked(0);
    });

    // Connect second save button signal if second LiDAR is enabled
    if (SettingsManager::getInstance().getSecondPort() == true) {
        // Use lambda to call on_save_clicked with chartIndex 1 for second LiDAR
        m_save_button2.signal_clicked().connect([this]() {
            on_save_clicked(1);
        });
    }

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

    gtk_chart_set_x_min(m_chart, 0.0);
    gtk_chart_set_y_max(m_chart, current_max_y);
    gtk_chart_set_width(m_chart, 800);
}

void GraphWindow::setup_second_chart() {
    gtk_chart_set_type(m_chart2, GTK_CHART_TYPE_LINE);
    gtk_chart_set_title(m_chart2, "Distance-Time Graph (Second LiDAR)");
    gtk_chart_set_label(m_chart2, "Distance Measurement (LiDAR 2)");
    gtk_chart_set_x_label(m_chart2, "Time [s]");
    gtk_chart_set_y_label(m_chart2, "Distance [units]");

    gtk_chart_set_x_min(m_chart2, 0.0);
    gtk_chart_set_y_max(m_chart2, current_max_y);
    gtk_chart_set_width(m_chart2, 800);
}

gboolean GraphWindow::plot_point_callback(gpointer user_data) {
    Point* point = static_cast<Point*>(user_data);
    gtk_chart_plot_point(point->chart, point->x, point->y);
    delete point;
    return G_SOURCE_REMOVE;
}

void GraphWindow::update_thread_function() {
    int time_to_remove_first = global_start_time_one;
    int time_to_remove_second = global_start_time_two;

    // For global_start_time_first and second s_window use simply data from main window and always call it on button click to setup new start time
    int time_to_remove_first_s_window = global_start_time_first_s_window;
    int time_to_remove_second_s_window = global_start_time_second_s_window;

    while (m_running) {
        int distance1, time1;
        int distance2, time2;
        bool updatedFirst = false;
        bool updatedSecond = false;

        // Process data from first LiDAR
        if (m_chart != nullptr && ThreadSafeQueue::getInstance().try_pop_device(distance1, time1, 0)) {
            g_mutex_lock(&m_mutex);

            // std::cout << "1: " << distance2 << " " << time2 << std::endl;
            // std::cout << "I AM ALIVE 1" << std::endl;

            double x1 = static_cast<double>(time1 - time_to_remove_first);
            double y1 = static_cast<double>(distance1);

            m_data_points.emplace_back(x1, y1);

            if (SettingsManager::getInstance().getAutoScrollViewport()) {
                double viewport_width = static_cast<double>(
                    SettingsManager::getInstance().getViewportWidth());
                double x_min = (x1 > viewport_width) ? (x1 - viewport_width) : 0.0;
                gtk_chart_set_x_min(m_chart, x_min);
                gtk_chart_set_x_max(m_chart, x1);
            } else {
                double current_x_max = gtk_chart_get_x_max(m_chart);
                if (x1 > current_x_max) {
                    gtk_chart_set_x_max(m_chart, x1);
                }
            }

            if (SettingsManager::getInstance().getAutoViewportHeight()) {
                if (y1 > current_max_y) {
                    current_max_y = y1 * 1.2;
                    gtk_chart_set_y_max(m_chart, current_max_y);
                }
            } else {
                double fixed_y_max = SettingsManager::getInstance().getViewportHeight();
                gtk_chart_set_y_max(m_chart, fixed_y_max);
            }

            Point* point = new Point{x1, y1, m_chart};
            g_idle_add(plot_point_callback, point);

            g_mutex_unlock(&m_mutex);
            updatedFirst = true;
        }

        // Process data from second LiDAR - check both conditions in the loop
        if (SettingsManager::getInstance().getSecondPort() && m_chart2 != nullptr &&
            ThreadSafeQueue::getInstance().try_pop_device(distance2, time2, 1)) {
            g_mutex_lock(&m_mutex);

            // std::cout << "2: " << distance2 << " " << time2 << std::endl;
            // std::cout << "I AM ALIVE 2" << std::endl;

            double x2 = static_cast<double>(time2 - time_to_remove_second);
            double y2 = static_cast<double>(distance2);

            m_data_points2.emplace_back(x2, y2);

            if (SettingsManager::getInstance().getAutoScrollViewport()) {
                double viewport_width = static_cast<double>(
                    SettingsManager::getInstance().getViewportWidth());
                double x_min = (x2 > viewport_width) ? (x2 - viewport_width) : 0.0;
                gtk_chart_set_x_min(m_chart2, x_min);
                gtk_chart_set_x_max(m_chart2, x2);
            } else {
                double current_x_max = gtk_chart_get_x_max(m_chart2);
                if (x2 > current_x_max) {
                    gtk_chart_set_x_max(m_chart2, x2);
                }
            }

            if (SettingsManager::getInstance().getAutoViewportHeight()) {
                if (y2 > current_max_y) {
                    current_max_y = y2 * 1.2;
                    gtk_chart_set_y_max(m_chart2, current_max_y);  // Fixed: was m_chart, should be m_chart2
                }
            } else {
                double fixed_y_max = SettingsManager::getInstance().getViewportHeight();
                gtk_chart_set_y_max(m_chart2, fixed_y_max);  // Fixed: was m_chart, should be m_chart2
            }

            Point* point2 = new Point{x2, y2, m_chart2};
            g_idle_add(plot_point_callback, point2);

            g_mutex_unlock(&m_mutex);
            updatedSecond = true;
        }

        if (!updatedFirst && !updatedSecond) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }
}


void GraphWindow::on_close_clicked() {
    hide();
}

void GraphWindow::on_save_clicked(int chartIndex) {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream filename;
    filename << "chart-lidar" << (chartIndex + 1) << "-" << std::put_time(&tm, "%Y%m%d-%H%M%S") << ".csv";

    g_mutex_lock(&m_mutex);

    if (chartIndex == 0) {
        // Save first LiDAR chart
        gtk_chart_save_csv(m_chart, filename.str().c_str(), nullptr);
        std::cout << "Saved LiDAR 1 data to: " << filename.str() << std::endl;
    } else if (chartIndex == 1 && m_chart2) {
        // Save second LiDAR chart
        gtk_chart_save_csv(m_chart2, filename.str().c_str(), nullptr);
        std::cout << "Saved LiDAR 2 data to: " << filename.str() << std::endl;
    } else {
        std::cerr << "Error: Invalid chart index or chart not available" << std::endl;
    }

    g_mutex_unlock(&m_mutex);
}

void GraphWindow::on_save_clicked_chart2() {
    // Wrapper method for second chart
    on_save_clicked(1);
}
