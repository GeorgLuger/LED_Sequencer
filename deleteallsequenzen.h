#ifndef DELETEALLSEQUENZEN_H
#define DELETEALLSEQUENZEN_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>

namespace Ui {
class DeleteAllSequenzen;
}

class DeleteAllSequenzen : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteAllSequenzen(QWidget *parent = nullptr);
    ~DeleteAllSequenzen();

public slots:
    void dropTables(); // loescht den gesamten Inhalt der Tabellen Sequenzen und Effekte

private:
    Ui::DeleteAllSequenzen *ui;
};

#endif // DELETEALLSEQUENZEN_H
