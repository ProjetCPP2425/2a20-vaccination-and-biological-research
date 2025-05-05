#include "arduinomanager.h"

/*ArduinoManager::ArduinoManager(QObject *parent)
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

        qDebug() << "📥 Ligne reçue Arduino:" << strLine;

        if (strLine.startsWith("UID:")) {   // <- pas "UID de la carte :" attention
            QString uid = strLine.section(':', 1).trimmed();
            emit rfidUIDReceived(uid);
        }
        else if (strLine.startsWith("TEMP:")) {
            QString tempString = strLine.section(':', 1).trimmed();
            bool ok;
            float temp = tempString.toFloat(&ok);
            if (ok) {
                emit temperatureReceived(temp);
            } else {
                qDebug() << "⚠ Erreur de conversion température:" << tempString;
            }
        }
        else {
            qDebug() << "⚠ Trame inconnue :" << strLine;
        }
    }
}


void ArduinoManager::sendCommand(const QString &command)
{
    if (serial->isOpen() && serial->isWritable()) {
        serial->write(command.toUtf8());
        serial->flush();
    }
}
*/
#include "arduinomanager.h"
#include <QDebug>

ArduinoManager::ArduinoManager(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}

/*int ArduinoManager::connect_arduino1()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.hasVendorIdentifier() && info.hasProductIdentifier()) {
            if (info.vendorIdentifier() == 9025 && info.productIdentifier() == 67) {
                serial->setPort(info);
                if (serial->open(QIODevice::ReadWrite)) {
                    serial->setBaudRate(QSerialPort::Baud9600);
                    connect(serial, &QSerialPort::readyRead, this, &ArduinoManager::read_from_arduino);
                    return 0; // Succès
                } else {
                    return 1; // Port trouvé mais pas ouvert
                }
            }
        }
    }
    return -1; // Arduino non trouvé
}*/
int ArduinoManager::connect_arduino1()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.hasVendorIdentifier() && info.hasProductIdentifier()) {
            if (info.vendorIdentifier() == 9025 && info.productIdentifier() == 67 &&
                info.portName() != "COM5") { // ⛔ NE PAS PRENDRE COM5
                serial->setPort(info);
                serial->setBaudRate(QSerialPort::Baud9600);
                serial->setDataBits(QSerialPort::Data8);
                serial->setParity(QSerialPort::NoParity);
                serial->setStopBits(QSerialPort::OneStop);
                serial->setFlowControl(QSerialPort::NoFlowControl);

                if (serial->open(QIODevice::ReadWrite)) {
                    connect(serial, &QSerialPort::readyRead, this, &ArduinoManager::read_from_arduino);
                    qDebug() << "✅ Arduino Manager connecté sur :" << info.portName();
                    return 0;
                } else {
                    qDebug() << "❌ Erreur ouverture :" << serial->errorString();
                    return 1;
                }
            }
        }
    }

    qDebug() << "❌ Aucun port disponible pour Arduino Manager (hors COM5).";
    return -1;
}


QString ArduinoManager::getPortName()
{
    return serial->portName();
}

void ArduinoManager::read_from_arduino()
{
    while (serial->canReadLine()) {
        QByteArray line = serial->readLine().trimmed();
        QString strLine = QString::fromUtf8(line);
        qDebug() << "📥 Ligne reçue Arduino:" << strLine;

        if (strLine.startsWith("UID:")) {
            QString uid = strLine.section(':', 1).trimmed();
            emit rfidUIDReceived(uid);
        }
        else if (strLine.startsWith("TEMP:")) {
            QString tempStr = strLine.section(':', 1).trimmed();
            bool ok;
            float temp = tempStr.toFloat(&ok);
            if (ok) {
                emit temperatureReceived(temp);
            }
        }
    }
}

void ArduinoManager::sendCommand(const QString &command)
{
    if (serial->isOpen() && serial->isWritable()) {
        serial->write(command.toUtf8());
        serial->flush();
    }
}
