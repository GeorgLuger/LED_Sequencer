#ifndef DETAILSEQUENZENWINDOW_H
#define DETAILSEQUENZENWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <ceffekte.h>
#include <csequenzen.h>
#include <sendeeffektwindow.h>

namespace Ui {
class DetailSequenzenWindow;
}

class DetailSequenzenWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DetailSequenzenWindow(QWidget *parent = nullptr);
    ~DetailSequenzenWindow();

public slots:
    void sendeEffekt(); // Effekt Daten erzeugen und an Sende Effekt Window übermitteln
    void addEffekt(); // Der aktuell eingestellte Effekt wird an die Sequenz hinzugefügt
    void entfEffekt(); // Der letzte Effekt wird von der Sequenz entfernt

private:
    Ui::DetailSequenzenWindow *ui;
    QSqlQueryModel *sqm;
    void sqlquery(QString query);
    int currentSeqNr; // aktuelle Sequenz Nummer
    int currentSeqFolge; // aktuelle Nummer innerhalb der Sequenz Folge
    int firstEffekt; // ID des ersten Effekts
    int lastEffekt; // ID des letzten Effekts
    int currentEffID; // momentane Effekt ID
};

#endif // DETAILSEQUENZENWINDOW_H
