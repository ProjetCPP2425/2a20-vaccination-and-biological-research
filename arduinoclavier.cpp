/*
#include "arduinoclavier.h"

arduinoCLAVIER::arduinoCLAVIER() {}
*/



#include "arduinoclavier.h"

#include <QDebug>


ArduinoClavier::ArduinoClavier()
{
    serial = new QSerialPort;
    arduino_port_name = "";
    arduino_is_available = false;

    qDebug() << "🔍 Ports disponibles :";
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts()) {
        qDebug() << " - " << port.portName() << " | " << port.description();
    }


    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts()) {
        if (port.hasVendorIdentifier() && port.hasProductIdentifier()) {
            if (port.vendorIdentifier() == arduino_uno_vendor_id &&
                port.productIdentifier() == arduino_uno_product_id) {
                arduino_port_name = port.portName();
                arduino_is_available = true;
            }
        }
    }

    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);

        if (serial->open(QIODevice::ReadOnly)) {
            qDebug() << "✅ Port série ouvert :" << serial->portName();
        } else {
            qDebug() << "❌ Impossible d’ouvrir le port série :" << serial->portName();
        }


        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
    }
}

int ArduinoClavier::connect_arduino()
{
    if (arduino_is_available) {
        return 0; // connecté
    } else {
        return 1; // pas trouvé
    }
}

QString ArduinoClavier::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort* ArduinoClavier::getserial()
{
    return serial;
}

QByteArray ArduinoClavier::read_from_arduino()
{
    if (serial->isReadable()) {
        return serial->readAll();
    }
    return QByteArray();
}
