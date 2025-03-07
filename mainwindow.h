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
#include <QLabel>
#include <QTextEdit>



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

private:
    Ui::MainWindow *ui;
    Carnets carnetTmp;
    QSortFilterProxyModel *proxyModel;
    QString idAModifier = "";  // ✅ Stocke le CIN à modifier

    bool modeModification = false; // Mode modification activé/désactivé
    void validateInput(QLineEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg);
    void validateTextEdit(QTextEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg);


};
#endif // MAINWINDOW_H
