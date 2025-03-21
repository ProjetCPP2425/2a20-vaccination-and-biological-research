/*#ifndef SMSNOTIF_H
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
    const QString ACCOUNT_SID = "";  // 🔹 Remplace par ton SID Twilio
    const QString AUTH_TOKEN = "";    // 🔹 Remplace par ton Auth Token
    const QString TWILIO_PHONE = "+";   // 🔹 Ton numéro Twilio
};

#endif // SMSNOTIF_H*/
