#include "SendeSequenzWindow.h"
#include "ui_SendeSequenzWindow.h"

SendeSequenzWindow::SendeSequenzWindow(int SequenzAuswahl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendeSequenzWindow),
    SequenzNr(SequenzAuswahl)
{
    ui->setupUi(this);
    this->setWindowTitle("Senden Sequenz an Microcontroller");

    // Anpassungen am Design der Label Objekte
    ui->label_1->setStyleSheet("background-color: #22ffff;"
                                "border-style: solid;"
                                "border-width: 1px;"
                                "border-color: #c0c0c0;"
                                "padding: 2px;"
                                "font-size: 10pt;"
                                "font-family: Arial;"
                                "color: #000000;");

    ui->label_2->setStyleSheet("background-color: #00ff22;"
                                "border-style: solid;"
                                "border-width: 1px;"
                                "border-color: #c0c0c0;"
                                "padding: 2px;"
                                "font-size: 10pt;"
                                "font-family: Arial;"
                                "color: #000000;");

    // Anpassungen am Design der Buttons
    ui->pushSenden->setStyleSheet("background-color:#00ff00");
    ui->pushAnhalten->setStyleSheet("background-color:#FF7C80");

    // Setzen der Standardwerte der Objekte
    ui->label_1->setText("waiting for user input");
    ui->label_2->setText("waiting for user input");

    QString resultIP = scanNetworkESP(); // Scannt das Netzwerk nach aktiven ESP8266 Microcontroller

    // Eintrag der gefundenen IP in das IP Adresse Line Edit Objekt
    ui->IPAdresseLineEdit->setText("http://" +resultIP + "/json");
    ui->IPAdresseLineEdit->setReadOnly(1);

    SequenzNr = SequenzAuswahl;
    QSqlQuery query;
    query.prepare("SELECT MAX(SeqFolge) FROM Sequenzen WHERE SeqNr = :SN");
    query.bindValue(":SN", SequenzNr);
    query.exec();
    query.next();
    maxFolge = (query.value(0).toInt()); // Anzahl der gesamten Effekte der Sequenz
    currentFolge = 1; // Nummer des aktuellen Effekts der Abfolge
    stopSignal = 1; // Stopsignal

    QObject::connect(ui->pushSenden, SIGNAL(clicked()), SLOT(setstartSignal()));
    QObject::connect(ui->pushSenden, SIGNAL(clicked()), SLOT(executeSequenz()));
    QObject::connect(ui->pushAnhalten, SIGNAL(clicked()), SLOT(anhaltenSenden()));
}


SendeSequenzWindow::~SendeSequenzWindow()
{
    delete ui;
}

// Sequenz wird gestartet
void SendeSequenzWindow::setstartSignal()
{
    stopSignal = 0;
    currentFolge = 1;
}

// Sequenz wird durch User oder Programm gestoppt
void SendeSequenzWindow::setstopSignal()
{
    stopSignal = 1;
    currentFolge = 1;
}

// Sequenz wird Effekt für Effekt abgearbeitet
void SendeSequenzWindow::executeSequenz()
{

    if (currentFolge <= maxFolge && stopSignal == 0)
    {
        erzeugeEffektDaten();
    }

    return;
}

// Erzeugung der Effekt Daten auf Basis der SQL Daten
void SendeSequenzWindow::erzeugeEffektDaten()
{
    QSqlQuery query_seq;
    CEffekte m_EffektWerte;
    // Abfrage der SQL Datenbank des aktuellen Effekts
    query_seq.prepare("SELECT Sequenzen.SeqID, Sequenzen.SeqNr, Sequenzen.SeqName, Sequenzen.SeqFolge, \
             Effekte.EffID, Effekte.EffHelligkeit, Effekte.EffPar1, Effekte.EffPar2, \
             Effekte.EffZeitdauer, EffektePresets.EffPresName, EffektePresets.EffPresWLEDNR, \
             Colour.ColID, Colour.ColName, Colour.Col1, Colour.Col2, Colour.Col3, \
             Colour.Col4, Colour.Col5, Colour.Col6, Colour.Col7, Colour.Col8, Colour.Col9 \
             from Sequenzen join Effekte on Sequenzen.SeqEffID = Effekte.EffID \
             join EffektePresets on Effekte.EffEffPresID = EffektePresets.EffPresID \
             join Colour on Effekte.EffColID = Colour.ColID \
             WHERE Sequenzen.SeqNr = :SN AND Sequenzen.SeqFolge = :SF");
    query_seq.bindValue(":SN", SequenzNr); // Vom User ausgewählte Sequenz Nummer
    query_seq.bindValue(":SF", currentFolge);
    query_seq.exec();
    query_seq.next();

    // Zuweisung der Werte zum m_EffektWerte Objekt
    m_EffektWerte.EffID = query_seq.value(4).toInt();
    m_EffektWerte.EffPresName = query_seq.value(9).toString();
    m_EffektWerte.EffPresWLEDNR = query_seq.value(10).toInt();
    m_EffektWerte.EffHelligkeit = query_seq.value(5).toInt();
    m_EffektWerte.EffPar1 = query_seq.value(6).toInt();
    m_EffektWerte.EffPar2 = query_seq.value(7).toInt();
    m_EffektWerte.EffZeitdauer = query_seq.value(8).toInt();
    m_EffektWerte.EffEffPresID = 0; // nicht in Benutzung für JSON Erzeugung
    m_EffektWerte.EffColID = query_seq.value(11).toInt();

    startSenden(m_EffektWerte); // Ausführen des Senden des aktuellen Effekt

    // Timer damit nächster Effekt erst nach der vom User bestimmten Zeitdauer gesendet wird
    QTimer *timer_folge = new QTimer(this);
    connect(timer_folge, &QTimer::timeout, this, &SendeSequenzWindow::executeSequenz);
    timer_folge->start(m_EffektWerte.EffZeitdauer*1000);

    connect(timer_folge, &QTimer::timeout, timer_folge, &QTimer::deleteLater);

    currentFolge++; // Nächster Effekt in der Folge wird ausgewählt
}

// Starten das Senden der JSON Daten an den Microcontroller
void SendeSequenzWindow::startSenden(CEffekte EffektWerte)
{
    // Erzeugung des JSON Documents auf Basis der Effektwerte
    QJsonDocument jsonData;
    jsonData = EffektWerte.erzeugeJSON(EffektWerte);
    QByteArray byteData = jsonData.toJson();

    ui->label_1->setText(jsonData.toJson(QJsonDocument::Compact));
    QUrl url(ui->IPAdresseLineEdit->text()); // Zuweisung der IP Adresse
    QNetworkRequest request(url);
    ui->label_2->setText(url.toString());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Ausführen des HTTP Post Requests
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, byteData);

    // Auswertung der HTTP Post Request Antwort
    QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray responseData = reply->readAll();
        reply->deleteLater();
        QString label_3_Data = QString::fromUtf8(responseData);
        ui->label_2->setText(label_3_Data);
        manager->deleteLater();
    });

    // Erzeuge Timer Window Fenster und öffne das Fenster
    TimerWindow* Timer_Window = new TimerWindow(EffektWerte,this);
    Timer_Window->setWindowModality(Qt::WindowModal);
    Timer_Window->show();

    // Setze einen Timer der nach x Sekunden das Fenster schliesst
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, Timer_Window, &TimerWindow::close);
    timer->start(EffektWerte.EffZeitdauer*1000);

    // Verwendung von deleteLater um ein sicheres löschen der Objekte zu ermöglichen
    connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::timeout, Timer_Window, &TimerWindow::deleteLater);

    return;
}

// Deaktiviert die LED mittels Helligkeit = 0
void SendeSequenzWindow::anhaltenSenden()
{
    setstopSignal(); // Sequenz stoppen

    // Erzeugung des JSON Documents auf Basis der Effektwerte
    QJsonDocument jsonData;
    CEffekte EffektWerte;
    EffektWerte.EffID = 1;
    EffektWerte.EffPresName = "Solid";
    EffektWerte.EffPresWLEDNR = 1;
    EffektWerte.EffHelligkeit = 0; // Deaktivierung der LEDs durch Helligkeit setzen auf 0
    EffektWerte.EffPar1 = 1;
    EffektWerte.EffPar2 = 1;
    EffektWerte.EffZeitdauer = 1;
    EffektWerte.EffEffPresID = 1;
    EffektWerte.EffColID = 1;

    jsonData = EffektWerte.erzeugeJSON(EffektWerte);
    QJsonObject jsonObj = jsonData.object();
    jsonObj["bri"] = 0; // Deaktivierung der LEDs durch Helligkeit setzen auf 0
    jsonData.setObject(jsonObj);
    QByteArray byteData = jsonData.toJson();

    ui->label_1->setText(jsonData.toJson(QJsonDocument::Compact));
    QUrl url(ui->IPAdresseLineEdit->text()); // Zuweisung der IP Adresse
    QNetworkRequest request(url);
    ui->label_2->setText(url.toString());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Ausführen des HTTP Post Requests
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, byteData);

    // Auswertung der HTTP Post Request Antwort
    QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray responseData = reply->readAll();
        reply->deleteLater();
        QString label_3_Data = QString::fromUtf8(responseData);
        ui->label_2->setText(label_3_Data);
        manager->deleteLater();
    });

    return;
}

// Scannt das Netzwerk nach aktiven ESP8266 Microcontroller
QString SendeSequenzWindow::scanNetworkESP()
{
    QProcess myProcess;
    myProcess.start("arp", QStringList() << "-a");
    myProcess.waitForStarted();
    myProcess.closeWriteChannel();
    myProcess.waitForFinished();

    QByteArray result = myProcess.readAll();
    QString scannedIP;

    if (result.length() > 0)
    {
        QString resultStr = QString(result);
        QStringList list = resultStr.split(QString("\r\n"));

        for (const QString& entry : list)
        {
            if (entry.contains("dynamisch")) {
                QStringList line = entry.trimmed().split(" ");
                if (line[0] == "192.168.0.39") // add ESP8266 Network Check HERE
                {
                    scannedIP = line[0];
                }
            }
        }
    }
    return scannedIP;
}
