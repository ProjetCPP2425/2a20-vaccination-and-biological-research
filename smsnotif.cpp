#include "smsnotif.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

// Twilio credentials
const QString ACCOUNT_SID = "ACce2a987f2a3feade2e5fb7de382f6893";
const QString AUTH_TOKEN = "d9220379af88550ed11cb92cf3955b1e";
const QString TWILIO_NUMBER = "+19472182043";

SmsNotif::SmsNotif() {}

bool SmsNotif::sendSMS(const QString &to, const QString &message)
{
    QNetworkAccessManager manager;
    QEventLoop loop;

    // URL Twilio
    QString url = QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(ACCOUNT_SID);
    QUrl serviceUrl(url);
    QNetworkRequest request(serviceUrl);

    // Authentification Basic
    QString credentials = ACCOUNT_SID + ":" + AUTH_TOKEN;
    QByteArray base64Credentials = credentials.toUtf8().toBase64();
    request.setRawHeader("Authorization", "Basic " + base64Credentials);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Corps du message
    QUrlQuery params;
    params.addQueryItem("To", to);
    params.addQueryItem("From", TWILIO_NUMBER);
    params.addQueryItem("Body", message);

    // Envoi de la requête POST
    QNetworkReply *reply = manager.post(request, params.query(QUrl::FullyEncoded).toUtf8());
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Vérification de la réponse
    QByteArray response = reply->readAll();
    qDebug() << "🔁 Réponse Twilio :" << response;

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        if (jsonResponse.isObject()) {
            QJsonObject obj = jsonResponse.object();
            QString status = obj["status"].toString();
            return status == "queued" || status == "sent";
        }
    } else {
        qDebug() << "❌ Erreur réseau Twilio :" << reply->errorString();
    }

    reply->deleteLater();
    return false;
}
