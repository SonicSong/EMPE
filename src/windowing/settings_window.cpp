#include "settings_window.h"
#include <string>

SettingsWindow::SettingsWindow()
    : m_grid()
    , m_port_label("Port:")
    , m_baud_label("Baud Rate:")
    , m_scan_ports("Scan Ports")
    , m_apply_button("Apply")
    , m_cancel_button("Cancel")
    , m_button_box(Gtk::Orientation::HORIZONTAL, 5)
{
    set_title("EMPE Settings");
    set_modal(true);

    // Configure the grid
    m_grid.set_margin(10);
    m_grid.set_row_spacing(5);
    m_grid.set_column_spacing(5);
    set_child(m_grid);

    // Add widgets to the grid
    m_grid.attach(m_port_label, 0, 0);
    m_grid.attach(m_port_combo, 1, 0);
    m_grid.attach(m_scan_ports, 2, 0);
    
    m_grid.attach(m_baud_label, 0, 1);
    m_grid.attach(m_baud_combo, 1, 1);

    // Configure button box
    m_button_box.set_halign(Gtk::Align::END);
    m_button_box.append(m_apply_button);
    m_button_box.append(m_cancel_button);
    m_grid.attach(m_button_box, 0, 2, 3, 1);

    // Connect signals
    m_scan_ports.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_click_scan_ports));
    m_apply_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_apply_clicked));
    m_cancel_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_cancel_clicked));

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
    }
    m_baud_combo.set_active_text("9600"); // Default value
}

void SettingsWindow::update_port_list() {
    m_port_combo.remove_all();
    
    std::vector<DeviceInfo> devices = connection_search();
    
    for (const auto& device : devices) {
        m_port_combo.append(device.path, 
            device.path + " (" + device.description + ")");
    }
    
    if (!devices.empty()) {
        m_port_combo.set_active(0);
    }
}

void SettingsWindow::on_click_scan_ports() {
    update_port_list();
}

void SettingsWindow::on_apply_clicked() {
    Glib::ustring selected_port = m_port_combo.get_active_id();
    Glib::ustring baud_str = m_baud_combo.get_active_text();
    
    if (!selected_port.empty() && !baud_str.empty()) {
        auto& settings = SettingsManager::getInstance();
        settings.setPort(selected_port);
        settings.setBaudRate(std::stoi(baud_str));
        hide();
    }
}

void SettingsWindow::on_cancel_clicked() {
    hide();
}

SettingsWindow::~SettingsWindow() = default;