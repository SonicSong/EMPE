#include "graph_window.h"

//TODO: Possible fix for how much data is displayed? Count elements in vector with data and later just trim to leave the most recent eg. 100 points. Don't use seconds as it's unrelaiable. Use temporary vector to store trimmed data. It should work as continously updated vector. Could be CPU heavy.
// I need to think a bit more about it as it's not stupid but it's not easy. Also need to verify how do I get the data to be displayed if I get it directly from vector that stores the data or from the thread connection_init().
GraphWindow::GraphWindow(const std::vector<std::pair<int, int>>& initial_data)
    : m_box(Gtk::Orientation::VERTICAL),
      m_chart(nullptr),
      m_running(true),
      m_save_button("Save data to csv"),
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

    // Setup save button
    m_save_button.set_label("Save data to CSV");
    m_save_button.set_halign(Gtk::Align::CENTER);
    m_save_button.set_margin(5);

    // Add buttons to box
    m_box.append(m_save_button);
    m_box.append(m_close_window);

    // Set box as main window content
    set_child(m_box);

    // Connect signals
    m_close_window.signal_clicked().connect(
        sigc::mem_fun(*this, &GraphWindow::on_close_clicked));
    m_save_button.signal_clicked().connect(
        sigc::mem_fun(*this, &GraphWindow::on_save_clicked));

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

gboolean GraphWindow::plot_point_callback(gpointer user_data) {
    Point* point = static_cast<Point*>(user_data);
    // std::cerr << "Plotting: (" << point->x << ", " << point->y << ")\n";
    gtk_chart_plot_point(point->chart, point->x, point->y);
    delete point;
    return G_SOURCE_REMOVE;
}

void GraphWindow::update_thread_function() {
    while (m_running) {
        int distance, time;
        if (ThreadSafeQueue::getInstance().try_pop(distance, time)) {
            g_mutex_lock(&m_mutex);

            double x = static_cast<double>(time);
            double y = static_cast<double>(distance);

            // Store new point
            m_data_points.emplace_back(x, y);


            // Set x-axis range to show only the time window (viewport scroll)
            double x_max = x;
            double viewport_width = static_cast<double>(
                SettingsManager::getInstance().getViewportWidth());
            double x_min = (x_max > viewport_width) ? (x_max - viewport_width) : 0.0;
            gtk_chart_set_x_min(m_chart, x_min);
            gtk_chart_set_x_max(m_chart, x_max);

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

void GraphWindow::on_close_clicked() {
    hide();
}

void GraphWindow::on_save_clicked() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream filename;
    filename << "chart-" << std::put_time(&tm, "%Y%m%d-%H%M%S") << ".csv";

    g_mutex_lock(&m_mutex);
    gtk_chart_save_csv(m_chart, filename.str().c_str());
    g_mutex_unlock(&m_mutex);
}
