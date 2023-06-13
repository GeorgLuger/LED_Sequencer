#include "sendeeffektwindow.h"
#include "ui_sendeeffektwindow.h"

SendeEffektWindow::SendeEffektWindow(CEffekte EffektWerte, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendeEffektWindow),
    m_EffektWerte(EffektWerte)
{
    ui->setupUi(this);

    this->setWindowTitle("Senden Effekt an Microcontroller");

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

    QObject::connect(ui->pushSenden, SIGNAL(clicked()), SLOT(startSenden()));
    QObject::connect(ui->pushAnhalten, SIGNAL(clicked()), SLOT(anhaltenSenden()));
}


SendeEffektWindow::~SendeEffektWindow()
{
    delete ui;
}

// Starten das Senden der JSON Daten an den Microcontroller
void SendeEffektWindow::startSenden()
{
    // Erzeugung des JSON Documents auf Basis der Effektwerte
    QJsonDocument jsonData;
    jsonData = m_EffektWerte.erzeugeJSON(m_EffektWerte);
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
    TimerWindow* Timer_Window = new TimerWindow(m_EffektWerte,this);
    Timer_Window->setWindowModality(Qt::WindowModal);
    Timer_Window->show();

    // Setze einen Timer der nach x Sekunden das Fenster schliesst
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, Timer_Window, &TimerWindow::close);
    timer->start(m_EffektWerte.EffZeitdauer*1000);

    // Verwendung von deleteLater um ein sicheres löschen der Objekte zu ermöglichen
    connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::timeout, Timer_Window, &TimerWindow::deleteLater);

    return;
}

// Deaktiviert die LED mittels Helligkeit = 0
void SendeEffektWindow::anhaltenSenden()
{
    // Erzeugung des JSON Documents auf Basis der Effektwerte
    QJsonDocument jsonData;
    jsonData = m_EffektWerte.erzeugeJSON(m_EffektWerte);
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
QString SendeEffektWindow::scanNetworkESP()
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
