#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include "../serial_read.h"
#include "../serial_search.h"
#include <iostream>

class MainWindow : public Gtk::Window {
    public:
        MainWindow();
        ~MainWindow() override;

    private:
        void on_button_clicked();
        void create_graph();
        void select_port_button();

        Gtk::Button m_button;
};

#endif //WINDOW_H