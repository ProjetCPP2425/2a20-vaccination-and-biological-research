#ifndef EMPLOYES_H
#define EMPLOYES_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

class Employe {
private:
    QString CIN;
    int ID_EMPLOYE;
    QString NOM;
    QString PRENOM;
    QString POSTE;
    QString SEXE;
    float SALAIRE;
    QString CONTACT;
    QDate DATE_EMBAUCHE;
    int DISPONIBILITE;
    QString TYPE_ABSENCES;
    QString RFID_ID; // ✅ Nouveau

public:
    QString lastError;

    Employe();
    Employe(QString cin, int id, QString nom, QString prenom, QString poste, QString sexe, float salaire,
            QString contact, QDate date_embauche, int disponibilite, QString type_absences);

    QString getCIN() const;
    int getIdEmploye() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getPoste() const;
    QString getSexe() const;
    float getSalaire() const;
    QString getContact() const;
    QDate getDateEmbauche() const;
    int getDisponibilite() const;
    QString getTypeAbsences() const;
    QString getRFID_ID() const; // ✅
    QString getPrenomFromCIN(const QString &cin);

    void setCIN(const QString &cin);
    void setIdEmploye(int id);
    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setPoste(const QString &poste);
    void setSexe(const QString &sexe);
    void setSalaire(float salaire);
    void setContact(const QString &contact);
    void setDateEmbauche(const QDate &date);
    void setDisponibilite(int disponibilite);
    void setTypeAbsences(const QString &typeAbsences);
    void setRFID_ID(const QString &rfid); // ✅

    bool ajouter();
    bool modifier(QString cin);
    bool supprimerParCIN(QString cin);
    bool chargerEmploye(QString cin);
    QSqlQueryModel* afficher();
    bool authentifier(const QString &login, const QString &cin);
    QString getPosteFromCIN(const QString &cin);
    QSqlQueryModel* afficherParPoste();
    QSqlQueryModel* afficherParAnciennete();
    QSqlQueryModel* afficherParSalaire();
    QSqlQueryModel* rechercherParCIN(const QString &cin);
    QSqlQueryModel* rechercherParContact(const QString &contact);
    QSqlQueryModel* rechercherParDisponibilite(int dispo);
};

#endif // EMPLOYES_H
