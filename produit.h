#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Produit
{
private:
    int idProduit;
    QString nomProduit;
    QString categorie;
    int quantite;
    QDate dateFabrication;
    QDate dateExpiration;
    QString nomFournisseur;

public:
    Produit();
    Produit(int idProduit, QString nomProduit, QString categorie, int quantite,
            QDate dateFabrication, QDate dateExpiration, QString nomFournisseur);

    int GetIdProduit() const;
    QString GetNomProduit() const;
    QString GetCategorie() const;
    int GetQuantite() const;
    QDate GetDateFabrication() const;
    QDate GetDateExpiration() const;
    QString GetNomFournisseur() const;

    void SetIdProduit(int idProduit);
    void SetNomProduit(QString nomProduit);
    void SetCategorie(QString categorie);
    void SetQuantite(int quantite);
    void SetDateFabrication(QDate dateFabrication);
    void SetDateExpiration(QDate dateExpiration);
    void SetNomFournisseur(QString nomFournisseur);


    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(QString nomProduit);
    bool modifier( QString oldNomProduit, QString newNomProduit);
    bool remplirChampsModification(QString nomProduit, QString &categorie, int &quantite,
                                   QDate &dateFabrication, QDate &dateExpiration, QString &nomFournisseur);

    QSqlQueryModel* rechercherTout(const QString& critere);
    QSqlQueryModel* trierPar(const QString& critere);
    QSqlQueryModel* rapportStockSecurite();
    QMap<QString, int> calculerTauxStock();

    QString MessageDeExpiration();

};

#endif // PRODUIT_H
