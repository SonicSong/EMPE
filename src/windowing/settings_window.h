#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include "../connection.h"
#include "../settings_manager.h"

class SettingsWindow : public Gtk::Window {
public:
    SettingsWindow();
    ~SettingsWindow() override;

private:
    void on_apply_clicked();
    void on_cancel_clicked();
    void on_click_scan_ports();
    void update_port_list();
    void setup_baud_rates();

    Gtk::Grid m_grid;
    Gtk::Label m_port_label;
    Gtk::Label m_baud_label;
    Gtk::Label m_interval_label;
    Gtk::ComboBoxText m_port_combo;
    Gtk::ComboBoxText m_baud_combo;
    Gtk::SpinButton m_interval_spin;
    Gtk::Button m_scan_ports;
    Gtk::Button m_apply_button;
    Gtk::Button m_cancel_button;
    Gtk::Box m_button_box;
    Gtk::Label m_graph_window_label;
    Gtk::SpinButton m_graph_window_spin;
};

#endif // SETTINGS_WINDOW_H