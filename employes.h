#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QCategoryAxis>

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

public:
    // Constructeurs
    Employe();
    Employe(QString cin, int id, QString nom, QString prenom, QString poste, QString sexe, float salaire,
            QString contact, QDate date_embauche, int disponibilite, QString type_absences);

    // Getters
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

    // Setters
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

    // Méthodes pour interaction avec la base de données (déclarées mais pas implémentées ici)
    bool ajouter();
   bool modifier(int id_employe);
    bool supprimerParCIN(QString cin);

    QSqlQueryModel* afficher();
private:
    QString lastError;
public:
    QString getLastError() const { return lastError; }


};

#endif // EMPLOYE_H
