#include "window.h"

MainWindow::MainWindow() 
    : m_box(Gtk::Orientation::VERTICAL),
      m_button("Start"),
      m_settings_button("Port settings"),
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
    //TODO: Calling this function will hang up the entire process as it's infinite loop it won't exit.
    connection_init();
    std::cerr << "Not yet" << std::endl;
}

void MainWindow::on_settings_button_clicked() {
    if (!m_settings_window) {
        m_settings_window = new SettingsWindow();
    }
    m_settings_window->show();
}

void MainWindow::save_data() {
    std::cerr << "NIE MA ZAPISU" << std::endl;
}

void MainWindow::on_about_button_clicked() {
    std::cout << "Nie ma About" << std::endl;
}


void MainWindow::create_graph() {
    std::cout << "NIE MA GRAFIKI" << std::endl;
}

void MainWindow::select_port_button() {
    std::cout << "NIE MA PORTU" << std::endl;
}