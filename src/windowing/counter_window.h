//
// Created by raven on 7/17/25.
//

#ifndef COUNTER_WINDOW_H
#define COUNTER_WINDOW_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/label.h>

#include <glibmm/main.h>
#include <glibmm/wrap.h>

#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <deque>
#include <functional>

#include "../counter.h"
#include "../ThreadSafeQueue.h"

class counter_window : public Gtk::Window {
public:
    counter_window();
    ~counter_window();

private:
    void on_start_clicked();
    void on_counter_start_clicked();
    void on_threshold_changed();
    void on_window_changed();
    void update_counter_time(const std::string& time_text);


    Gtk::Box m_box;
    Gtk::Button m_start_button;
    Gtk::Grid m_counter_grid;
    Gtk::Button m_close_button;
    Gtk::Label m_threshold_label;   // Label for threshold setting
    Gtk::SpinButton m_threshold_spin;  // Spin button for threshold value
    Gtk::SpinButton m_window_spin;
    Gtk::Button m_counter_start_button;
    // Gtk::Label m_crossings_label;  // New label for threshold crossings
    Gtk::Label m_window_label;
    Gtk::Label m_counter_time_label;  // New label for counter time display


    Counter m_counter;

};

#endif //COUNTER_WINDOW_H
