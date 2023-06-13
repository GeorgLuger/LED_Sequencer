#ifndef IMPORTSQLWINDOW_H
#define IMPORTSQLWINDOW_H

#include <QDialog>
#include <QTSql>

namespace Ui {
class ImportSQLWindow;
}

class ImportSQLWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ImportSQLWindow(QWidget *parent = nullptr);
    ~ImportSQLWindow();

public slots:
    void importSQL(); // Daten der Tabellen Sequenzen und Effekte werden importiert

private:
    Ui::ImportSQLWindow *ui;
};

#endif // IMPORTSQLWINDOW_H
