#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "carnets.h"
#include "smsnotif.h"

#include <QStyledItemDelegate>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QTextEdit>

#include <QTimer>
#include <QDate>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class ButtonDelegate;

}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajout_carnet_clicked();
    void displayCarnet();
    void on_supprimerC_clicked();

    void on_rechercheC_textChanged(const QString &arg1);

    void on_modifierC_clicked();

    void on_btnPredire_clicked();

    void on_annuler_clicked();


    void on_btnGeneratePDF_clicked();



    void on_btnStat_clicked();

private:
    Ui::MainWindow *ui;
    Carnets carnetTmp;
    QSortFilterProxyModel *proxyModel;
    QString idAModifier = "";

    bool modeModification = false; // Mode modification activé/désactivé
    void validateInput(QLineEdit *field, QLabel *errorLabel, QRegularExpression regex,
                       const QString &errorMsg, bool checkZeros, bool allowOnlyNumbers, int minValue, int maxValue);
        void validateTextEdit(QTextEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg);
    bool estValide();
        void envoyerRappelSMS();


        void testSMS();
        QTimer *smsTimer;

        void trierCarnets(const QString &critere);
};
#endif // MAINWINDOW_H
