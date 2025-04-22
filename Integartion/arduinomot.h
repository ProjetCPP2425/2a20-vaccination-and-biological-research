#ifndef ARDUINOMOT_H
#define ARDUINOMOT_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class Arduinomot
{
public:
    // Méthodes de la classe Arduinomot
    Arduinomot();
    int connect_arduino();                     // Connecter le PC à Arduino
    int close_arduino();                       // Fermer la connexion
    void write_to_arduino(QByteArray);         // Envoyer des données vers Arduino
    QByteArray read_from_arduino();            // Lire des données depuis Arduino
    QSerialPort* getserial();                  // Accesseur de l'objet série
    QString getarduino_port_name();            // Accesseur du nom de port

private:
    QSerialPort* serial;                       // Objet pour la communication série
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_producy_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;                           // Données lues depuis Arduino
};

#endif // ARDUINOMOT_H

