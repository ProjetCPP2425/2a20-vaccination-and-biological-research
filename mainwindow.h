#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "carnets.h"
#include <QStyledItemDelegate>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QDebug>
#include <QSortFilterProxyModel>



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

private:
    Ui::MainWindow *ui;
    Carnets carnetTmp;
    QSortFilterProxyModel *proxyModel;
    int idAModifier = -1;  // ID du carnet à modifier (-1 signifie pas de modification)
    bool modeModification = false; // Mode modification activé/désactivé
};
#endif // MAINWINDOW_H
