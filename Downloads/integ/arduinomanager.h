// ArduinoManager.h
#ifndef ARDUINOMANAGER_H
#define ARDUINOMANAGER_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QObject>
#include <QByteArray>
#include <QDebug>

class ArduinoManager : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoManager(QObject *parent = nullptr);
    int connect_arduino();
    void write_to_arduino(const QByteArray &data);
    QString getPortName() const;
    QSerialPort* getSerial();

signals:
    void rfidUIDReceived(const QString &uid);

private slots:
    void read_from_arduino();

private:
    QSerialPort *serial;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data_buffer;
};

#endif // ARDUINOMANAGER_H
