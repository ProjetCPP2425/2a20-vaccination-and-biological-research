#include "arduino.h"
#include <QDebug>
#include <QSerialPort>

arduino::arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;
}

QString arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort* Arduino::getserial()
{
    return serial;
}

int arduino::connect_arduino()
{
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) { // Correction ici
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                qDebug() << "Arduino port name is:" << arduino_port_name;
                serial->setPortName(arduino_port_name);
                if (serial->open(QSerialPort::ReadWrite)) {
                    serial->setBaudRate(QSerialPort::Baud9600);
                    serial->setDataBits(QSerialPort::Data8);
                    serial->setParity(QSerialPort::NoParity);
                    serial->setStopBits(QSerialPort::OneStop);
                    serial->setFlowControl(QSerialPort::NoFlowControl);
                    return 0; // Success
                } else {
                    qDebug() << "Failed to open serial port for Arduino.";
                    return 1; // Failed to open serial port
                }
            }
        }
    }
    qDebug() << "Arduino not found.";
    return -1; // Arduino not found
}

int arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        return 0; // Success
    }
    return 1; // Failed to close
}

QByteArray arduino::read_from_arduino()
{
    if (serial->isReadable()) {
        data = serial->readAll();
        return data;
    }
    return QByteArray(); // Return empty QByteArray if not readable
}

void arduino::write_to_arduino(QByteArray d)
{
    if (serial->isWritable()) {
        serial->write(d);
    } else {
        qDebug() << "Couldn't write to serial!";
    }
}
