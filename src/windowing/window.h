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

        Gtk::Box m_box;
        Gtk::Button m_button;
        Gtk::Box m_button_box;  // For horizontal button layout
        Gtk::Box m_label_box;   // For horizontal label layout
        Gtk::Button m_save_button;
        Gtk::Button m_settings_button;
        Gtk::Button m_about_button;
        Gtk::Button m_graph_button;
        GraphWindow* m_graph_window;
        Gtk::Label m_distance_label;
        Gtk::Label m_time_label;
        SettingsWindow* m_settings_window;
        Licenses* m_licenses_window;

        std::thread connection_thread;
        bool is_running;
        std::vector<std::pair<int, int>> data_points; // pairs of (distance, time)
};

#endif //WINDOW_H
