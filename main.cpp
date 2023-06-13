#include "mainwindow.h"
#include "sqldatabase.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!createConnection()) // Establishing the connection to the SQL database
    {
        return EXIT_FAILURE;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
