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
#include "arduinotension.h"
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


    void on_ajouterP_clicked();
    void validerChampsP();

    void on_supprimer_clickedP();

    void on_Affichage_2_currentChanged(int index);


    void remplirChampsModificationP(QString nomProduit);


    void verifierNomProduit();



    void on_modifierP_clicked();



    void on_pushButton_11_clicked();



    void on_rechercherP_clicked();



    void lireDonneesSerie();


    void on_comboBox_6P_activated(int index);

    //void on_pushButton_clicked();

    void on_pdfP_clicked();

    void on_StatP_clicked();

    void on_TableViewP_clicked(const QModelIndex &index);

    void on_telecharger_qr_code_clicked();



private:
    Ui::MainWindow *ui;
    Produit produitTmp;
    QString produitAModifier;
    bool modeModification = false;
mailer email;
    arduinotension A ;
     QString bufferSerie;
};
#endif // MAINWINDOW_H
