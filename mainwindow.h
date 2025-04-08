
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employes.h"
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


//QT_CHARTS_USE_NAMESPACE




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
 // Pour envoyer la question


    //void actualiserStatistiquesAbsenteisme();







    //void remplirChampsModification(int id);

    void on_Affichage_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    Employe employe;
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







};
#endif // MAINWINDOW_H

