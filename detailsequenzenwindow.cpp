#include "detailsequenzenwindow.h"
#include "csequenzen.h"
#include "ui_detailsequenzenwindow.h"

DetailSequenzenWindow::DetailSequenzenWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailSequenzenWindow)
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
    ui->sequenzNameLineEdit->setText("Default A");

    this->setWindowTitle("Neue Sequenz erstellen");

    // Fuellen der Combobox mit Effekte Presets
    QSqlQuery query("select * from EffektePresets");
    while(query.next())
    {
        // Fuellen der Combobox "Effekte"
        QString inhalt = query.value(1).toString();
        ui->effektComboBox->addItem(inhalt, query.value(0));
    }

    // Fuellen der Combobox mit Colour Presets
    QSqlQuery query_2("select * from Colour");
    while(query_2.next())
    {
        // Fuellen der Combobox "Colour"
        QString inhalt = query_2.value(1).toString();
        ui->farbeComboBox->addItem(inhalt, query_2.value(0));
    }

    // QSqlqueryModel anlegen
    sqm = new QSqlQueryModel();

    // Startwerte festlegen
    currentSeqNr = 1; // aktuelle Sequenz Nummer
    currentSeqFolge = 1; // aktuelle Nummer innerhalb der Sequenz Folge
    firstEffekt = 0; // ID des ersten Effekts
    lastEffekt = 1;// ID des letzten Effekts
    currentEffID = 1; // momentane Effekt ID

    QSqlQuery query_seq;
    query_seq.prepare("SELECT MAX(SeqNr) FROM Sequenzen");
    query_seq.exec();
    query_seq.next();
    currentSeqNr = (query_seq.value(0).toInt())+1;

    query_seq.prepare("SELECT MAX(SeqEffID) FROM Sequenzen");
    query_seq.exec();
    query_seq.next();
    lastEffekt = (query_seq.value(0).toInt())+1;

    // Query Abfrage Statement inklusive Tabellen Verknüpfung mittels join
    sqlquery("select Sequenzen.SeqID, Sequenzen.SeqNr, Sequenzen.SeqName, Sequenzen.SeqFolge, \
             Effekte.EffID, Effekte.EffHelligkeit, Effekte.EffPar1, Effekte.EffPar2, \
             Effekte.EffZeitdauer, EffektePresets.EffPresName, EffektePresets.EffPresWLEDNR, \
             Colour.ColID, Colour.ColName, Colour.Col1, Colour.Col2, Colour.Col3, \
             Colour.Col4, Colour.Col5, Colour.Col6, Colour.Col7, Colour.Col8, Colour.Col9 \
             from Sequenzen join Effekte on Sequenzen.SeqEffID = Effekte.EffID \
             join EffektePresets on Effekte.EffEffPresID = EffektePresets.EffPresID \
             join Colour on Effekte.EffColID = Colour.ColID");

    QObject::connect(ui->pushsendeEffekt, SIGNAL(clicked()), SLOT(sendeEffekt()));
    QObject::connect(ui->pushaddEffekt, SIGNAL(clicked()), SLOT(addEffekt()));
    QObject::connect(ui->pushentfEffekt, SIGNAL(clicked()), SLOT(entfEffekt()));

}

DetailSequenzenWindow::~DetailSequenzenWindow()
{
    delete ui;
}

// Der aktuell eingestellte Effekt wird an die Sequenz hinzugefügt
void DetailSequenzenWindow::addEffekt()
{
    QSqlQuery query;
    CEffekte EffektWerte;
    // Groesste ID Nummer suchen und mit +1 dem aktuellen Effekt zuweisen
    query.prepare("SELECT MAX(EffID) FROM Effekte");
    query.exec();
    query.next();
    EffektWerte.EffID = (query.value(0).toInt())+1;
    currentEffID = EffektWerte.EffID;

    // Abfragen der Effekt Combobox
    QString searchTerm = ui->effektComboBox->currentText();
    // Suchen des Wertes in der Tabelle und Rückgabe des Wertes
    query.prepare("SELECT * FROM EffektePresets WHERE EffPresName LIKE '%' || :searchTerm || '%'");
    query.bindValue(":searchTerm", searchTerm);
    query.exec();
    query.next();
    EffektWerte.EffEffPresID = query.value(0).toInt();
    EffektWerte.EffPresWLEDNR = query.value(2).toInt();
    // Abfragen der Colour Combobox
    searchTerm = ui->farbeComboBox->currentText();
    // Suchen des Wertes in der Tabelle und Rückgabe des Wertes
    query.prepare("SELECT * FROM Colour WHERE ColName LIKE '%' || :searchTerm || '%'");
    query.bindValue(":searchTerm", searchTerm);
    query.exec();
    query.next();
    EffektWerte.EffColID = query.value(0).toInt();
    // Abfragen der Parameter
    EffektWerte.EffHelligkeit = ui->helligkeitSpinBox->value();
    EffektWerte.EffPar1 = ui->tempoSpinBox->value();
    EffektWerte.EffPar2 = ui->intensitaetSpinBox->value();
    EffektWerte.EffZeitdauer = ui->zeitdauerSpinBox->value();

    // aktuellen Effekt in die Tabelle Effekte hinzufügen
    QSqlQuery insert;
    insert.prepare("insert into Effekte (EffID, EffHelligkeit, EffPar1, EffPar2, EffZeitdauer, EffEffPresID, \
                    EffColID) values (:ID, :Hell, :Par1, :Par2, :Zeit, :EffPresID, :ColID)");
    insert.bindValue(":ID", EffektWerte.EffID);
    insert.bindValue(":Hell", EffektWerte.EffHelligkeit);
    insert.bindValue(":Par1", EffektWerte.EffPar1);
    insert.bindValue(":Par2", EffektWerte.EffPar2);
    insert.bindValue(":Zeit", EffektWerte.EffZeitdauer);
    insert.bindValue(":EffPresID", EffektWerte.EffEffPresID);
    insert.bindValue(":ColID", EffektWerte.EffColID);
    insert.exec();

    // ermitteln der Werte für die Sequenz
    CSequenzen SequenzWerte;
    query.prepare("SELECT MAX(SeqID) FROM Sequenzen");
    query.exec();
    query.next();
    SequenzWerte.SeqID = (query.value(0).toInt())+1;
    SequenzWerte.SeqEffID = EffektWerte.EffID;
    SequenzWerte.SeqNr = currentSeqNr;
    SequenzWerte.SeqFolge = currentSeqFolge;
    currentSeqFolge++;
    SequenzWerte.SeqName = ui->sequenzNameLineEdit->text();

    // aktuelle Werte in die Tabelle Sequenz hinzufpgen
    insert.prepare("insert into Sequenzen (SeqID, SeqNr, SeqName, SeqFolge, SeqEffID) \
                    values (:ID, :Nr, :Name, :Folge, :SeqEffID)");
    insert.bindValue(":ID", SequenzWerte.SeqID);
    insert.bindValue(":Nr", SequenzWerte.SeqNr);
    insert.bindValue(":Name", SequenzWerte.SeqName);
    insert.bindValue(":Folge", SequenzWerte.SeqFolge);
    insert.bindValue(":SeqEffID", SequenzWerte.SeqEffID);
    insert.exec();

    // Update der Tabelle
    sqlquery("select Sequenzen.SeqID, Sequenzen.SeqNr, Sequenzen.SeqName, Sequenzen.SeqFolge, \
             Effekte.EffID, Effekte.EffHelligkeit, Effekte.EffPar1, Effekte.EffPar2, \
             Effekte.EffZeitdauer, EffektePresets.EffPresName, EffektePresets.EffPresWLEDNR, \
             Colour.ColID, Colour.ColName, Colour.Col1, Colour.Col2, Colour.Col3, \
             Colour.Col4, Colour.Col5, Colour.Col6, Colour.Col7, Colour.Col8, Colour.Col9 \
             from Sequenzen join Effekte on Sequenzen.SeqEffID = Effekte.EffID \
             join EffektePresets on Effekte.EffEffPresID = EffektePresets.EffPresID \
             join Colour on Effekte.EffColID = Colour.ColID");

    firstEffekt++;
    lastEffekt = EffektWerte.EffID;

    return;
}

// Der letzte Effekt wird von der Sequenz entfernt
void DetailSequenzenWindow::entfEffekt()
{
    if (firstEffekt != 0) // nur wenn auch ein Effekt hinzugefügt wurde ausführen
    {
        QSqlQuery entfEffekt;
        // löschen des letzten Eintrages aus der Tabelle Sequenzen
        entfEffekt.prepare("delete from Sequenzen where SeqEffID = :Seqid");
        entfEffekt.bindValue(":Seqid", lastEffekt);
        entfEffekt.exec();

        // löschen des letzten Eintrages aus der Tabelle Effekte
        entfEffekt.prepare("delete from Effekte where EffID = :Effid");
        entfEffekt.bindValue(":Effid", currentEffID);
        entfEffekt.exec();

        firstEffekt--;
        lastEffekt--;
        currentSeqFolge--;
        currentEffID--;
    }

    // Update der Tabelle
    sqlquery("select Sequenzen.SeqID, Sequenzen.SeqNr, Sequenzen.SeqName, Sequenzen.SeqFolge, \
             Effekte.EffID, Effekte.EffHelligkeit, Effekte.EffPar1, Effekte.EffPar2, \
             Effekte.EffZeitdauer, EffektePresets.EffPresName, EffektePresets.EffPresWLEDNR, \
             Colour.ColID, Colour.ColName, Colour.Col1, Colour.Col2, Colour.Col3, \
             Colour.Col4, Colour.Col5, Colour.Col6, Colour.Col7, Colour.Col8, Colour.Col9 \
             from Sequenzen join Effekte on Sequenzen.SeqEffID = Effekte.EffID \
             join EffektePresets on Effekte.EffEffPresID = EffektePresets.EffPresID \
             join Colour on Effekte.EffColID = Colour.ColID");

    return;
}

// Effekt Daten erzeugen und an Sende Effekt Window übermitteln
void DetailSequenzenWindow::sendeEffekt()
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


    // Abfragen der Parameter
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

void DetailSequenzenWindow::sqlquery(QString query)
{
    // Ausfuehren der Datenbank Abfrage
    sqm->setQuery(query);

    if (firstEffekt == 0) // Nur beim ersten Aufruf ausführen wegen Ladezeit
    {
        // Header setzen
        // 1. Parameter: Spalte, 2. Parameter: Formatierung, 3. Parameter: Text, der als Spaltenueberschrift dient
        sqm->setHeaderData(0, Qt::Horizontal, tr("SeqID"));
        sqm->setHeaderData(1, Qt::Horizontal, tr("SeqNr"));
        sqm->setHeaderData(2, Qt::Horizontal, tr("SeqName"));
        sqm->setHeaderData(3, Qt::Horizontal, tr("SeqFolge"));
        sqm->setHeaderData(4, Qt::Horizontal, tr("SeqEffID"));
        sqm->setHeaderData(5, Qt::Horizontal, tr("EffHelligkeit"));
        sqm->setHeaderData(6, Qt::Horizontal, tr("EffPar1"));
        sqm->setHeaderData(7, Qt::Horizontal, tr("EffPar2"));
        sqm->setHeaderData(8, Qt::Horizontal, tr("EffZeitdauer"));
        sqm->setHeaderData(9, Qt::Horizontal, tr("PresName"));
        sqm->setHeaderData(10, Qt::Horizontal, tr("WLEDNR"));
        sqm->setHeaderData(11, Qt::Horizontal, tr("ColID"));
        sqm->setHeaderData(12, Qt::Horizontal, tr("ColName"));
        sqm->setHeaderData(13, Qt::Horizontal, tr("Col1"));
        sqm->setHeaderData(14, Qt::Horizontal, tr("Col2"));
        sqm->setHeaderData(15, Qt::Horizontal, tr("Col3"));
        sqm->setHeaderData(16, Qt::Horizontal, tr("Col4"));
        sqm->setHeaderData(17, Qt::Horizontal, tr("Col5"));
        sqm->setHeaderData(18, Qt::Horizontal, tr("Col6"));
        sqm->setHeaderData(19, Qt::Horizontal, tr("Col7"));
        sqm->setHeaderData(20, Qt::Horizontal, tr("Col8"));
        sqm->setHeaderData(21, Qt::Horizontal, tr("Col9"));

        // Zuweisung des Models zur Tableview und Parameter Anpassungen
        ui->tableViewSequenzen->setModel(sqm);
        ui->tableViewSequenzen->setAlternatingRowColors(true);
        ui->tableViewSequenzen->setStyleSheet("QHeaderView::section {background-color:#ffbf00}");
        ui->tableViewSequenzen->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableViewSequenzen->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    ui->tableViewSequenzen->scrollToBottom();
    return;
}
