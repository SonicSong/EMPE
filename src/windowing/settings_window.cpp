#include "settings_window.h"
#include <string>

//TODO: Fix killing of the window so that when closing it by kill button it can be reopened and won't cause issues if someone will close it by accident.

SettingsWindow::SettingsWindow()
    : m_grid()
    , m_port_label("Port:")
    , m_baud_label("Baud Rate:")
    , m_interval_label("Update Interval (ms):")
    , m_scan_ports("Scan Ports")
    , m_apply_button("Apply")
    , m_cancel_button("Cancel")
    , m_button_box(Gtk::Orientation::HORIZONTAL, 5)
    // Initialize second port components
    , m_port_label2("Second Port:")
    , m_baud_label2("Second Baud Rate:")
    , m_interval_label2("Second Update Interval (ms):")
    , m_scan_ports2("Scan Ports")
{
    set_title("EMPE Settings");
    set_modal(true);
    set_hide_on_close(true);

    // Configure the grid
    m_grid.set_margin(10);
    m_grid.set_row_spacing(5);
    m_grid.set_column_spacing(5);
    set_child(m_grid);

    // Configure interval spin button
    m_interval_spin.set_range(1, 32);
    m_interval_spin.set_value(4);
    m_interval_spin.set_increments(1, 10);
    m_interval_spin.set_tooltip_text("Update interval in milliseconds");

    // Configure second LiDAR interval spin button
    m_interval_spin2.set_range(1, 32);
    m_interval_spin2.set_value(4);
    m_interval_spin2.set_increments(1, 10);
    m_interval_spin2.set_tooltip_text("Update interval for second LiDAR in milliseconds");

    m_viewport_width_label.set_text("Viewport Width (points):");
    m_viewport_width_spin.set_range(50, 100000);
    m_viewport_width_spin.set_value(SettingsManager::getInstance().getViewportWidth());
    m_viewport_width_spin.set_increments(100, 1000);

    // Configure auto-scroll checkbox
    m_auto_scroll_check.set_label("Auto-scroll viewport");
    m_auto_scroll_check.set_tooltip_text("When enabled, the viewport will automatically scroll to show new points. When disabled, the viewport will stay fixed and new points will be added to the right.");
    m_auto_scroll_check.set_active(SettingsManager::getInstance().getAutoScrollViewport());

    // Add to grid
    m_grid.attach(m_viewport_width_label, 0, 3, 1, 1);
    m_grid.attach(m_viewport_width_spin, 1, 3, 1, 1);

    m_second_port_check.set_label("Enable second LiDAR");
    m_second_port_check.set_tooltip_text("Enable reading and displaying data from a second LiDAR device.");
    m_second_port_check.set_active(SettingsManager::getInstance().getSecondPort());

    // Add auto-scroll checkbox to grid
    m_grid.attach(m_auto_scroll_check, 0, 4, 2, 1);
    m_grid.attach(m_second_port_check, 0, 5, 2, 1);;

    // Add widgets to the grid
    m_grid.attach(m_port_label, 0, 0);
    m_grid.attach(m_port_combo, 1, 0);
    m_grid.attach(m_scan_ports, 2, 0);

    m_grid.attach(m_baud_label, 0, 1);
    m_grid.attach(m_baud_combo, 1, 1);

    m_grid.attach(m_interval_label, 0, 2);
    m_grid.attach(m_interval_spin, 1, 2);

    // Configure button box
    m_button_box.set_halign(Gtk::Align::END);
    m_button_box.append(m_apply_button);
    m_button_box.append(m_cancel_button);
    m_grid.attach(m_button_box, 0, 10, 3, 1); // Move button box lower to make room for second LiDAR controls

    // Add second LiDAR controls (initially hidden/shown based on checkbox)
    m_grid.attach(m_port_label2, 0, 6, 1, 1);
    m_grid.attach(m_port_combo2, 1, 6, 1, 1);
    m_grid.attach(m_scan_ports2, 2, 6, 1, 1);

    m_grid.attach(m_baud_label2, 0, 7, 1, 1);
    m_grid.attach(m_baud_combo2, 1, 7, 1, 1);

    m_grid.attach(m_interval_label2, 0, 8, 1, 1);
    m_grid.attach(m_interval_spin2, 1, 8, 1, 1);

    // // Update visibility based on current setting
    if (SettingsManager::getInstance().getSecondPort()) {
        m_port_label2.show();
        m_port_combo2.show();
        m_scan_ports2.show();
        m_baud_label2.show();
        m_baud_combo2.show();
        m_interval_label2.show();
        m_interval_spin2.show();
    } else {
        m_port_label2.hide();
        m_port_combo2.hide();
        m_scan_ports2.hide();
        m_baud_label2.hide();
        m_baud_combo2.hide();
        m_interval_label2.hide();
        m_interval_spin2.hide();
    }

    // Connect signals
    m_scan_ports.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_click_scan_ports));
    m_scan_ports2.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_click_scan_ports));
    m_apply_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_apply_clicked));
    m_cancel_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_cancel_clicked));
    m_second_port_check.signal_toggled().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_second_port_toggled));

    // Initialize combos
    setup_baud_rates();
    update_port_list();

    // Set current values from settings manager
    auto& settings = SettingsManager::getInstance();
    if (!settings.getPort().empty()) {
        m_port_combo.set_active_id(settings.getPort());
    }
    m_baud_combo.set_active_text(std::to_string(settings.getBaudRate()));
}

void SettingsWindow::setup_baud_rates() {
    for (int rate : SettingsManager::availableBaudRates) {
        m_baud_combo.append(std::to_string(rate));
        m_baud_combo2.append(std::to_string(rate));
    }
    m_baud_combo.set_active_text("115200"); // Default value
    m_baud_combo2.set_active_text("115200"); // Default value for second LiDAR
}

void SettingsWindow::update_port_list() {
    m_port_combo.remove_all();
    m_port_combo2.remove_all();

    std::vector<DeviceInfo> devices = connection_search();

    for (const auto& device : devices) {
        m_port_combo.append(device.path,
            device.path + " (" + device.description + ")");
        m_port_combo2.append(device.path,
            device.path + " (" + device.description + ")");
    }

    if (!devices.empty()) {
        m_port_combo.set_active(0);
        m_port_combo2.set_active(0);
    }
}

void SettingsWindow::on_click_scan_ports() {
    update_port_list();
}

void SettingsWindow::on_apply_clicked() {
    Glib::ustring selected_port = m_port_combo.get_active_id();
    Glib::ustring baud_str = m_baud_combo.get_active_text();
    int interval = m_interval_spin.get_value_as_int();
    bool second_port_enabled = m_second_port_check.get_active();

    SettingsManager::getInstance().setViewportWidth(
        static_cast<int>(m_viewport_width_spin.get_value()));

    if (!selected_port.empty() && !baud_str.empty()) {
        auto& settings = SettingsManager::getInstance();
        settings.setPort(selected_port);
        settings.setBaudRate(std::stoi(baud_str));
        settings.setInterval(interval);
        settings.setAutoScrollViewport(m_auto_scroll_check.get_active());

        // Save second LiDAR settings
        settings.setSecondPort(second_port_enabled);
        if (second_port_enabled == true) {
            Glib::ustring selected_port2 = m_port_combo2.get_active_id();
            Glib::ustring baud_str2 = m_baud_combo2.get_active_text();
            int interval2 = m_interval_spin2.get_value_as_int();

            settings.setPort2(selected_port2);
            settings.setBaudRate2(std::stoi(baud_str2));
            settings.setSecondInterval(interval2);
        }

        hide();
    }
}

void SettingsWindow::on_cancel_clicked() {
    hide();
}

void SettingsWindow::on_second_port_toggled() {
    bool active = m_second_port_check.get_active();
    if (active) {
        m_port_label2.show();
        m_port_combo2.show();
        m_scan_ports2.show();
        m_baud_label2.show();
        m_baud_combo2.show();
        m_interval_label2.show();
        m_interval_spin2.show();
    } else {
        m_port_label2.hide();
        m_port_combo2.hide();
        m_scan_ports2.hide();
        m_baud_label2.hide();
        m_baud_combo2.hide();
        m_interval_label2.hide();
        m_interval_spin2.hide();
    }
}

SettingsWindow::~SettingsWindow() = default;