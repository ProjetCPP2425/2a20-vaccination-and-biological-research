#ifndef SMSNOTIF_H
#define SMSNOTIF_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

class SmsNotif : public QObject
{
    Q_OBJECT

public:
    explicit SmsNotif(QObject *parent = nullptr);
    bool sendSMS(const QString &phoneNumber, const QString &message);

private:
    QNetworkAccessManager *networkManager;
    const QString ACCOUNT_SID = "AC9c4ee10389c67b5bf54bca9f154e3fdd";  // 🔹 Remplace par ton SID Twilio
    const QString AUTH_TOKEN = "0262f62d4b4657385ec1b88e40ba76fd";    // 🔹 Remplace par ton Auth Token
    const QString TWILIO_PHONE = "+16575345402";   // 🔹 Ton numéro Twilio
};

#endif // SMSNOTIF_H
