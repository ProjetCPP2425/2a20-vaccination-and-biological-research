#include "medanalysis.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>

// 🔹 Constructeur
MedAnalysis::MedAnalysis(QObject *parent) : QObject(parent) {
    networkManager = new QNetworkAccessManager(this);
}

// 🔹 Récupérer les données de COVID-19 d'un pays
void MedAnalysis::fetchCovidData(const QString &country) {
    QString url = "https://disease.sh/v3/covid-19/countries/" + country;
    QNetworkRequest request = QNetworkRequest(QUrl(url));


    // 🔹 Définir un User-Agent pour éviter certains blocages d'API
    request.setHeader(QNetworkRequest::UserAgentHeader, "QtNetwork");

    // 🔹 Envoyer la requête GET via QNetworkAccessManager
    QNetworkReply *reply = networkManager->get(request);

    // 🔹 Connecter la réponse pour traiter les données quand elles arrivent
    connect(reply, &QNetworkReply::finished, this, [=]() { onNetworkReply(reply); });
}

// 🔹 Traitement de la réponse de l'API
void MedAnalysis::onNetworkReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        QString country = jsonObj["country"].toString();
        int cases = jsonObj["cases"].toInt();
        int deaths = jsonObj["deaths"].toInt();
        int recovered = jsonObj["recovered"].toInt();
        int population = jsonObj["population"].toInt();
        double vaccinationRate = jsonObj["vaccinationRate"].toDouble();
        // 🔹 Vérification et affichage des données
        qDebug() << "📊 Données reçues pour" << country;
        qDebug() << "Cas :" << cases << "| Décès :" << deaths << "| Guérisons :" << recovered;
        qDebug() << "Population :" << population;

        // 🔹 Emettre un signal avec les données
        emit dataFetched(country, cases, deaths, recovered, population, vaccinationRate);

    } else {
        // 🔹 Gestion avancée des erreurs
        qDebug() << "❌ Erreur de requête : " << reply->errorString();
        emit errorOccurred(reply->errorString());
    }

    // 🔹 Nettoyage
    reply->deleteLater();
}
