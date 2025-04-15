#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "compagne.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QSqlError>

#include <QMap>




#include <QListWidget>
#include <QDropEvent>
#include <QMimeData>


#include <QListWidgetItem>
#include <QDebug>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE





#include <QListWidget>
#include <QMenu>
#include <QAction>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

class DragDropListWidget : public QListWidget {
    Q_OBJECT
public:
    int id_compagne;

    DragDropListWidget(QWidget *parent, int compagneId)
        : QListWidget(parent), id_compagne(compagneId) {
        setAcceptDrops(true);
        setDragEnabled(true);
        setDefaultDropAction(Qt::CopyAction);
        setContextMenuPolicy(Qt::CustomContextMenu);  // 🎯 important !
        connect(this, &QListWidget::customContextMenuRequested, this, &DragDropListWidget::afficherMenuContextuel);
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
            event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent *event) override {
        QListWidget::dropEvent(event);

        QListWidgetItem *item = currentItem();
        if (!item) return;

        int id_employe = item->data(Qt::UserRole).toInt();

        // Éviter les doublons
        for (int i = 0; i < count(); ++i) {
            if (item == this->item(i)) continue;
            if (this->item(i)->data(Qt::UserRole).toInt() == id_employe)
                return;
        }

        // Cloner l'item
        QListWidgetItem *copie = new QListWidgetItem(*item);
        addItem(copie);
    }

private slots:
    void afficherMenuContextuel(const QPoint &pos) {
        QListWidgetItem *item = itemAt(pos);
        if (!item || item->flags() == Qt::NoItemFlags) return;  // ⛔ ne pas afficher le menu pour les items non déplaçables

        QMenu menu;
        QAction *supprimerAction = menu.addAction("❌ Retirer de la compagne");

        QAction *choix = menu.exec(mapToGlobal(pos));
        if (choix == supprimerAction) {
            int id_employe = item->data(Qt::UserRole).toInt();

            // ✅ Supprimer de la base
            QSqlQuery query;
            query.prepare("DELETE FROM CONTRIBUER WHERE ID_EMPLOYE = :e AND ID_COMPAGNE = :c");
            query.bindValue(":e", id_employe);
            query.bindValue(":c", id_compagne);

            if (!query.exec()) {
                QMessageBox::critical(this, "Erreur", "❌ Échec suppression : " + query.lastError().text());
                return;
            }

            // ✅ Supprimer visuellement
            delete takeItem(row(item));
        }
    }
};







class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // ✅ Rendez-les accessibles
    void chargerEmployes();
    void chargerCompagnes();

    void afficherAffectations();  // Génère le tableau dynamique


private slots:
    void on_pushButton_ajouter_clicked();

    void on_button_modifier_clicked();  // ✅ Gestion de la modification

    void displayCompagne();
    void navigateToCampaigns();

    void on_Button_supprimer_clicked();


    //metier1
    void on_calendar_campaigns_clicked(const QDate &date);


    void on_btn_export_campaigns_clicked();

    void rechercherParDate(const QDate &date);


    void genererPDFApprovisionnement(const Compagne& compagne);




    void afficherStatistiquesVaccinationParZone();

    void validerAffectations();


    //trie
    void trierCompagnes(const QString &critere);

    //recherche
    void rechercherCompagnes(const QString &texte);

    //Annuler
    void on_pushButton_annuler_clicked();


private:
    Ui::MainWindow *ui;
    Compagne compagneTmp;


    bool modeModification = false; // ✅ False par défaut (ajout), True si modification
    QString nomOriginal; // ✅ Stocke le nom original de la campagne pour la modification


    bool estValide(); // ✅ Ajout de la fonction de validation des champs




    //metier 1
    void chargerCompagnesParDate(const QDate &date);
    void alerterCompagnesImminentes();

    //metier2

    void affecterEmployeACompagne();
    bool aConflitAvecCompagne(int id_employe, QDate nouvelleDebut, QDate nouvelleFin);


    QMap<QString, int> mapEmployes;
    QMap<QString, int> mapCompagnes;




    QMap<QString, QList<QString>> mapAffectations;  // Map : compagne => liste d'affectés


    QList<class DragDropListWidget*> listeWidgetsCampagnes;



};












#endif // MAINWINDOW_H
