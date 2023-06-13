#include "importsqlwindow.h"
#include "ui_importsqlwindow.h"

ImportSQLWindow::ImportSQLWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportSQLWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Import SQL");

    QObject::connect(ui->pushButton, SIGNAL(clicked()), SLOT(importSQL()));
}

ImportSQLWindow::~ImportSQLWindow()
{
    delete ui;
}

// Daten der Tabellen Sequenzen und Effekte werden importiert
void ImportSQLWindow::importSQL()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    QFile file("export.sql");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            QString tableName = parts[0];
            QStringList columnNames = parts.mid(1);
            QString values = "(" + columnNames.join(",") + ")";
            if (tableName == "Effekte" || tableName == "Sequenzen") {
                QString insertQuery = "INSERT INTO " + tableName + " (" + columnNames.join(",") + ") VALUES ";
                line = in.readLine();
                while (!line.isEmpty()) {
                    QStringList rowValues = line.split(",");
                    QString valuesStr = "(" + rowValues.join(",") + ")";
                    query.exec(insertQuery + valuesStr);
                    line = in.readLine();
                    qDebug() << insertQuery + valuesStr;
                    qDebug() << query.lastError().text();
                }
            }
        }
        file.close();
    }
    return;
}
