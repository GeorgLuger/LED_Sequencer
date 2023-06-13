#ifndef AUSWAHLSEQUENZWINDOW_H
#define AUSWAHLSEQUENZWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QSqlQueryModel>
#include <sendesequenzwindow.h>

namespace Ui {
class AuswahlSequenzWindow;
}

class AuswahlSequenzWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AuswahlSequenzWindow(QWidget *parent = nullptr);
    ~AuswahlSequenzWindow();

public slots:
    void sendeSequenz(); // User kann eine Sequenz auswählen und an Sende Sequenz Window übermitteln

private:
    Ui::AuswahlSequenzWindow *ui;
    QSqlQueryModel *sqm;
    void sqlquery(QString query);
};

#endif // AUSWAHLSEQUENZWINDOW_H
