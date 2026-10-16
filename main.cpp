#include "mainwindow.h"
#include"MyMemory.h"
#include"database.h"
#include"profil.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/logo.png"));

    DataBase database;

    MainWindow w1;
    w1.show();
    //Interface_user w1("IBrahim@tdu.com") ;
    // Interface_user w1("brahim@etu.com") ;
    //w1.show();

    return app.exec();

}
