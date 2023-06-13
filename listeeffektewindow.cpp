#include "listeeffektewindow.h"
#include "ui_listeeffektewindow.h"

ListeEffekteWindow::ListeEffekteWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListeEffekteWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Liste Effekte Presets");

    rtm = new QSqlRelationalTableModel();
    displayTable();
}

ListeEffekteWindow::~ListeEffekteWindow()
{
    delete ui;
}

// Anzeige der Effekte als Liste
void ListeEffekteWindow::displayTable()
{
    // hier beim QSqlRelationalTableModel wird nur der Name der Tabelle übergeben
    // QT generiert von sich aus die notwendingen SQL-Statements
    rtm->setTable("EffektePresets");
    // Spaltenüberschriften
    rtm->setHeaderData(0, Qt::Horizontal, QObject::tr("EffPresID"));
    rtm->setHeaderData(1, Qt::Horizontal, QObject::tr("EffPresName"));
    rtm->setHeaderData(2, Qt::Horizontal, QObject::tr("EffPresWLEDNR"));
    // mit dem Selekt-Befehl erfolgt der DB Zugriff
    rtm->select();
    rtm->sort(1, Qt::AscendingOrder);

    // Zuweisung des Models zur Tableview und Parameter Anpassungen
    ui->tableViewEffekte->setModel(rtm);
    // Änderungen in die Datenbank übernehmen
    ui->tableViewEffekte->setItemDelegate(new QSqlRelationalDelegate(rtm));
    ui->tableViewEffekte->setAlternatingRowColors(true);
    ui->tableViewEffekte->setStyleSheet("QHeaderView::section {background-color:#00ffff}");
    ui->tableViewEffekte->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewEffekte->setEditTriggers(QAbstractItemView::NoEditTriggers);

    return;
}
