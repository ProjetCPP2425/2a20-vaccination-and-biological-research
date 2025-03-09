#ifndef LABORATOIRE_H
#define LABORATOIRE_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
class Laboratoire
{
private:
    int Id_laboratoire;
    QString Nom_Lab;
    QString Adresse;
    QString Type;
    QString Responsable;
    float Dépense;
    int Nb_Projets;
    QString Statut;
    int Matriels;
    int Personnel;
    QDate Date_creation;

public:
    Laboratoire();
    Laboratoire(int id,QString nom, QString adresse, QString type, QString responsable,
                float depense, int nb_projets, QString statut, int matriels, int personnel, QDate date_creation);

   // int getIdLaboratoire();
    QString getNomLab(){ return Nom_Lab; }
    QString getAdresse(){ return Adresse; }
    QString getType(){ return Type; }
    QString getResponsable() { return Responsable; }
    int getNbProjets() { return Nb_Projets; }
    QString getStatut(){ return Statut; }
    float getDepense() { return Dépense; }
    int getMatriels(){ return Matriels; }
    int getPersonnel(){ return Personnel; }
    QDate getDateCreation(){ return Date_creation; }

 //   void setIdLaboratoire(int id);
    void setNomLab(QString nom){ Nom_Lab = nom; }
    void setAdresse(QString adresse){ Adresse = adresse; }
    void setType(QString type) { Type = type; }
    void setResponsable(QString responsable){ Responsable = responsable; }
    void setDepense(float depense) { Dépense = depense; }
    void setNbProjets(int nbProjets){ Nb_Projets = nbProjets; }
    void setStatut(QString statut){ Statut = statut; }
    void setMatriels(int matriels){ Matriels = matriels; }
    void setPersonnel(int personnel) { Personnel = personnel; }
    void setDateCreation(QDate dateCreation){ Date_creation = dateCreation; }



    //CRUD
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int id);
    bool modifier(int id);

};
#endif // LABORATOIRE_H
