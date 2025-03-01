#include "compagne.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

// Constructeurs
Compagne::Compagne() {}

Compagne::Compagne(int Id_Campagne, QString Nom_Campagne, QDate Date_Debut, QDate Date_Fin,
                   QString Zone_Geographique, int Objectif_Doses, int Doses_Administrees,
                   QString Vaccins_Utilises, QString Statut)
{
    this->Id_Campagne = Id_Campagne;
    this->Nom_Campagne = Nom_Campagne;
    this->Date_Debut = Date_Debut;
    this->Date_Fin = Date_Fin;
    this->Zone_Geographique = Zone_Geographique;
    this->Objectif_Doses = Objectif_Doses;
    this->Doses_Administrees = Doses_Administrees;
    this->Vaccins_Utilises = Vaccins_Utilises;
    this->Statut = Statut;
}

// Getters et Setters (comme précédemment)


// Méthode pour ajouter la campagne dans la base de données
bool Compagne::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO campagne (Nom_Campagne, Date_Debut, Date_Fin, Zone_Geographique, "
                  "Objectif_Doses, Doses_Administrees, Vaccins_Utilises, Statut) "
                  "VALUES (:nom, :debut, :fin, :zone, :objectif, :doses, :vaccins, :statut)");

    query.bindValue(":nom", Nom_Campagne);
    query.bindValue(":debut", Date_Debut);
    query.bindValue(":fin", Date_Fin);
    query.bindValue(":zone", Zone_Geographique);
    query.bindValue(":objectif", Objectif_Doses);
    query.bindValue(":doses", Doses_Administrees);
    query.bindValue(":vaccins", Vaccins_Utilises);
    query.bindValue(":statut", Statut);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de la campagne : " << query.lastError().text();
        return false;
    }
    return true;
}

// Ancienne méthode pour afficher les campagnes
QSqlQueryModel* Compagne::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    // Requête pour afficher les campagnes
    model->setQuery("SELECT Nom_Campagne, Date_Debut, Date_Fin, Zone_Geographique, "
                    "Objectif_Doses, Doses_Administrees, Vaccins_Utilises, Statut FROM campagne");

    // Vérifier les erreurs SQL
    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'affichage des campagnes : " << model->lastError().text();
        return nullptr;
    }

    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom de la Campagne"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date de Début"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date de Fin"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Zone Géographique"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Objectif des Doses"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Doses Administrées"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Vaccins Utilisés"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}
