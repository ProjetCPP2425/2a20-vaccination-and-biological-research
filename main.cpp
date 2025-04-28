#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // ➡️ Changer en Light Mode
 /*   a.setStyle("Fusion");

    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, QColor(225, 225, 225));
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::BrightText, Qt::red);
    lightPalette.setColor(QPalette::Link, QColor(0, 0, 255));
    lightPalette.setColor(QPalette::Highlight, QColor(76, 163, 224));
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);

    a.setPalette(lightPalette);
*/
    Connection c;
    bool test = c.createconnect(); // ✅ ouvrir la base D’ABORD

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("✅ Base ouverte"),
                                 QObject::tr("Connexion réussie.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);

        MainWindow w; // ✅ Ensuite on lance la fenêtre
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("❌ Base non ouverte"),
                              QObject::tr("Échec de connexion.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        return -1;
    }
}
