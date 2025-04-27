#include "produit.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
Produit::Produit() {}

Produit::Produit(int idProduit, QString nomProduit, QString categorie, int quantite,
                 QDate dateFabrication, QDate dateExpiration, QString nomFournisseur)
{
    this->idProduit = idProduit;
    this->nomProduit = nomProduit;
    this->categorie = categorie;
    this->quantite = quantite;
    this->dateFabrication = dateFabrication;
    this->dateExpiration = dateExpiration;
    this->nomFournisseur = nomFournisseur;

}
bool Produit::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO PRODUITS (NOM_PRODUIT, CATEGORIE, QUANTITE, DATE_FABRICATION, DATE_EXPIRATION, NOM_FOURNISSEUR) "
                  "VALUES (:nomProduit, :categorie, :quantite, TO_DATE(:dateFabrication, 'YYYY-MM-DD'), TO_DATE(:dateExpiration, 'YYYY-MM-DD'), :nomFournisseur)");

    query.bindValue(":nomProduit", nomProduit);
    query.bindValue(":categorie", categorie);
    query.bindValue(":quantite", QVariant(quantite).toInt());
    query.bindValue(":dateFabrication", dateFabrication.toString("yyyy-MM-dd"));
    query.bindValue(":dateExpiration", dateExpiration.toString("yyyy-MM-dd"));
    query.bindValue(":nomFournisseur", nomFournisseur);

    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Erreur : La connexion à la base de données est fermée.";
        return false;
    }

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du produit :" << query.lastError().text();
        return false;
    }

    qDebug() << "Produit ajouté avec succès !";
    return true;
}
QSqlQueryModel* Produit::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT  NOM_PRODUIT, CATEGORIE, QUANTITE, DATE_FABRICATION, DATE_EXPIRATION, NOM_FOURNISSEUR "
                    "FROM PRODUITS");

    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur lors de l'affichage des produits : " << model->lastError().text();
        return nullptr;
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom du Produit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Quantité"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date de Fabrication"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date d'Expiration"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Nom du Fournisseur"));

    return model;
}
bool Produit::supprimer(QString nomProduit)
{
    QSqlQuery query;

    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "❌ Erreur : Connexion à la base de données fermée.";
        return false;
    }

    query.prepare("DELETE FROM PRODUITS WHERE NOM_PRODUIT = :nomProduit");
    query.bindValue(":nomProduit", nomProduit);

    // 🔹 Exécuter la requête et vérifier le succès
    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de la suppression du produit : " << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Produit supprimé avec succès :" << nomProduit;
    return true;
}
bool Produit::modifier(QString oldNomProduit, QString newNomProduit)
{
    QSqlQuery query;
    query.prepare("UPDATE PRODUITS SET "
                  "NOM_PRODUIT = :newNomProduit, "
                  "CATEGORIE = :categorie, "
                  "QUANTITE = :quantite, "
                  "DATE_FABRICATION = :dateFab, "
                  "DATE_EXPIRATION = :dateExp, "
                  "NOM_FOURNISSEUR = :fournisseur "
                  "WHERE NOM_PRODUIT = :oldNomProduit");

    query.bindValue(":newNomProduit", newNomProduit);
    query.bindValue(":categorie", categorie);
    query.bindValue(":quantite", quantite);
    query.bindValue(":dateFab", dateFabrication);
    query.bindValue(":dateExp", dateExpiration);
    query.bindValue(":fournisseur", nomFournisseur);
    query.bindValue(":oldNomProduit", oldNomProduit);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la modification:" << query.lastError().text();
        qDebug() << "Requête UPDATE:"
                 << "\nAncien NOM_PRODUIT:" << oldNomProduit
                 << "\nNouveau NOM_PRODUIT:" << newNomProduit
                 << "\nCATEGORIE:" << categorie
                 << "\nQUANTITE:" << quantite
                 << "\nDATE_FABRICATION:" << dateFabrication.toString("yyyy-MM-dd")
                 << "\nDATE_EXPIRATION:" << dateExpiration.toString("yyyy-MM-dd")
                 << "\nNOM_FOURNISSEUR:" << nomFournisseur;
        return false;
    }

    qDebug() << "✅ Modification réussie pour :" << oldNomProduit << " → " << newNomProduit;
    return true;
}



bool Produit::remplirChampsModification(QString nomProduit, QString &categorie, int &quantite,
                                        QDate &dateFabrication, QDate &dateExpiration, QString &nomFournisseur)
{  QSqlQuery query;

    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "❌ Erreur : Connexion à la base de données fermée.";
        return false;
    }

    query.prepare("SELECT CATEGORIE, QUANTITE, DATE_FABRICATION, DATE_EXPIRATION, NOM_FOURNISSEUR "
                  "FROM PRODUITS WHERE NOM_PRODUIT = :nomProduit");
    query.bindValue(":nomProduit", nomProduit);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors du chargement des données du produit : " << query.lastError().text();
        return false;
    }

    if (query.next()) {
        // 🔹 Récupérer les valeurs
        categorie = query.value(0).toString();
        quantite = query.value(1).toInt();
        dateFabrication = query.value(2).toDate();
        dateExpiration = query.value(3).toDate();
        nomFournisseur = query.value(4).toString();

        qDebug() << "✅ Données chargées pour le produit :" << nomProduit;
        return true;

    } else {
        qDebug() << "❌ Échec de la modification du produit :" << nomProduit;
        return false;
    }
}

QSqlQueryModel* Produit::rechercherTout(const QString& critere)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    QString critereLower = critere.toLower();

    query.prepare(R"(
        SELECT * FROM SMARTVACC.PRODUITS
        WHERE LOWER(NOM_PRODUIT) LIKE '%' || :critere || '%'
           OR LOWER(CATEGORIE) LIKE '%' || :critere || '%'
           OR LOWER(NOM_FOURNISSEUR) LIKE '%' || :critere || '%'
    )");

    query.bindValue(":critere", critereLower);

    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de la recherche :" << query.lastError().text();
    }

    model->setQuery(query);
    return model;
}




QSqlQueryModel* Produit::trierPar(const QString& critere)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString baseQuery = "SELECT ID_PRODUIT, NOM_PRODUIT, CATEGORIE, QUANTITE, DATE_FABRICATION, DATE_EXPIRATION, NOM_FOURNISSEUR FROM PRODUITS ";
    QString query;

    if (critere == "QTY_ASC") {
        query = baseQuery + "ORDER BY QUANTITE ASC";
    }
    else if (critere == "EXP_DESC") {
        query = baseQuery + "ORDER BY TO_DATE(DATE_EXPIRATION, 'YYYY-MM-DD') DESC ";
    }
    else if (critere == "FAB_ASC") {
        query = baseQuery + "ORDER BY TO_DATE(DATE_FABRICATION, 'YYYY-MM-DD') DESC ";
    }

    else {
        qDebug() << "❌ Critère de tri inconnu : " << critere;
        delete model;
        return nullptr;
    }

    qDebug() << "✅ Requête exécutée : " << query;

    model->setQuery(query);

    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur SQL :" << model->lastError().text();
        delete model;
        return nullptr;
    }

    // Ajout des entêtes des colonnes
   /* model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom du produit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Quantité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date de fabrication"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'expiration"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Fournisseur"));
*/
    return model;
}
QSqlQueryModel* Produit::rapportStockSecurite()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString query = R"(
        SELECT NOM_FOURNISSEUR,
               NOM_PRODUIT,
               CATEGORIE,
               QUANTITE,
               TO_DATE(DATE_FABRICATION, 'YYYY-MM-DD') AS DATE_FAB,
               TO_DATE(DATE_EXPIRATION, 'YYYY-MM-DD') AS DATE_EXP
        FROM PRODUITS
        WHERE QUANTITE BETWEEN 1 AND 5
        ORDER BY NOM_FOURNISSEUR ASC, NOM_PRODUIT ASC
    )";


    model->setQuery(query);  // ✅ corrige l'avertissement


    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur rapport stock de sécurité :" << model->lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}

QMap<QString, int> Produit::calculerTauxStock() {
    QMap<QString, int> result;
    QSqlQuery query;

    query.exec("SELECT QUANTITE FROM PRODUITS");

    while (query.next()) {
        int qty = query.value(0).toInt();

        if (qty == 1)
            result["Rupture"]++;
        else if (qty >= 2 && qty <= 5)
            result["StockSécurité"]++;
        else
            result["StockNormal"]++;
    }

    return result;
}

QString Produit::MessageDeExpiration()
{
    QString message="";
    QSqlQuery query;
    query.prepare("SELECT NOM_PRODUIT, CATEGORIE, QUANTITE, DATE_FABRICATION, DATE_EXPIRATION, NOM_FOURNISSEUR "
                  "FROM PRODUITS "
                  "WHERE DATE_EXPIRATION BETWEEN SYSDATE AND SYSDATE + 7");
if(query.exec())
{  bool aDesProduits = false;
   while(query.next())

   { if (!aDesProduits) {
           message += "⚠️ Attention : les produits suivants sont périmés ou vont expirer dans les 7 jours :\n\n";
           aDesProduits = true;
       }

       QString nomP =query.value(0).toString() ;
       QString categorie =query.value(1).toString() ;
       QString quantite =query.value(2).toString();
       QString dateF =query.value(3).toString();
       QString dateE =query.value(4).toString();
       QString nomF =query.value(5).toString();

       QString text = " nom Produit: " + nomP + "\n"
                       "categorie: " + categorie + "\n"
                       "quantite: " + quantite + "\n"
                "date de fabrucation: " + dateF + "\n"
                "date d'expiration: " + dateE + "\n"
                "nom furnisseur: " + nomF + "\n";
       message=message+text;
   }


}
return message;

}

QString Produit::MessageAlerte()
{
    QString message="";
    QSqlQuery query;
    query.prepare("SELECT NOM_PRODUIT, CATEGORIE, QUANTITE "
                  "FROM PRODUITS "
                  "WHERE CATEGORIE='Vaccin' ");
    if(query.exec())
    {  bool aDesProduits = false;
        while(query.next())

        { if (!aDesProduits) {
                message += "⚠️ Attention : Coupure courant Ces produits necessitent une refregeration  :\n\n";
                aDesProduits = true;
            }

            QString nomP =query.value(0).toString() ;
            QString categorie =query.value(1).toString() ;
            QString quantite =query.value(2).toString();


            QString text = " nom Produit: " + nomP + "\n"
                                                     "categorie: " + categorie + "\n"
                                         "quantite: " + quantite + "\n";
            message=message+text;
        }


    }
    return message;

}
