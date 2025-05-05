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
    int connect_arduino1();
    void write_to_arduino1(const QByteArray &data);
    QString getPortName() const;
    QSerialPort* getSerial1();
    void sendCommand(const QString &command);
    QString getPortName();
public slots:
              // void displayTemperature(float temp);

signals:
    void rfidUIDReceived(const QString &uid);
    void temperatureReceived(float temperature);
private slots:
    void read_from_arduino();

private:
    QSerialPort *serial;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data_buffer;
};

#endif // ARDUINOMANAGER_H
