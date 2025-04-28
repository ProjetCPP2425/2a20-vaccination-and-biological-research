#include "ArduinoManager.h"

ArduinoManager::ArduinoManager(QObject *parent)
    : QObject(parent), serial(new QSerialPort), arduino_is_available(false)
{
}

int ArduinoManager::connect_arduino()
{
    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts()) {
        if (portInfo.hasVendorIdentifier() && portInfo.hasProductIdentifier()) {
            if (portInfo.vendorIdentifier() == 9025 && portInfo.productIdentifier() == 67) {
                arduino_port_name = portInfo.portName();
                arduino_is_available = true;
                break;
            }
        }
    }

    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        if (serial->open(QIODevice::ReadWrite)) {
            QObject::connect(serial, &QSerialPort::readyRead, this, &ArduinoManager::read_from_arduino);
            return 0; // succès
        } else {
            return 1; // port trouvé mais pas ouvert
        }
    }
    return -1; // Arduino introuvable
}

void ArduinoManager::write_to_arduino(const QByteArray &data)
{
    if (serial->isOpen() && serial->isWritable()) {
        serial->write(data);
    }
}

QString ArduinoManager::getPortName() const
{
    return arduino_port_name;
}

QSerialPort* ArduinoManager::getSerial()
{
    return serial;
}

void ArduinoManager::read_from_arduino()
{
    while (serial->canReadLine()) {
        QByteArray line = serial->readLine().trimmed();
        QString strLine = QString::fromUtf8(line);
        if (strLine.startsWith("UID:")) {
            QString uid = strLine.section(':', 1).trimmed();
            emit rfidUIDReceived(uid);
        }
    }
}
