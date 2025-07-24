#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/label.h>
#include <iostream>
#include <thread>
#include <vector>
#include <utility>
#include <fstream>


#include "../serial_read.h"
#include "../serial_search.h"
#include "../connection.h"
#include "../settings_manager.h"
#include "settings_window.h"
#include "graph_window.h"
#include "licenses.h"
#include "../counter.h"

class MainWindow : public Gtk::Window {
    public:
        MainWindow();
        ~MainWindow() override;

    protected:
        // Add this method to update labels from the main thread
        bool update_labels();

    private:
        void on_button_clicked();
        void on_settings_button_clicked();
        void create_graph();
        void on_about_button_clicked();
        void on_graph_window_hide();
        void on_licenses_window_hide();
        void on_counter_start_clicked();
        void on_threshold_changed();
        void on_window_changed();
        void update_counter_time(const std::string& time_text);
        void update_second_lidar_visibility(); // Add this method

        Gtk::Box m_box;
        Gtk::Button m_button;
        Gtk::Box m_button_box;  // For horizontal button layout
        Gtk::Box m_label_box;   // For horizontal label layout
        Gtk::Box m_label_box2;
        Gtk::Box m_threshold_box;  // New box for threshold settings
        Gtk::Box m_counter_box;

        Gtk::Button m_save_button;
        Gtk::Button m_settings_button;
        Gtk::Button m_about_button;
        Gtk::Button m_graph_button;
        Gtk::Button m_counter_start_button;
        GraphWindow* m_graph_window;
        Gtk::Label m_distance_label;
        Gtk::Label m_distance_label2;
        Gtk::Label m_time_label;
        Gtk::Label m_time_label2;
        // Gtk::Label m_crossings_label;  // New label for threshold crossings
        Gtk::Label m_threshold_label;   // Label for threshold setting
        Gtk::SpinButton m_threshold_spin;  // Spin button for threshold value
        Gtk::SpinButton m_window_spin;
        Gtk::Label m_window_label;
        Gtk::Label m_counter_time_label;  // New label for counter time display

        SettingsWindow* m_settings_window;
        Licenses* m_licenses_window;

        std::thread connection_thread;
        bool is_running;
        std::vector<std::pair<int, int>> data_points; // pairs of (distance, time)
        std::vector<std::pair<int, int>> data_points2; // pairs of (distance, time)
        Counter m_counter;
};

#endif //WINDOW_H
