
/*
#ifndef ARDUINOCLAVIER_H
#define ARDUINOCLAVIER_H

class arduinoCLAVIER
{
public:
    arduinoCLAVIER();
};

#endif // ARDUINOCLAVIER_H


*/


#ifndef ARDUINOCLAVIER_H
#define ARDUINOCLAVIER_H

#include <QSerialPort>
#include <QSerialPortInfo>

class ArduinoClavier {
public:
    ArduinoClavier();
    int connect_arduino(); // Connexion
    QString getarduino_port_name(); // Nom du port
    QSerialPort* getserial(); // Pointeur vers le port
    QByteArray read_from_arduino(); // Lire les données

private:
    QSerialPort *serial;
    QString arduino_port_name;
    bool arduino_is_available;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
};

#endif // ARDUINOCLAVIER_H
