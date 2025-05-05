#include <iostream>
#include <gtkmm.h>
#include <gtk/gtk.h>
#include "src/serial_read.h"
#include "src/serial_search.h"
#include "src/window.h"

int main(int argc, char **argv) {
    std::vector<DeviceInfo> devices = search_ports("/dev/");

    if (!devices.empty()) {
        std::string selectedPort = select_port(devices);

        if (!selectedPort.empty()) {
            std::cout << selectedPort << std::endl;
            serial_read(selectedPort);
        }
    } else {
        std::cerr << "No ports found" << std::endl;
        return 1;
    }

    return 0;
}


// auto app = Gtk::Application::create("EMPE");
// return app->make_window_and_run<MainWindow>(argc, argv);