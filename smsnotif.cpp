#include "SmsNotif.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

SmsNotif::SmsNotif(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

bool SmsNotif::sendSMS(const QString &to, const QString &message)
{
    // URL de l'API Twilio
    QString url = "https://api.twilio.com/2010-04-01/Accounts/" + ACCOUNT_SID + "/Messages.json";
    QNetworkRequest request(url);

    // 🔒 Authentification de Twilio (Basic Auth)
    QString auth = ACCOUNT_SID + ":" + AUTH_TOKEN;
    QByteArray authHeader = "Basic " + auth.toLocal8Bit().toBase64();
    request.setRawHeader("Authorization", authHeader);

    // 📦 Corps de la requête
    QUrlQuery params;
    params.addQueryItem("To", to);              // Numéro du destinataire
    params.addQueryItem("From", TWILIO_PHONE);  // Numéro Twilio
    params.addQueryItem("Body", message);       // Contenu du SMS

    // 🚀 Envoi de la requête POST
    QNetworkReply *reply = networkManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());

    // 🔎 Affichage de la réponse pour Debugging
    QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray responseData = reply->readAll();
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "✅ SMS accepté par Twilio.";
            qDebug() << "📜 Code HTTP :" << statusCode;
            qDebug() << "📄 Réponse : " << QString(responseData);
        } else {
            qDebug() << "❌ Erreur d'envoi du SMS :" << reply->errorString();
            qDebug() << "📜 Code HTTP :" << statusCode;
            qDebug() << "📄 Réponse complète : " << QString(responseData);
        }

        reply->deleteLater();
    });

    return true; // Indique que la requête a été envoyée
}
