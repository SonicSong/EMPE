#include <iostream>
#include <gtkmm.h>
#include <thread>
#include <atomic>
#include <csignal>

#ifdef _WIN32
#include <windows.h>
#endif

#include "src/windowing/window.h"
#include "src/serial_read.h"
#include "src/serial_search.h"
#include "src/ThreadSafeQueue.h"
#include "src/atomic_vars.h"
#include "src/connection.h"

int main(int argc, char **argv) {

    #ifdef _WIN32
    // Windows-specific initialization if needed
    SetProcessDPIAware(); // Handle DPI awareness
    #endif

    // Gtk::Application::create();


    auto app = Gtk::Application::create("org.empe.application");
    return app->make_window_and_run<MainWindow>(argc, argv);  // Added argc, argv parameters
}