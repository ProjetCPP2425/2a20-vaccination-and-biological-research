#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "compagne.h"

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
    void on_pushButton_ajouter_clicked();  // Slot pour l'ajout de la campagne
    void displayCompagne();  // Slot pour afficher les campagnes

private:
    Ui::MainWindow *ui;
    Compagne compagneTmp;  // Instance de la classe Compagne pour gérer les campagnes
};

#endif // MAINWINDOW_H
