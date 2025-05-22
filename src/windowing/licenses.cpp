#include "licenses.h"

Licenses::Licenses()
    : m_grid()
    , m_gtkmm_lic("Gtkmm license")
    , m_cancel_button("Close")
    , m_re2_lic("RE2 license")
    , m_serial_cpp_lic("serial_cpp license")
    , m_gtkchart_lic("GtkChart license")
{
    set_title("Licenses");
    set_default_size(400, 300);

    m_grid.set_margin(10);
    m_grid.set_row_spacing(5);
    m_grid.set_column_spacing(5);
    set_child(m_grid);

    m_box.set_halign(Gtk::Align::END);
    m_box.append(m_cancel_button);
    m_grid.attach(m_box, 0, 4, 3, 1);

    m_cancel_button.signal_clicked().connect(
        sigc::mem_fun(*this, &Licenses::on_cancel_buttom));
}

Licenses::~Licenses() = default;

void Licenses::on_button_gtkmm() {

}

void Licenses::on_button_gtkchart() {

}

void Licenses::on_button_re2() {

}

void Licenses::on_button_serial_cpp() {

}

void Licenses::on_cancel_buttom() {
    hide();
}

License_Window_Popup::License_Window_Popup() :
    m_cancel_button("Close") {

}

License_Window_Popup::~License_Window_Popup() = default;

void License_Window_Popup::on_cancel_button() {
    hide();
}

