#include "window.h"

MainWindow::MainWindow() : m_button("Start") {
    m_button.set_margin(10);

    m_button.signal_clicked().connect(sigc::mem_fun(*this,
                &MainWindow::on_button_clicked));

    set_child(m_button);
}


MainWindow::~MainWindow() {

}

void MainWindow::on_button_clicked() {
    std::cout << "Hello World" << std::endl;
}

void MainWindow::create_graph() {
    std::cout << "NIE MA GRAFIKI" << std::endl;
}
