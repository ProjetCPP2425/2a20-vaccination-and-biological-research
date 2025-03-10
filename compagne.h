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
    Compagne(int Id_Campagne, QString Nom_Campagne, QDate Date_Debut, QDate Date_Fin, QString Zone_Geographique,
             int Objectif_Doses, int Doses_Administrees, QString Vaccins_Utilises, QString Statut);

    // Getters
    int getIdCampagne() const;
    QString getNomCampagne() const;
    QDate getDateDebut() const;
    QDate getDateFin() const;
    QString getZoneGeographique() const;
    int getObjectifDoses() const;
    int getDosesAdministrees() const;
    QString getVaccinsUtilises() const;
    QString getStatut() const;

    // Setters
    void setIdCampagne(int id);
    void setNomCampagne(const QString &nom);
    void setDateDebut(const QDate &dateDebut);
    void setDateFin(const QDate &dateFin);
    void setZoneGeographique(const QString &zone);
    void setObjectifDoses(int objectif);
    void setDosesAdministrees(int doses);
    void setVaccinsUtilises(const QString &vaccins);
    void setStatut(const QString &statut);

    // CRUD
    bool ajouter();
    QSqlQueryModel * afficher();

    bool modifier(QString nom);
    bool chargerCampagne(QString nom);  // ✅ Charge les données d’une campagne avant modification


    bool supprimer(QString nom);

};

#endif // COMPAGNE_H
