#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/checkbutton.h>
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
    void on_second_port_toggled();

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
    Gtk::Box m_settings_box;
    Gtk::Box m_settings_box2;
    Gtk::Box m_button_box;
    Gtk::Label m_viewport_width_label;
    Gtk::SpinButton m_viewport_width_spin;
    Gtk::CheckButton m_auto_scroll_check;

    // UI Elements related to the second port/LIDAR device
    Gtk::CheckButton m_second_port_check;
    Gtk::Label m_port_label2;
    Gtk::Label m_baud_label2;
    Gtk::Label m_interval_label2;
    Gtk::ComboBoxText m_port_combo2;
    Gtk::ComboBoxText m_baud_combo2;
    Gtk::SpinButton m_interval_spin2;
    Gtk::Button m_scan_ports2;
};

#endif // SETTINGS_WINDOW_H