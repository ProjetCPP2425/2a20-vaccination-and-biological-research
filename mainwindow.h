
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employes.h"
#include "carnets.h"
#include "smsnotif.h"
#include <QStyledItemDelegate>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QDebug>       // (Facultatif) Pour le debug
#include <QLabel>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QDesktopServices>
#include <QUrl>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextOption>               // Pour setWordWrapMode()
#include <QTextEdit>                 // Si manquant pour textEdit_chat
#include <QLineEdit>                 // Pour lineEdit_question
#include <QLabel>                    // Si tu utilises des QLabel pour erreurs
#include <QJsonParseError>           // Pour gérer les erreurs JSON
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QMap>


#include <QStyledItemDelegate>

#include <QSortFilterProxyModel>


#include <QTimer>
#include <QDate>
#include <QTimer>
#include <QPushButton> // pour QPushButton
#include <QResizeEvent> // pour resizeEvent




//daoussar



#include "compagne.h"

#include <QListWidget>
#include <QDropEvent>
#include <QMimeData>


#include <QListWidgetItem>




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










class ButtonDelegate;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    //daoussar

    void afficherAffectations(); // Affiche le tableau des affectations dynamique (style Trello)


private slots:

    void on_pushButton_supprimer_employe_clicked();
    void on_annuler_clicked();
    void on_ajouter_employe_clicked();
    void on_button_modifier_clicked();
    //void seConnecter();
    void appliquerTriEmployes();  // nouveau slot
    void lancerRechercheEmploye();
    void reinitialiserTableEmployes();
    void chargerNomsDansComboBoxPDF();
    void exporterPlanningHoraireHTML();
   // void calculerTauxAbsenteisme();
    void afficherStatistiquesEmployes();
    void afficherTableauAbsencesDetaillees();
    void on_button_chatbot_icon_clicked();
    void on_button_envoyer_clicked();
    void receptionReponseGPT(QNetworkReply *reply);
      void on_button_fermer_chatbot_clicked();
    void afficherHistoriqueConnexions();
      void on_pushButtonStat_15_clicked();

      void on_pushButton_pdf_3_clicked();
 // Pour envoyer la question



    //void actualiserStatistiquesAbsenteisme();
    void on_ajout_carnet_clicked();
    void displayCarnet();
    void on_supprimerC_clicked();

    void on_rechercheC_textChanged(const QString &arg1);

    void on_modifierC_clicked();

    void on_btnPredire_clicked();

    void on_annulerr_clicked();


    void on_btnGeneratePDF_clicked();



    void on_btnStat_clicked();



    //daoussar

    // === 🔁 Gestion CRUD des compagnes ===
    void on_pushButton_ajouter_clicked();     // ➕ Ajouter ou modifier une compagne
    void on_button_modifierD_clicked();        // ✏️ Charger une compagne dans le formulaire pour modification
    void on_Button_supprimer_clicked();       // 🗑️ Supprimer une compagne
    void on_pushButton_annuler_clicked();     // 🔁 Réinitialiser ou restaurer le formulaire

    void displayCompagne();                   // 📋 Afficher la liste des compagnes
    void navigateToCampaigns();               // 🧭 Aller vers la page des campagnes

    // === 📅 Métier 1 - Gestion calendrier et PDF campagne ===
    void on_calendar_campaigns_clicked(const QDate &date);    // 📆 Clic sur calendrier
    void rechercherParDate(const QDate &date);                // 🔍 Recherche campagnes actives à une date
    void on_btn_export_campaigns_clicked();                   // 🧾 Exporter les campagnes affichées en PDF
    void genererPDFApprovisionnement(const Compagne& compagne); // 📦 Générer un PDF pour une compagne spécifique

    // === 👨‍⚕️ Métier 2 - Affectation des employés ===
    void validerAffectations();                 // ✅ Valider les affectations (glisser-déposer)

    // === 📊 Statistiques ===
    void afficherStatistiquesVaccinationParZone();  // 📈 Générer statistiques (camembert)

    // === 🔍 Recherche et tri dynamique ===
    void rechercherCompagnes(const QString &texte); // 🔍 Filtrer les compagnes
    void trierCompagnes(const QString &critere);    // ↕️ Trier les compagnes


    void on_pushButtonStat_14_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;




    //void remplirChampsModification(int id);

    void on_Affichage_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    Carnets carnetTmp;
    QSortFilterProxyModel *proxyModel;
    QString idAModifier = "";

    bool modeModifications = false; // Mode modification activé/désactivé
    void validateInput(QLineEdit *field, QLabel *errorLabel, QRegularExpression regex,
                       const QString &errorMsg, bool checkZeros, bool allowOnlyNumbers, int minValue, int maxValue);
    void validateTextEdit(QTextEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg);
    bool estValides();
    void envoyerRappelSMS();


    void testSMS();
    QTimer *smsTimer;

    void trierCarnets(const QString &critere);







    Employe employe;
    QMap<QString, QString> cacheGPT;
    QPushButton *chatbotButton;

    QChartView *chartViewStatistique = nullptr;  // pour éviter plusieurs affichages
    QChartView *chartViewParPoste = nullptr;
    QNetworkAccessManager *manager;
    void envoyerRequeteChatGPT(const QString &message);
    QJsonArray faq;
    int tentativeConnexion = 0;
    bool connexionBloquee = false;





    bool modeModification = false;
    QString cinOriginal;
    void validateInput(QLineEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg);
    void validateTextEdit(QTextEdit *field, QLabel *errorLabel, QRegularExpression regex);
    bool estValide();
    void seConnecter();
    void redirectTo(const QString &poste, int pageIndex, bool fullAccess = false);
    void disableOtherModulesExcept(const QStringList &allowed);
    void insererEmployesTest();
    QString genererReponseRH(const QString &question);
    QMap<QString, QString> faqMap;
    QString chercherReponseFAQ(const QString &question);
    void enregistrerConnexion(const QString& login);
    void afficherDashboard(const QString& prenom, const QString& poste);
    void afficherChatbot();
    void convertirHTMLenPDF(const QString& cheminHTML);




//daoussar

    // === 🧩 Données internes ===
    Compagne compagneTmp;          // Objet temporaire pour ajout/modification
    bool modeModification_d = false; // True si on modifie une campagne existante
    QString nomOriginal;           // Nom de la campagne avant modification

    // === 🔐 Validation ===
    bool estValide_d(); // Vérifie que tous les champs sont correctement remplis

    // === ⏰ Métier 1 - Fonctions internes ===
    void chargerCompagnesParDate(const QDate &date);  // Campagnes actives à une date
    void alerterCompagnesImminentes();                // Affiche un message si une campagne commence bientôt

    // === 📦 Métier 2 - Affectation

    bool aConflitAvecCompagne(int id_employe, QDate nouvelleDebut, QDate nouvelleFin); // Vérifie les conflits

    // === 🗺️ Structures de données utilisées ===

    QMap<QString, QList<QString>> mapAffectations;  // Compagne → Liste employés
    QList<DragDropListWidget*> listeWidgetsCampagnes; // Colonnes des campagnes






};
#endif // MAINWINDOW_H

