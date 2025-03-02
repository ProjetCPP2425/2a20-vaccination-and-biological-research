#include "employes.h"

// Constructeur par défaut
Employe::Employe() {}

// Constructeur avec paramètres
Employe::Employe(QString cin, int id, QString nom, QString prenom, QString poste, QString sexe, float salaire,
                 QString contact, QDate date_embauche, int disponibilite, QString type_absences)
{
    this->CIN = cin;
    this->ID_EMPLOYE = id;
    this->NOM = nom;
    this->PRENOM = prenom;
    this->POSTE = poste;
    this->SEXE = sexe;
    this->SALAIRE = salaire;
    this->CONTACT = contact;
    this->DATE_EMBAUCHE = date_embauche;
    this->DISPONIBILITE = disponibilite;
    this->TYPE_ABSENCES = type_absences;
}
