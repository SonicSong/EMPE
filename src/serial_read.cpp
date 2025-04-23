#include "serial_read.h"
#include <serial_cpp/serial.h>

void serial_read() {

  serial_cpp::Serial serial_port_1("/dev/tty.usbserial-0001");

  if (serial_port_1.isOpen() == false) {
    std::cerr << "Failed to open serial port." << std::endl;

  } else {
    std::cout << "Serial port opened." << std::endl;
  }

  std::cout << "Baudrate-pre: " << serial_port_1.getBaudrate() << std::endl;
  serial_port_1.setBaudrate(115200);
  std::cout << "Baudrate-post: " << serial_port_1.getBaudrate() << std::endl << std::endl;

  std::cout << "Flowcontrol-pre: " << serial_port_1.getFlowcontrol() << std::endl;
  serial_port_1.setFlowcontrol(serial_cpp::flowcontrol_hardware);
  std::cout << "Flowcontrol-post: " << serial_port_1.getFlowcontrol() << std::endl << std::endl;

  std::cout << "Stopbit-pre: " << serial_port_1.getStopbits() << std::endl;
  serial_port_1.setStopbits(serial_cpp::stopbits_one);
  std::cout << "Stopbit-post: " << serial_port_1.getStopbits() << std::endl << std::endl;

  std::cout << "Parity-pre: " << serial_port_1.getParity() << std::endl;
  serial_port_1.setParity(serial_cpp::parity_none);
  std::cout << "Parity-post: " << serial_port_1.getParity() << std::endl << std::endl;

  std::cout << "Bytesize-pre: " << serial_port_1.getBytesize() << std::endl;
  serial_port_1.setBytesize(serial_cpp::eightbits);
  std::cout << "Bytesize-post: " << serial_port_1.getBytesize() << std::endl << std::endl;

  serial_port_1.setTimeout(serial_cpp::Timeout::max(), 100, 0, 100, 0);

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
