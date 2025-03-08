#include "medanalysis.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>

// 🔹 Constructeur
MedAnalysis::MedAnalysis(QObject *parent) : QObject(parent) {
    qDebug() << "✅ Initialisation de MedAnalysis...";
    networkManager = new QNetworkAccessManager(this);

    if (!networkManager) {
        qDebug() << "❌ Erreur : networkManager n'a pas pu être créé !";
    }

    connect(networkManager, &QNetworkAccessManager::finished, this, &MedAnalysis::onNetworkReply);
}


// 🔹 Récupérer les données de COVID-19 d'un pays
void MedAnalysis::fetchCovidData(const QString &country) {
    qDebug() << "📡 Envoi de la requête à l'API pour :" << country;

    if (country.isEmpty()) {
        qDebug() << "❌ Erreur : Aucun pays sélectionné.";
        emit errorOccurred("Aucun pays sélectionné.");
        return;
    }

    QString url = "https://disease.sh/v3/covid-19/countries/" + country;
    QNetworkRequest request = QNetworkRequest(QUrl(url));


    // 🔹 Définir un User-Agent pour éviter certains blocages d'API
    request.setHeader(QNetworkRequest::UserAgentHeader, "QtNetwork");

    // 🔹 Envoyer la requête GET via QNetworkAccessManager
    QNetworkReply *reply = networkManager->get(request);
    if (!reply) {
        qDebug() << "❌ ERREUR : L'API n'a pas répondu, annulation de la requête !";
        return;
    }
    // 🔹 Connecter la réponse pour traiter les données quand elles arrivent
    connect(reply, &QNetworkReply::finished, this, [=]() { onNetworkReply(reply); });
}

// 🔹 Traitement de la réponse de l'API
void MedAnalysis::onNetworkReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug() << "✅ Réponse de l'API :" << responseData;  // 🔍 Vérifier la réponse brute

        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        if (jsonObj.isEmpty()) {
            qDebug() << "❌ JSON vide, mais on ignore l'erreur.";
            return;
        }


        QString country = jsonObj["country"].toString();
        int cases = jsonObj["cases"].toInt();
        int deaths = jsonObj["deaths"].toInt();
        int recovered = jsonObj["recovered"].toInt();
        int population = jsonObj["population"].toInt();
        double vaccinationRate = jsonObj["vaccinationRate"].toDouble();

        qDebug() << "📊 Pays :" << country << "| Cas :" << cases << "| Décès :" << deaths
                 << "| Guérisons :" << recovered << "| Population :" << population
                 << "| Vaccination :" << vaccinationRate;

        emit dataFetched(country, cases, deaths, recovered, population, vaccinationRate);
    } else {
        emit errorOccurred(reply->errorString());
    }

    reply->deleteLater();
}

