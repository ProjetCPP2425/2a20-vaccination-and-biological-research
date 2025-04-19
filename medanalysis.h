#ifndef MEDANALYSIS_H
#define MEDANALYSIS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class MedAnalysis : public QObject
{
    Q_OBJECT

public:
    explicit MedAnalysis(QObject *parent = nullptr);
    void fetchCovidData(const QString &isoCode); // Ex: "FRA", "ITA", etc.

signals:
    void dataFetched(QString country, int confirmed, int deaths, int recovered, int active);
    void errorOccurred(QString error);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
};

#endif // MEDANALYSIS_H


