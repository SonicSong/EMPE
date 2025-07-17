//
// Created by raven on 7/17/25.
//

#include "counter_window.h"

counter_window::counter_window()
    : m_box(Gtk::Orientation::VERTICAL),
      m_counter_box(Gtk::Orientation::HORIZONTAL),
      m_threshold_label("Threshold:"),
      m_window_label("Time Window (s):"),
      m_counter_start_button("Start Counter"),
      m_counter_time_label(""),  // Initialize the counter time label
      m_threshold_spin(),
      m_window_spin() {

    set_title("Counter Window");
    set_default_size(400, 200);
    set_modal(true);
    set_hide_on_close(true);

    // Configure spin buttons
    m_threshold_spin.set_range(0, 1000);
    m_threshold_spin.set_increments(1, 10);
    m_threshold_spin.set_value(100);

    m_window_spin.set_range(1, 3600);
    m_window_spin.set_increments(1, 10);
    m_window_spin.set_value(60);

    // Connect signals
    m_counter_start_button.signal_clicked().connect(
        sigc::mem_fun(*this, &counter_window::on_counter_start_clicked));
    m_threshold_spin.signal_value_changed().connect(
        sigc::mem_fun(*this, &counter_window::on_threshold_changed));
    m_window_spin.signal_value_changed().connect(
        sigc::mem_fun(*this, &counter_window::on_window_changed));

    // Set the callback for counter time updates
    m_counter.setRemainingTimeCallback(
        sigc::mem_fun(*this, &counter_window::update_counter_time));

    // Add counter elements to counter box
    m_counter_box.append(m_threshold_label);
    m_counter_box.append(m_threshold_spin);
    m_counter_box.append(m_window_label);
    m_counter_box.append(m_window_spin);
    m_counter_box.append(m_counter_start_button);
    // Add the counter time label to the UI
    m_box.append(m_counter_time_label);

    // Add counter box to main box
    m_box.append(m_counter_box);

    set_child(m_box);
}

counter_window::~counter_window() = default;


void counter_window::update_counter_time(const std::string& time_text) {
    // Use Glib::signal_idle to safely update the UI from a non-main thread
    auto update_func = [this, time_text]() {
        m_counter_time_label.set_text(time_text);
        return false; // Return false to run only once
    };

    Glib::signal_idle().connect(update_func);
}

void counter_window::on_counter_start_clicked() {
    if (m_counter_start_button.get_label() == "Start Counter") {
        m_counter.setCounterThreshold(m_threshold_spin.get_value());
        m_counter.setTimeWindow(std::chrono::seconds(static_cast<int>(m_window_spin.get_value())));
        m_counter.startCounter();
        m_counter_start_button.set_label("Stop Counter");
    } else {
        m_counter.stopCounter();
        m_counter_start_button.set_label("Start Counter");
        // Clear the counter time label when stopping the counter
        m_counter_time_label.set_text("");
    }
}

void counter_window::on_threshold_changed() {
    m_counter.setCounterThreshold(m_threshold_spin.get_value());
}

void counter_window::on_window_changed() {
    m_counter.setTimeWindow(std::chrono::seconds(static_cast<int>(m_window_spin.get_value())));
}
