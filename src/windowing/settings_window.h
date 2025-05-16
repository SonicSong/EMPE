#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>

class SettingsWindow : public Gtk::Window {
public:
    SettingsWindow();
    ~SettingsWindow() override;

private:
    void on_apply_clicked();
    void on_cancel_clicked();

    Gtk::Box m_box;
    Gtk::Button m_apply_button;
    Gtk::Button m_cancel_button;
};

#endif // SETTINGS_WINDOW_H