#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "laboratoire.h"
#include <QMainWindow>

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
    void on_pushButton_32_clicked();
    void displayLaboratoires();
    void on_pushButton_37_clicked();
    void on_pushButton_34_clicked();
   // void validateFields();
  //  void on_rechercherLabo_textChanged( QString &arg1);
  // void on_comboBox_Tri_currentIndexChanged(int index);

   private:
    Ui::MainWindow *ui;
    Laboratoire labTmp;
};
#endif // MAINWINDOW_H
