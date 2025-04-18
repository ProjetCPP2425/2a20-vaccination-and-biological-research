#ifndef COMPAGNE_H
#define COMPAGNE_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Compagne
{
private:
    int Id_Compagne;
    QString Nom_Compagne;
    QDate Date_Debut;
    QDate Date_Fin;
    QString Zone_Geographique;
    int Objectif_Doses;
    int Doses_Administrees;
    QString Vaccins_Utilises;
    QString Statut;

    QString Fournitures;       // ✅ Liste des fournitures nécessaires
    float Cout_Fournitures;     // ✅ Coût estimé pour l'achat des fournitures

public:
    // Constructeurs
    Compagne();
    Compagne(int Id_Compagne, QString Nom_Compagne, QDate Date_Debut, QDate Date_Fin, QString Zone_Geographique,
             int Objectif_Doses, int Doses_Administrees, QString Vaccins_Utilises, QString Statut ,QString Fournitures, float Cout_Fournitures);

    // Getters
    int getIdCompagne() const;
    QString getNomCompagne() const;
    QDate getDateDebut() const;
    QDate getDateFin() const;
    QString getZoneGeographique() const;
    int getObjectifDoses() const;
    int getDosesAdministrees() const;
    QString getVaccinsUtilises() const;
    QString getStatut() const;

    QString getFournitures() const;
    float getCoutFournitures() const;

    // Setters
    void setIdCompagne(int id);
    void setNomCompagne(const QString &nom);
    void setDateDebut(const QDate &dateDebut);
    void setDateFin(const QDate &dateFin);
    void setZoneGeographique(const QString &zone);
    void setObjectifDoses(int objectif);
    void setDosesAdministrees(int doses);
    void setVaccinsUtilises(const QString &vaccins);
    void setStatut(const QString &statut);

    void setFournitures(const QString &fournitures);
    void setCoutFournitures(float cout);

    // CRUD
    bool ajouter();
    QSqlQueryModel * afficher();

    bool modifier(QString nom);
    bool chargerCompagne(QString nom);  // ✅ Charge les données d’une campagne avant modification


    bool supprimer(QString nom);

};

#endif // COMPAGNE_H
