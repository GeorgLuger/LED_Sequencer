#ifndef LOESCHESEQUENZWINDOW_H
#define LOESCHESEQUENZWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class LoescheSequenzWindow;
}

class LoescheSequenzWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoescheSequenzWindow(QWidget *parent = nullptr);
    ~LoescheSequenzWindow();

public slots:
    void loescheSequenz(); // User kann eine Sequenz auswählen und löschen

private:
    Ui::LoescheSequenzWindow *ui;
    QSqlQueryModel *sqm;
    void sqlquery(QString query);
};

#endif // LOESCHESEQUENZWINDOW_H
