#ifndef LICENSES_H
#define LICENSES_H

#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/grid.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <fstream>
#include <iostream>

class Licenses : public Gtk::Window {
    public:
        Licenses();
        ~Licenses() override;

    private:
        void on_button_gtkmm();
        void on_button_re2();
        void on_button_serial_cpp();
        void on_button_gtkchart();
        void on_button_empe();
        void on_cancel_button();
        void display_license(const std::string& license_name);
        void update_info_label(const std::string& library_name, const std::string& repo_link);

        Gtk::Grid m_grid;
        Gtk::Box m_box;
        Gtk::Button m_gtkmm_lic;
        Gtk::Button m_re2_lic;
        Gtk::Button m_serial_cpp_lic;
        Gtk::Button m_gtkchart_lic;
        Gtk::Button m_empe_lic;
        Gtk::ScrolledWindow m_scrolled_window;
        Gtk::TextView m_text_view;
        Gtk::Button m_cancel_button;
        Gtk::Label m_info_label;  // Label to show library name and repo link
};

#endif //LICENSES_H
