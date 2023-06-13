#ifndef SENDEEFFEKTWINDOW_H
#define SENDEEFFEKTWINDOW_H

#include <QDialog>
#include <ceffekte.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <timerwindow.h>

namespace Ui {
class SendeEffektWindow;
}

class SendeEffektWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SendeEffektWindow(CEffekte EffektWerte, QWidget *parent = nullptr);
    ~SendeEffektWindow();

public slots:
    void startSenden(); // Starten das Senden der JSON Daten an den Microcontroller
    void anhaltenSenden(); // Deaktiviert die LED mittels Helligkeit = 0
    QString scanNetworkESP(); // Scannt das Netzwerk nach aktiven ESP8266 Microcontroller

private:
    Ui::SendeEffektWindow *ui;
    CEffekte m_EffektWerte;
};

#endif // SENDEEFFEKTWINDOW_H
