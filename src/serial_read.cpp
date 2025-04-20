#include <gio/gio.h>
#include <gio/gunixinputstream.h>
#include <glib.h>
#include <termios.h>

#include "serial_read.h"

#ifdef _WIN32
  std::string serial_port_type = "com0";
#endif

#ifdef __linux__
  std::string serial_port_type = "/dev/ttyUSB0";
#endif

// #ifdef

typedef struct {
  GOutputStream *output_stream;
  // struct configuration_port config;
  struct termios config;
};

void serial_read() {
  //macOS tty.usbserial-0001
  //Linux ttyUSB0
  //macOS BT cu.EMPESensor
  //macOS BT tty.EMPESensor


  // serial_port_1.open();


  // serial_1->setBaudRate(115200);
  // serial_1.setDataBits(QSerialPort::Data8);
  // serial_1.setParity(QSerialPort::NoParity);
  // serial_1.setStopBits(QSerialPort::OneStop);
  // serial_1.setFlowControl(QSerialPort::NoFlowControl);
  std::cout << "serial_read" << std::endl;
}
