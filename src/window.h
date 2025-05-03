#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <iostream>

class MainWindow : public Gtk::Window {
    public:
        MainWindow();
        ~MainWindow() override;

    private:
        void on_button_clicked();

        Gtk::Button m_button;
};

#endif //WINDOW_H