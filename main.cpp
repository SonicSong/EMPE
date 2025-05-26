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

#ifdef _WIN32
BOOL WINAPI windows_ctrl_handler(DWORD ctrl_type) {
    if (ctrl_type == CTRL_C_EVENT || ctrl_type == CTRL_BREAK_EVENT) {
        stop_reading(SIGINT);
        return TRUE;
    }
    return FALSE;
}
#endif

int main(int argc, char **argv) {
    // connection_init();
    // auto& settings = SettingsManager::getInstance();
    // std::string current_port = settings.getPort();
    // int current_baud = settings.getBaudRate();

    Gtk::Application::create();

    auto app = Gtk::Application::create("org.empe.application");
    return app->make_window_and_run<MainWindow>(argc, argv);  // Added argc, argv parameters
}