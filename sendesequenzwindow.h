#ifndef SENDESEQUENZWINDOW_H
#define SENDESEQUENZWINDOW_H

#include <QDialog>
#include <ceffekte.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <timerwindow.h>
#include <QSqlQueryModel>
#include <QtSql>

namespace Ui {
class SendeSequenzWindow;
}

class SendeSequenzWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SendeSequenzWindow(int SequenzAuswahl, QWidget *parent = nullptr);
    ~SendeSequenzWindow();

public slots:
    void startSenden(CEffekte EffektWerte); // Starten das Senden der JSON Daten an den Microcontroller
    void anhaltenSenden(); // Deaktiviert die LED mittels Helligkeit = 0
    QString scanNetworkESP(); // Scannt das Netzwerk nach aktiven ESP8266 Microcontroller
    void executeSequenz(); // Sequenz wird Effekt für Effekt abgearbeitet
    void erzeugeEffektDaten(); // Erzeugung der Effekt Daten auf Basis der SQL Daten
    void setstartSignal(); // Sequenz wird gestartet
    void setstopSignal(); // Sequenz wird durch User oder Programm gestoppt

private:
    Ui::SendeSequenzWindow *ui;
    int SequenzNr; // Vom User ausgewählte Sequenz Nummer
    int maxFolge; // Anzahl der gesamten Effekte der Sequenz
    int currentFolge; // Nummer des aktuellen Effekts der Abfolge
    int stopSignal; // Stopsignal
};

#endif // SENDESEQUENZWINDOW_H
