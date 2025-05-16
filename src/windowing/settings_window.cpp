// settings_window.cpp
#include "settings_window.h"

SettingsWindow::SettingsWindow() 
    : m_box(Gtk::Orientation::VERTICAL),
      m_apply_button("Apply"),
      m_cancel_button("Cancel") {
    
    set_title("Settings");
    set_default_size(300, 200);

    m_box.set_margin(10);
    m_box.set_spacing(10);

    m_apply_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_apply_clicked));
    m_cancel_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingsWindow::on_cancel_clicked));

    m_box.append(m_apply_button);
    m_box.append(m_cancel_button);

    set_child(m_box);
}

SettingsWindow::~SettingsWindow() {
}

void SettingsWindow::on_apply_clicked() {
    hide();
}

void SettingsWindow::on_cancel_clicked() {
    hide();
}