#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

static bool createConnection()
{
    // qDebug() << QSqlDatabase::drivers(); // Liste der Treiber
    // Name des Treibers für SQL-Lite Datenbanken ist QSQLITE
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // Name der Datenbank ist LED_Sequenzen.db
    db.setDatabaseName("LED_Sequenzen.db");
    if (!db.open())
    {
        // Fehler, Datenbank kann nicht geoffnet werden
        QMessageBox msg;
        msg.setText("Oeffnen der Datenbank LED_Sequenzen.db nicht moeglich!");
        msg.setWindowTitle("Fehler");
        msg.addButton("Ok", QMessageBox::YesRole);
        msg.exec();
        return false;
    }

    // Oeffnen OK => Erstellen der Datenbank
    QSqlQuery query;
        query.exec("PRAGMA foreign_keys = ON");
        query.exec("drop table Colour");
        query.exec("drop table EffektePresets");

        // Datenbank Tabelle Colour erstellen
        query.exec("CREATE TABLE IF NOT EXISTS 'Colour' ( \
                   'ColID' INTEGER NOT NULL, \
                   'ColName' TEXT NOT NULL, \
                   'Col1' INTEGER NOT NULL, \
                   'Col2' INTEGER NOT NULL, \
                   'Col3' INTEGER NOT NULL, \
                   'Col4' INTEGER NOT NULL, \
                   'Col5' INTEGER NOT NULL, \
                   'Col6' INTEGER NOT NULL, \
                   'Col7' INTEGER NOT NULL, \
                   'Col8' INTEGER NOT NULL, \
                   'Col9' INTEGER NOT NULL, \
                   PRIMARY KEY('ColID' AUTOINCREMENT) \
               );");

        // Tabelle Colour mit Farb Presets befüllen
        query.exec("insert into Colour values(1, 'Grün - Blau - Türkis', 100, 255, 100, 0, 155, 255, 0, 255, 200)");
        query.exec("insert into Colour values(2, 'Rot - Pink - Blau', 255, 80, 80, 255, 55, 155, 200, 0, 0)");
        query.exec("insert into Colour values(3, 'Rot - Orange - Gelb', 255, 80, 80, 255, 155, 0, 255, 225, 25)");
        query.exec("insert into Colour values(4, 'Rot - Rot - Rot', 255, 80, 80, 255, 80, 80, 255, 80, 80)");
        query.exec("insert into Colour values(5, 'Grün - Grün - Grün', 100, 255, 100, 100, 255, 100, 100, 255, 100)");
        query.exec("insert into Colour values(6, 'Blau - Blau - Blau', 0, 155, 255, 0, 155, 255, 0, 155, 255)");
        query.exec("insert into Colour values(7, 'Gelb - Gelb - Gelb', 255, 225, 25, 255, 225, 25, 255, 225, 25)");
        query.exec("insert into Colour values(8, 'Orange - Orange - Orange', 255, 155, 0, 255, 155, 0, 255, 155, 0)");
        query.exec("insert into Colour values(9, 'Pink - Pink - Pink', 255, 55, 155, 255, 55, 155, 255, 55, 155)");

        // Datenbank Tabelle EffektePresets erstellen
        query.exec("CREATE TABLE IF NOT EXISTS 'EffektePresets' ( \
                   'EffPresID' INTEGER NOT NULL, \
                   'EffPresName' TEXT NOT NULL, \
                   'EffPresWLEDNR' INTEGER NOT NULL, \
                   PRIMARY KEY('EffPresID' AUTOINCREMENT) \
               );");

        // Tabelle EffektePresets mit WLED Project Effekten befüllen
        query.exec("insert into EffektePresets values(1, 'Solid      ', 0)");
        query.exec("insert into EffektePresets values(2, 'Blink      ', 1)");
        query.exec("insert into EffektePresets values(3, 'Breathe    ', 2)");
        query.exec("insert into EffektePresets values(4, 'Wipe       ', 3)");
        query.exec("insert into EffektePresets values(5, 'Scan       ', 10)");
        query.exec("insert into EffektePresets values(6, 'Saw        ', 16)");
        query.exec("insert into EffektePresets values(7, 'Aurora     ', 38)");
        query.exec("insert into EffektePresets values(8, 'Twinklefox ', 80)");
        query.exec("insert into EffektePresets values(9, 'Fireworks  ', 89)");

        // Datenbank Tabelle Effekte erstellen
        query.exec("CREATE TABLE IF NOT EXISTS 'Effekte' ( \
                   'EffID' INTEGER NOT NULL, \
                   'EffHelligkeit' INTEGER NOT NULL, \
                   'EffPar1' INTEGER NOT NULL, \
                   'EffPar2' INTEGER NOT NULL, \
                   'EffZeitdauer' INTEGER NOT NULL, \
                   'EffEffPresID' INTEGER NOT NULL, \
                   'EffColID' INTEGER NOT NULL, \
                   PRIMARY KEY('EffID' AUTOINCREMENT), \
                   FOREIGN KEY('EffEffPresID') REFERENCES 'EffektePresets'('EffPresID'), \
                   FOREIGN KEY('EffColID') REFERENCES 'Colour'('ColID') \
               );");

        // Datenbank Tabelle Sequenzen erstellen
        query.exec("CREATE TABLE IF NOT EXISTS 'Sequenzen' ( \
                   'SeqID' INTEGER NOT NULL, \
                   'SeqNr' INTEGER NOT NULL, \
                   'SeqName' TEXT NOT NULL, \
                   'SeqFolge' TEXT NOT NULL, \
                   'SeqEffID' INTEGER NOT NULL, \
                   PRIMARY KEY('SeqID' AUTOINCREMENT), \
                   FOREIGN KEY('SeqEffID') REFERENCES 'Effekte'('EffID') \
               );");

    return true;
}

#endif // SQLDATABASE_H
