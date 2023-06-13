#ifndef LISTEEFFEKTEWINDOW_H
#define LISTEEFFEKTEWINDOW_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class ListeEffekteWindow;
}

class ListeEffekteWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ListeEffekteWindow(QWidget *parent = nullptr);
    ~ListeEffekteWindow();

private:
    Ui::ListeEffekteWindow *ui;
    QSqlRelationalTableModel *rtm;
    void displayTable(); // Anzeige der Effekte als Liste
};

#endif // LISTEEFFEKTEWINDOW_H
