#ifndef LISTESEQUENZENWINDOW_H
#define LISTESEQUENZENWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QSqlQueryModel>

namespace Ui {
class ListeSequenzenWindow;
}

class ListeSequenzenWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ListeSequenzenWindow(QWidget *parent = nullptr);
    ~ListeSequenzenWindow();

private:
    Ui::ListeSequenzenWindow *ui;
    QSqlQueryModel *sqm;
    void sqlquery(QString query); // Anzeige der Sequenzen als Liste
};

#endif // LISTESEQUENZENWINDOW_H
