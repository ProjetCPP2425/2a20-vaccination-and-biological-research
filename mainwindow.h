#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "produit.h"
#include <QStyledItemDelegate>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include "email.h"
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


    void on_ajouter_clicked();
    void validerChamps();

    void on_supprimer_clicked();

    void on_Affichage_2_currentChanged(int index);


    void remplirChampsModification(QString nomProduit);


    void verifierNomProduit();



    void on_modifier_clicked();



    void on_pushButton_11_clicked();



    void on_rechercher_clicked();






    void on_comboBox_6_activated(int index);

    //void on_pushButton_clicked();

    void on_pdf_clicked();

    void on_Stat_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_telecharger_qr_code_clicked();

private:
    Ui::MainWindow *ui;
    Produit produitTmp;
    QString produitAModifier;
    bool modeModification = false;
mailer email;
};
#endif // MAINWINDOW_H
