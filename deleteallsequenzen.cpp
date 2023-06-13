#include "deleteallsequenzen.h"
#include "ui_deleteallsequenzen.h"

DeleteAllSequenzen::DeleteAllSequenzen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteAllSequenzen)
{
    ui->setupUi(this);

    this->setWindowTitle("Alle Sequenzen loeschen");

    QObject::connect(ui->pushButton, SIGNAL(clicked()), SLOT(dropTables()));
}

DeleteAllSequenzen::~DeleteAllSequenzen()
{
    delete ui;
}

// loescht den gesamten Inhalt der Tabellen Sequenzen und Effekte
void DeleteAllSequenzen::dropTables()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Alle Sequenzen loeschen", "Wollen Sie wirklich ALLE Sequenzen loeschen?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) // Nach User Auswahl "Yes" werden die Inhalte von Sequenzen und Effekte gelöscht
    {
        QSqlQuery query;
        query.prepare("DELETE FROM Sequenzen");
        query.exec();
        query.prepare("DELETE FROM Effekte");
        query.exec();
    }

    return;
}
