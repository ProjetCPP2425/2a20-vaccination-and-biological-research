#include "laboratoires.h"

laboratoires::laboratoire() {}

laboratoires::laboratoire(int id, QString nom_lab, QString adresse, QString type, QString responsable,
                           float depense, int nb_projets, QString statut, float materiels, float personnel,
                           QDate date_creation)
{
    this->id_laboratoire = id;
    this->nom_lab = nom_lab;
    this->adresse = adresse;
    this->type = type;
    this->responsable = responsable;
    this->depense = depense;
    this->nb_projets = nb_projets;
    this->statut = statut;
    this->materiels = materiels;
    this->personnel = personnel;
    this->date_creation = date_creation;
}
