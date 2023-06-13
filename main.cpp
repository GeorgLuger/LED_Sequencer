#include "mainwindow.h"
#include "sqldatabase.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!createConnection()) // Aufbau der Verbindung zur SQL Datenbank
    {
        return EXIT_FAILURE;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
