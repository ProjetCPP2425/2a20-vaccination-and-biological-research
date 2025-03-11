
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



    //void remplirChampsModification(int id);

private:
    Ui::MainWindow *ui;
    Employe employe;


    bool modeModification = false;
    QString cinOriginal;
    void validateInput(QLineEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg);
    void validateTextEdit(QTextEdit *field, QLabel *errorLabel, QRegularExpression regex);
    bool estValide();



};
#endif // MAINWINDOW_H

