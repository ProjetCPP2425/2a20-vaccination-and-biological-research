#ifndef VACCIN_H
#define VACCIN_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QCategoryAxis>
#include <QStandardItemModel>
class Vaccin
{
private:
    int id_vaccination;
    QString nom;
    QString type;
    QDate date_creation;
    QDate date_expiration;
    QString statut;
    QString certification_vaccin;

public:
    // Constructeurs
    Vaccin();
    Vaccin(int, QString, QString,QDate, QDate,QString, QString);

    // Getters
    int getIdVaccination() { return id_vaccination; }
    QString getNom() { return nom; }
    QString getType() { return type; }
    QDate getDateCreation() { return date_creation; }
    QDate getDateExpiration() { return date_expiration; }
    QString getStatut() { return statut; }
    QString getCertificationVaccin() { return certification_vaccin; }

    // Setters
    void setIdVaccination(int id) { id_vaccination = id; }
    void setNom(QString n) { nom = n; }
    void setType(QString t) { type= t; }
    void setDateCreation(QDate dc) { date_creation = dc; }
    void setDateExpiration(QDate de) { date_expiration = de; }
    void setStatut(QString s) { statut = s; }
    void setCertificationVaccin(QString cv) { certification_vaccin = cv; }

    // CRUD
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier(int, QString,QString, QDate, QDate,QString, QString);
    bool remplirChampsModification(int id, QString &nom, QString &type, QDate &date_creation, QDate &date_expiration, QString &statut, QString &certification);
    bool modifier(int id);
    QSqlQueryModel * notificationsExpiration();

    // Tri
    QSqlQueryModel * trier(QString critere, QString ordre);

    // Recherche
    QSqlQueryModel * rechercher(QString critere, QString valeur);

    // Notification automatique sur les dates d'expiration


    // Statistiques
    QChartView * statistiquesVaccinations();
};

#endif // VACCIN_H

