#include "auswahleffektewindow.h"
#include "ui_auswahleffektewindow.h"

AuswahlEffekteWindow::AuswahlEffekteWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuswahlEffekteWindow)
{
    ui->setupUi(this);
    // Wertebereiche und Standardwerte festlegen
    ui->helligkeitSpinBox->setRange(0,255);
    ui->helligkeitSpinBox->setValue(30);
    ui->tempoSpinBox->setRange(0,255);
    ui->tempoSpinBox->setValue(10);
    ui->intensitaetSpinBox->setRange(0,255);
    ui->intensitaetSpinBox->setValue(120);
    ui->zeitdauerSpinBox->setRange(0,3600);
    ui->zeitdauerSpinBox->setValue(10);

    this->setWindowTitle("Auswahl Effekt");

    // Fuellen der Combobox mit Effekte Presets
    QSqlQuery query("select * from EffektePresets");
    while(query.next())
    {
        // Fuellen der Combobox "Effekte"
        QString inhalt = query.value(1).toString();
        ui->effektComboBox->addItem(inhalt, query.value(0));
    }

    // Fuellen der Combobox mit Farb Presets
    QSqlQuery query_2("select * from Colour");
    while(query_2.next())
    {
        // Fuellen der Combobox "Colour"
        QString inhalt = query_2.value(1).toString();
        ui->farbeComboBox->addItem(inhalt, query_2.value(0));
    }

    QObject::connect(ui->pushsendeEffekt, SIGNAL(clicked()), SLOT(sendeEffekt()));

}

AuswahlEffekteWindow::~AuswahlEffekteWindow()
{
    delete ui;
}

// Effekt Daten erzeugen und an Sende Effekt Window übermitteln
void AuswahlEffekteWindow::sendeEffekt()
{
    CEffekte EffektWerte;

    // Abfragen der Effekt Combobox
    QString searchTerm = ui->effektComboBox->currentText();
    // Suchen des Wertes in der Tabelle und Rückgabe des Wertes
    QSqlQuery query;
    query.prepare("SELECT * FROM EffektePresets WHERE EffPresName LIKE '%' || :searchTerm || '%'");
    query.bindValue(":searchTerm", searchTerm);
    query.exec();
    query.next();
    EffektWerte.EffPresWLEDNR = query.value(2).toInt();

    // Abfragen der Colour Combobox
    searchTerm = ui->farbeComboBox->currentText();
    // Suchen des Wertes in der Tabelle und Rückgabe des Wertes
    query.prepare("SELECT * FROM Colour WHERE ColName LIKE '%' || :searchTerm || '%'");
    query.bindValue(":searchTerm", searchTerm);
    query.exec();
    query.next();
    EffektWerte.EffColID = query.value(0).toInt();

    // Abfragen der restlichen Effekte Parameter
    EffektWerte.EffHelligkeit = ui->helligkeitSpinBox->value();
    EffektWerte.EffPar1 = ui->tempoSpinBox->value();
    EffektWerte.EffPar2 = ui->intensitaetSpinBox->value();
    EffektWerte.EffZeitdauer = ui->zeitdauerSpinBox->value();

    SendeEffektWindow Sende_Effekte_Window(EffektWerte, this); // neues Fenster anlegen
    Sende_Effekte_Window.setWindowModality(Qt::ApplicationModal);
    Sende_Effekte_Window.show(); // neues Fenster anzeigen
    Sende_Effekte_Window.exec(); // neues Fenster ausführen

    return;
}

