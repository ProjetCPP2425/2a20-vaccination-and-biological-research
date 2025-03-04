#ifndef CARNETS_H
#define CARNETS_H
#include <QString>
#include <QDate>
#include <QSqlQueryModel>


class Carnets
{
private:
    int Id_Carnet;
    QString Cin;
    QString Nom;
    QString Prenom;
    int Age;
    QString Sexe;
    QString Num;
    float Poids;
    QDate Date_Rdv;
    QString Remarques;
    QString Statut_Vaccinal;

public:
    Carnets();
    Carnets(int Id_Carnet, QString Cin, QString Nom, QString Prenom, int Age, QString Sexe, QString Num,
            float Poids, QDate Date_Rdv, QString Remarques, QString Statut_Vaccinal);

    int GetId_Carnet();
    QString GetCin();
    QString GetNom();
    QString GetPrenom();
    int GetAge();
    QString GetSexe();
    QString GetNum();
    float GetPoids();
    QDate GetDate_Rdv();
    QString GetRemarques();
    QString GetStatut_Vaccinal();


    void SetId_Carnet(int Id_Carnet);
    void SetCin(QString Cin);
    void SetNom(QString Nom);
    void SetPrenom(QString Prenom);
    void SetAge(int Age);
    void SetSexe(QString Sexe);
    void SetNum(QString Num);
    void SetPoids(float Poids);
    void SetDate_Rdv(QDate Date_Rdv);
    void SetRemarques(QString Remarques);
    void SetStatut_Vaccinal(QString Statut_Vaccinal);


    //CRUD
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier(int id, QString cin, QString nom, QString prenom, int age, QString sexe,
                  QString num, float poids, QDate date_rdv, QString remarques, QString statut_vaccinal);
    bool remplirChampsModification(int id, QString &cin, QString &nom, QString &prenom, int &age,
                                   QString &sexe, QString &num, float &poids, QDate &date_rdv,
                                   QString &remarques, QString &statut_vaccinal);
    QString analyserRisque(int id);
    int getDernierIdAjout();



};

#endif // CARNETS_H
