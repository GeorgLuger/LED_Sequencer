#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QString>
#include <QTableView>
#include <QSqlQueryModel>
#include <QTabWidget>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include <listeeffektewindow.h>
#include <auswahleffektewindow.h>
#include <listesequenzenwindow.h>
#include <detailsequenzenwindow.h>
#include <auswahlsequenzwindow.h>
#include <loeschesequenzwindow.h>
#include <deleteallsequenzen.h>
#include <exportsqlwindow.h>
#include <importsqlwindow.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void Liste_Effekte(); // Öffnet Fenster der Effekt Liste
    void Auswahl_Effekte(); // Öffnet Fenster der Effekt Auswahl
    void Liste_Sequenzen(); // Öffnet Fenster der Sequenzen Liste
    void Detail_Sequenzen(); // Öffnet Fenster zur Erstellung einer Sequenz
    void Auswahl_Sequenzen(); // Öffnet Fenster zum Auswählen und Senden einer Sequenz
    void Loesche_Sequenzen(); // Öffnet Fenster zum Löschen einer Sequenz
    void exportieren_SQL(); // Öffnet Fenster zum Exportieren der SQL Tabellen Sequenzen und Effekte
    void importieren_SQL(); // Öffnet Fenster zum Importieren der SQL Tabellen Sequenzen und Effekte
    void delete_all_Sequenzen(); // Öffnet ein Fenster zum Löschen aller aktuellen Sequenzen
    void Programm_Beenden(); // Beendet das Programm
    void Hilfe_Oeffnen(); // Öffnet das Hilfe PDF

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *sqm;
    void sqlquery(QString query);
};
#endif // MAINWINDOW_H
