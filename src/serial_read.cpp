#include <iostream>
#include <string>
#include "serial_read.h"



#ifdef _WIN32
  std::string serial_port_type = "com0";
#endif

#ifdef __linux__
  std::string serial_port_type = "/dev/ttyUSB0";
#endif

// #ifdef

std::string serial_read() {
  //macOS tty.usbserial-0001
  //Linux ttyUSB0
  //macOS BT cu.EMPESensor
  //macOS BT tty.EMPESensor


  // serial_port_1.open();



  std::string serial_read_result;

  // for (int i = 0; i < 14; i++){
  // while (true) {
  // serial_read_result = std::string(serial_port_1.read());
  // std::cout << serial_read_result;
  // }

  return serial_read_result;
}