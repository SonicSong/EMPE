#include "window.h"

MainWindow::MainWindow() 
    : m_box(Gtk::Orientation::VERTICAL),
      m_button("Start"),
      m_settings_button("Settings"),
      m_settings_window(nullptr) {
    
    m_box.set_margin(10);
    m_box.set_spacing(10);

    m_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_button_clicked));
    m_settings_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_settings_button_clicked));

    m_box.append(m_button);
    m_box.append(m_settings_button);

    set_child(m_box);
}

MainWindow::~MainWindow() {
    delete m_settings_window;
}

void MainWindow::on_button_clicked() {
    std::cout << "Hello World" << std::endl;
}

void MainWindow::on_settings_button_clicked() {
    if (!m_settings_window) {
        m_settings_window = new SettingsWindow();
    }
    m_settings_window->show();
}

void MainWindow::create_graph() {
    std::cout << "NIE MA GRAFIKI" << std::endl;
}

void MainWindow::select_port_button() {
    std::cout << "NIE MA PORTU" << std::endl;
}