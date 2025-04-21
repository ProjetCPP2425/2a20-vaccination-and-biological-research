#ifndef ARDUINORFID_H
#define ARDUINORFID_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class ArduinoRFID
{
public:
    ArduinoRFID();
    int connect_arduino();
    int close_arduino();
    void write_to_arduino(QByteArray);
    QByteArray read_from_arduino();
    QSerialPort* getserial();
    QString getarduino_port_name();

private:
    QSerialPort *serial;
    static const quint16 arduino_rfid_vendor_id = 9025;
    static const quint16 arduino_rfid_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
};

#endif // ARDUINORFID_H
