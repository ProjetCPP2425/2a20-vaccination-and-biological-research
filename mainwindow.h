#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "compagne.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QSqlError>

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
    void on_pushButton_ajouter_clicked();

    void on_button_modifier_clicked();  // ✅ Gestion de la modification

    void displayCompagne();
    void navigateToCampaigns();

    void on_Button_supprimer_clicked();




private:
    Ui::MainWindow *ui;
    Compagne compagneTmp;


    bool modeModification = false; // ✅ False par défaut (ajout), True si modification
    QString nomOriginal; // ✅ Stocke le nom original de la campagne pour la modification


    bool estValide(); // ✅ Ajout de la fonction de validation des champs
};

#endif // MAINWINDOW_H
