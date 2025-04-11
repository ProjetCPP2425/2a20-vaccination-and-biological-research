#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "laboratoire.h"
#include <QMainWindow>
#include <QMap>
#include<QtNetwork/QTcpSocket>
#include<QtNetwork/QTcpServer>

#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QDataStream>
#include <QString>
#include <QByteArray>
#include <QList>



#include <QTcpSocket>

#include <QTimer>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
struct LaboStats {
    int nbProjets;
    int personnel;
    int matriels;
    float depense;
};


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
    void validateFields();
    void  populateResponsableComboBox();


    void rechercherLabo(const QString &searchText);

    void on_comboBox_Tri_currentIndexChanged();
    void onTabWidgetPageChanged(int index);
    void onExcelClicked();
    void populateNomlabComboBox();
    void showMap();
    void on_selectLab_clicked();   // Slot to handle lab selection
    void on_sendMessage_clicked(); // Slot to handle send message button click
    void Read_Data_From_Socket();


   private:
    Ui::MainWindow *ui;
    Laboratoire labTmp;
    void goToStatistiques();

    bool exporterLabsVersExcel(const QString& cheminFichier);


    QMap<QString, LaboStats> fetchLaboStatistics();
    void displayStatisticsChart();

    QTcpSocket *TCPSocket;

    void populateLabNameComboBox();

};
#endif // MAINWINDOW_H
