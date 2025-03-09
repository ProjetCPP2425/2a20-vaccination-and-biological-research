#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employes.h"
#include <QStyledItemDelegate>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QDebug>       // (Facultatif) Pour le debug

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
private slots:
   // void on_pushButton_modifier_employe_clicked();
    void on_pushButton_supprimer_employe_clicked();
    void on_ajouter_employe_clicked();


    //void remplirChampsModification(int id);

private:
    Ui::MainWindow *ui;
    Employe employe;

};
#endif // MAINWINDOW_H
