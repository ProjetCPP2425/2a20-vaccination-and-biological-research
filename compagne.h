#ifndef COMPAGNE_H
#define COMPAGNE_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Compagne
{
private:
    int Id_Campagne;
    QString Nom_Campagne;
    QDate Date_Debut;
    QDate Date_Fin;
    QString Zone_Geographique;
    int Objectif_Doses;
    int Doses_Administrees;
    QString Vaccins_Utilises;
    QString Statut;

public:
    // Constructeurs
    Compagne();
    Compagne(int Id_Campagne, QString Nom_Campagne, QDate Date_Debut, QDate Date_Fin,
             QString Zone_Geographique, int Objectif_Doses, int Doses_Administrees,
             QString Vaccins_Utilises, QString Statut);

    // Getters
    int GetId_Campagne();
    QString GetNom_Campagne();
    QDate GetDate_Debut();
    QDate GetDate_Fin();
    QString GetZone_Geographique();
    int GetObjectif_Doses();
    int GetDoses_Administrees();
    QString GetVaccins_Utilises();
    QString GetStatut();

    // Setters
    void SetId_Campagne(int Id_Campagne);
    void SetNom_Campagne(QString Nom_Campagne);
    void SetDate_Debut(QDate Date_Debut);
    void SetDate_Fin(QDate Date_Fin);
    void SetZone_Geographique(QString Zone_Geographique);
    void SetObjectif_Doses(int Objectif_Doses);
    void SetDoses_Administrees(int Doses_Administrees);
    void SetVaccins_Utilises(QString Vaccins_Utilises);
    void SetStatut(QString Statut);

    // CRUD
    bool ajouter();
    QSqlQueryModel * afficher();
};

#endif // COMPAGNE_H
