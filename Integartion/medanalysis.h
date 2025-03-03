#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QUrl>


class MedAnalysis : public QObject {
    Q_OBJECT

public:
    explicit MedAnalysis(QObject *parent = nullptr);
    void fetchCovidData(const QString &country);

signals:
    void dataFetched(QString country, int cases, int deaths, int recovered, int population, double vaccinationRate);
    void errorOccurred(QString error);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;

};

