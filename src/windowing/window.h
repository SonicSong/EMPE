#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include "../serial_read.h"
#include "../serial_search.h"
#include "settings_window.h"
#include <iostream>

class MainWindow : public Gtk::Window {
    public:
        MainWindow();
        ~MainWindow() override;

    private:
        void on_button_clicked();
        void on_settings_button_clicked();
        void create_graph();
        void select_port_button();
        void save_data();
        void on_about_button_clicked();

        Gtk::Box m_box;
        Gtk::Button m_button;
        Gtk::Button m_settings_button;
        Gtk::Button m_graph_button;
        Gtk::Button m_save_data;
        Gtk::Button m_about_button;
        SettingsWindow* m_settings_window;
};

#endif //WINDOW_H