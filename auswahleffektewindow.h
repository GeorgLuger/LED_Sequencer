#ifndef AUSWAHLEFFEKTEWINDOW_H
#define AUSWAHLEFFEKTEWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <ceffekte.h>
#include <sendeeffektwindow.h>

namespace Ui {
class AuswahlEffekteWindow;
}

class AuswahlEffekteWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AuswahlEffekteWindow(QWidget *parent = nullptr);
    ~AuswahlEffekteWindow();

public slots:
    void sendeEffekt(); // Effekt Daten erzeugen und an Sende Effekt Window übermitteln

private:
    Ui::AuswahlEffekteWindow *ui;
};

#endif // AUSWAHLEFFEKTEWINDOW_H
