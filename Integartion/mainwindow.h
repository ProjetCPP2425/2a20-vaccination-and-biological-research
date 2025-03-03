
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vaccin.h"
 //#include "buttondelegate.h"
#include <QStyledItemDelegate>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QDebug>       // (Facultatif) Pour le debug
#include <QVector>
#include <QDate>
#include <QSystemTrayIcon>
#include "medanalysis.h"
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
    void on_pushButton_ajouter_v_clicked();
    void on_pushButton_suppv_clicked();
    void on_pushButton_modifier_2_clicked();
    void remplirChampsModification(int id);

    void on_lineEdit_recherche_2_textChanged(const QString &arg1);
    void verifierVaccinsExpires();
    void updateCovidStats(QString country, int cases, int deaths, int recovered, int population, double vaccinationRate);
    void showError(QString error);
private:
    Ui::MainWindow *ui;
    Vaccin vacc;
    bool modeModification = false;
    int idAModifier = -1;

};
#endif // MAINWINDOW_H
