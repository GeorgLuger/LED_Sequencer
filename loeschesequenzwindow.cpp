#include "loeschesequenzwindow.h"
#include "ui_loeschesequenzwindow.h"

LoescheSequenzWindow::LoescheSequenzWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoescheSequenzWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Lösche Sequenz");

    // QSqlqueryModel anlegen
    sqm = new QSqlQueryModel();

    // Query Abfrage Statement inklusive Tabellen Verknüpfung mittels join
    sqlquery("select Sequenzen.SeqID, Sequenzen.SeqNr, Sequenzen.SeqName, Sequenzen.SeqFolge, \
             Effekte.EffID, Effekte.EffHelligkeit, Effekte.EffPar1, Effekte.EffPar2, \
             Effekte.EffZeitdauer, EffektePresets.EffPresName, EffektePresets.EffPresWLEDNR, \
             Colour.ColID, Colour.ColName, Colour.Col1, Colour.Col2, Colour.Col3, \
             Colour.Col4, Colour.Col5, Colour.Col6, Colour.Col7, Colour.Col8, Colour.Col9 \
             from Sequenzen join Effekte on Sequenzen.SeqEffID = Effekte.EffID \
             join EffektePresets on Effekte.EffEffPresID = EffektePresets.EffPresID \
             join Colour on Effekte.EffColID = Colour.ColID");

    // Werte und Wertebereiche festelegen
    ui->sequenzNrSpinBox->setValue(0);
    ui->sequenzNrSpinBox->setRange(0,0);

    QSqlQuery query_seq;
    query_seq.prepare("SELECT MAX(SeqNr) FROM Sequenzen");
    query_seq.exec();
    query_seq.next();
    int maxSequenz = (query_seq.value(0).toInt());
    if (maxSequenz > 0)
    {
        ui->sequenzNrSpinBox->setValue(1);
        ui->sequenzNrSpinBox->setRange(1,maxSequenz);
        QObject::connect(ui->pushsendeSequenz, SIGNAL(clicked()), SLOT(loescheSequenz()));
    }

}

LoescheSequenzWindow::~LoescheSequenzWindow()
{
    delete ui;
}

// User kann eine Sequenz auswählen und löschen
void LoescheSequenzWindow::loescheSequenz()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Sequenz wirklich loeschen", "Wollen Sie die Sequenz wirklich loeschen?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        int minEffekt = 0, maxEffekt = 0;
        int sequenzNr = 0, erfolgreich = 0;
        sequenzNr = ui->sequenzNrSpinBox->value();

        // Ermitteln der maximalen und minimalen Sequenz Effekt IDs
        QSqlQuery query;
        query.prepare("SELECT MIN(SeqEffID) FROM Sequenzen WHERE SeqNr = :SN");
        query.bindValue(":SN", sequenzNr);
        query.exec();
        query.next();
        minEffekt = (query.value(0).toInt());
        query.prepare("SELECT MAX(SeqEffID) FROM Sequenzen WHERE SeqNr = :SN");
        query.bindValue(":SN", sequenzNr);
        query.exec();
        query.next();
        maxEffekt = (query.value(0).toInt());

        // Vorbereiten und löschen der Einträge in Sequenzen und Effekte
        query.prepare("DELETE FROM Sequenzen WHERE SeqEffID BETWEEN :min_value AND :max_value");
        query.bindValue(":min_value", minEffekt);
        query.bindValue(":max_value", maxEffekt);
        query.exec();
        query.prepare("DELETE FROM Effekte WHERE EffID BETWEEN :min_value AND :max_value");
        query.bindValue(":min_value", minEffekt);
        query.bindValue(":max_value", maxEffekt);
        erfolgreich = query.exec();

        if (erfolgreich)
        {
                // falls die Query erfolgreich war, letzte Änderung übernehmen
                QSqlDatabase::database().commit();
                QMessageBox::information(this, tr("Erfolgreich gelöscht"), tr("Die Sequenz wurde erfolgreich gelöscht."), QMessageBox::Ok);
        }
        else
        {
                // falls die Query NICHT erfolgreich war, letzte Änderung nicht übernehmen
                QSqlDatabase::database().rollback();
                QMessageBox::critical(this, tr("Fehler"), tr("Fehler beim Löschen der Sequenz."), QMessageBox::Ok);
        }

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
    ui->tableViewSequenzen->scrollToBottom();

    return;
}



void LoescheSequenzWindow::sqlquery(QString query)
{
    // Ausfuehren der Datenbank Abfrage
    sqm->setQuery(query);

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
    ui->tableViewSequenzen->scrollToBottom();

    return;
}
