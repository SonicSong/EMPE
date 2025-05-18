#ifndef LICENSES_H
#define LICENSES_H

#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>

class Licenses : public Gtk::Window {
    public:
        Licenses();
        ~Licenses() override;

    private:
        void on_button_gtkmm();

        Gtk::Box m_box;
        Gtk::Button m_gtkmm_lic;
};

#endif //LICENSES_H
