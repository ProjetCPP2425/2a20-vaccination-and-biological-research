
#include "medanalysis.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QUrl>

MedAnalysis::MedAnalysis(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this))
{
}

void MedAnalysis::fetchCovidData(const QString &isoCode)
{
    QUrl url("https://covid-api.com/api/reports?iso=" + isoCode);
    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        onNetworkReply(reply);
    });
}

void MedAnalysis::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Erreur réseau : " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        emit errorOccurred("Réponse JSON invalide.");
        return;
    }

    QJsonArray dataArray = doc.object()["data"].toArray();
    if (dataArray.isEmpty()) {
        emit errorOccurred("Aucune donnée reçue pour ce pays.");
        return;
    }

    QJsonObject countryData = dataArray.first().toObject();
    QJsonObject region = countryData["region"].toObject();

    QString country = region["name"].toString();
    int confirmed = countryData["confirmed"].toInt();
    int deaths = countryData["deaths"].toInt();
    int recovered = countryData["recovered"].toInt();
    int active = countryData["active"].toInt();

    emit dataFetched(country, confirmed, deaths, recovered, active);
}
