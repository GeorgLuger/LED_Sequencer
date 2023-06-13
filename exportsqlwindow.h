#ifndef EXPORTSQLWINDOW_H
#define EXPORTSQLWINDOW_H

#include <QDialog>
#include <QTSql>
#include <sqldatabase.h>

namespace Ui {
class ExportSQLWindow;
}

class ExportSQLWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ExportSQLWindow(QWidget *parent = nullptr);
    ~ExportSQLWindow();

public slots:
    void exportSQL(); // Daten der Tabellen Sequenzen und Effekte werden exportiert

private:
    Ui::ExportSQLWindow *ui;
};

#endif // EXPORTSQLWINDOW_H
