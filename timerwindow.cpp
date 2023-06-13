#include "timerwindow.h"
#include "ui_timerwindow.h"

TimerWindow::TimerWindow(CEffekte EffektWerte, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimerWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Timer");

    // Anpassungen am Design der Label Objekte
    ui->label_1->setStyleSheet("background-color: #22ffff;"
                                "border-style: solid;"
                                "border-width: 1px;"
                                "border-color: #c0c0c0;"
                                "padding: 2px;"
                                "font-size: 10pt;"
                                "font-family: Arial;"
                                "color: #000000;");

    ui->label_2->setStyleSheet("background-color: #ffbf00;"
                                "border-style: solid;"
                                "border-width: 1px;"
                                "border-color: #c0c0c0;"
                                "padding: 2px;"
                                "font-size: 10pt;"
                                "font-family: Arial;"
                                "color: #000000;");

    ui->progressBar->setRange(0, 100);

    // Anzeige der Werte des aktuellen Effekts mittels JSON
    QJsonDocument jsonData;
    jsonData = EffektWerte.erzeugeJSON(EffektWerte);
    ui->label_1->setText(jsonData.toJson(QJsonDocument::Compact));
    ui->label_2->setText("Zeitdauer des aktuellen Effekts in [s]: " + QString::number(EffektWerte.EffZeitdauer));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TimerWindow::updateProgress);

    timer->start(100); // Aktualsierung alle 100 Millisekunden
    startTime = QDateTime::currentDateTime();
    totalDuration = EffektWerte.EffZeitdauer * 1000; // Gesamtdauer setzen
}

TimerWindow::~TimerWindow()
{
    delete ui;
}

 // Aktualisierung der Progress Bar
void TimerWindow::updateProgress()
{
    int elapsed = startTime.msecsTo(QDateTime::currentDateTime()); // verstrichene Zeit
    int progress = ((elapsed * 100) / totalDuration); // Fortschritt

    ui->progressBar->setValue(progress); // Aktualiserung der Progress Bar UI

    if (elapsed >= totalDuration) // Nach Ablauf der Gesamtzeit ausführen
    {
        timer->stop();
        timer->deleteLater();
        close();
    }
}

// Routine bei Timer Ende
void TimerWindow::closeEvent(QCloseEvent *event)
{
    if (timer->isActive()) {
        timer->stop();
        timer->deleteLater();
    }
    QDialog::closeEvent(event);
}
