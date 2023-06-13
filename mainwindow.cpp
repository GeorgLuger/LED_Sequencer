#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sqm = new QSqlQueryModel();

    // Zuweisung der Action Felder des Menüs zu den Funktionen(Fenster Erzeugung)
    QObject::connect(ui->actionListe_Effekte, SIGNAL(triggered()), SLOT(Liste_Effekte()));
    QObject::connect(ui->actionSende_Effekte, SIGNAL(triggered()), SLOT(Auswahl_Effekte()));
    QObject::connect(ui->actionListe_Sequenzen, SIGNAL(triggered()), SLOT(Liste_Sequenzen()));
    QObject::connect(ui->actionNeue_Sequenz, SIGNAL(triggered()), SLOT(Detail_Sequenzen()));
    QObject::connect(ui->actionSende_Sequenz, SIGNAL(triggered()), SLOT(Auswahl_Sequenzen()));
    QObject::connect(ui->actionLoesche_Sequenz, SIGNAL(triggered()), SLOT(Loesche_Sequenzen()));
    QObject::connect(ui->actionDelete_All_Sequenzen, SIGNAL(triggered()), SLOT(delete_all_Sequenzen()));
    QObject::connect(ui->actionExport_Sequenzen, SIGNAL(triggered()), SLOT(exportieren_SQL()));
    QObject::connect(ui->actionImport_Sequenzen, SIGNAL(triggered()), SLOT(importieren_SQL()));
    QObject::connect(ui->actionProgramm_Beenden, SIGNAL(triggered()), SLOT(Programm_Beenden()));
    QObject::connect(ui->actionHilfoeffnen, SIGNAL(triggered()), SLOT(Hilfe_Oeffnen()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete sqm;
}

 // Öffnet Fenster der Effekt Liste
void MainWindow::Liste_Effekte()
{
    ListeEffekteWindow Liste_Effekte_Window(this); // neues Fenster anlegen
    Liste_Effekte_Window.setWindowModality(Qt::ApplicationModal);
    Liste_Effekte_Window.show(); // neues Fenster anzeigen
    Liste_Effekte_Window.exec(); // neues Fenster ausführen
    return;
}

 // Öffnet Fenster der Effekt Auswahl
void MainWindow::Auswahl_Effekte()
{
    AuswahlEffekteWindow Detail_Effekte_Window(this); // neues Fenster anlegen
    Detail_Effekte_Window.setWindowModality(Qt::ApplicationModal);
    Detail_Effekte_Window.show(); // neues Fenster anzeigen
    Detail_Effekte_Window.exec(); // neues Fenster ausführen
    return;
}

 // Öffnet Fenster der Sequenzen Liste
void MainWindow::Liste_Sequenzen()
{
    ListeSequenzenWindow Liste_Sequenzen_Window(this); // neues Fenster anlegen
    Liste_Sequenzen_Window.setWindowModality(Qt::ApplicationModal);
    Liste_Sequenzen_Window.show(); // neues Fenster anzeigen
    Liste_Sequenzen_Window.exec(); // neues Fenster ausführen
    return;
}

// Öffnet Fenster zur Erstellung einer Sequenz
void MainWindow::Detail_Sequenzen()
{
    DetailSequenzenWindow Detail_Sequenzen_Window(this); // neues Fenster anlegen
    Detail_Sequenzen_Window.setWindowModality(Qt::ApplicationModal);
    Detail_Sequenzen_Window.show(); // neues Fenster anzeigen
    Detail_Sequenzen_Window.exec(); // neues Fenster ausführen
    return;
}

// Öffnet Fenster zum Auswählen und Senden einer Sequenz
void MainWindow::Auswahl_Sequenzen()
{
    AuswahlSequenzWindow Auswahl_Sequenzen_Window(this); // neues Fenster anlegen
    Auswahl_Sequenzen_Window.setWindowModality(Qt::ApplicationModal);
    Auswahl_Sequenzen_Window.show(); // neues Fenster anzeigen
    Auswahl_Sequenzen_Window.exec(); // neues Fenster ausführen
    return;
}

// Öffnet Fenster zum Löschen einer Sequenz
void MainWindow::Loesche_Sequenzen()
{
    LoescheSequenzWindow Loesche_Sequenzen_Window(this); // neues Fenster anlegen
    Loesche_Sequenzen_Window.setWindowModality(Qt::ApplicationModal);
    Loesche_Sequenzen_Window.show(); // neues Fenster anzeigen
    Loesche_Sequenzen_Window.exec(); // neues Fenster ausführen
    return;

}

// Öffnet Fenster zum Exportieren der SQL Tabellen Sequenzen und Effekte
void MainWindow::exportieren_SQL()
{
    ExportSQLWindow Export_SQL_Window(this);
    Export_SQL_Window.setWindowModality(Qt::ApplicationModal);
    Export_SQL_Window.show(); // neues Fenster anzeigen
    Export_SQL_Window.exec(); // neues Fenster ausführen
}

// Öffnet Fenster zum Importieren der SQL Tabellen Sequenzen und Effekte
void MainWindow::importieren_SQL()
{
    ImportSQLWindow Import_SQL_Window(this);
    Import_SQL_Window.setWindowModality(Qt::ApplicationModal);
    Import_SQL_Window.show(); // neues Fenster anzeigen
    Import_SQL_Window.exec(); // neues Fenster ausführen
}

// Öffnet ein Fenster zum Löschen aller aktuellen Sequenzen
void MainWindow::delete_all_Sequenzen()
{
    DeleteAllSequenzen Delete_All_Sequenzen(this);
    Delete_All_Sequenzen.setWindowModality(Qt::ApplicationModal);
    Delete_All_Sequenzen.show(); // neues Fenster anzeigen
    Delete_All_Sequenzen.exec(); // neues Fenster ausführen
}

// Beendet das Programm
void MainWindow::Programm_Beenden()
{
    close();
    return;
}

// Öffnet das Hilfe PDF
void MainWindow::Hilfe_Oeffnen()
{
    QString pdfFilePath = QApplication::applicationDirPath() + "/Hilfe_Pdf_LED_Sequenzer.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(pdfFilePath));
    return;
}

