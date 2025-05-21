#ifndef LICENSES_H
#define LICENSES_H

#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/grid.h>

class Licenses : public Gtk::Window {
    public:
        Licenses();
        ~Licenses() override;

    private:
        void on_button_gtkmm();
        void on_button_re2();
        void on_button_serial_cpp();
        void on_button_gtkchart();
        void on_cancel_buttom();

        Gtk::Grid m_grid;
        Gtk::Box m_box;
        Gtk::Button m_gtkmm_lic;
        Gtk::Button m_re2_lic;
        Gtk::Button m_serial_cpp_lic;
        Gtk::Button m_gtkchart_lic;
        Gtk::Button m_cancel_button;

};

#endif //LICENSES_H
