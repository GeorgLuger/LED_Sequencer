#include "exportsqlwindow.h"
#include "ui_exportsqlwindow.h"

ExportSQLWindow::ExportSQLWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportSQLWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Export SQL");

    QObject::connect(ui->pushButton, SIGNAL(clicked()), SLOT(exportSQL()));
}

ExportSQLWindow::~ExportSQLWindow()
{
    delete ui;
}

// Daten der Tabellen Sequenzen und Effekte werden exportiert
void ExportSQLWindow::exportSQL()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    QFile file("export.sql");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        QStringList tablesToExport = { "Sequenzen", "Effekte" };
        for (const QString &tableName : tablesToExport) {
            query.exec("SELECT * FROM " + tableName);
            QSqlRecord record = query.record();
            int columnCount = record.count();
            QString columnNames;
            for (int i = 0; i < columnCount; ++i) {
                if (i != 0) {
                    columnNames += ",";
                }
                columnNames += record.fieldName(i);
            }
            out << tableName << "," << columnNames << "\n";
            while (query.next()) {
                QString values;
                for (int i = 0; i < columnCount; ++i) {
                    if (i != 0) {
                        values += ",";
                    }
                    values += query.value(i).toString();
                }
                out << tableName << "," << values << "\n";
            }
        }
        file.close();
    }
    return;
}
