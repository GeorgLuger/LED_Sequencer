#ifndef CEFFEKTE_H
#define CEFFEKTE_H
#include <QString>
#include <QtSql>
#include <QJsonDocument>
#include <QJsonObject>
#include <ccolour.h>


class CEffekte
{
public:
    CEffekte();

    int EffID;
    QString EffPresName;
    int EffPresWLEDNR;
    int EffHelligkeit;
    int EffPar1;
    int EffPar2;
    int EffZeitdauer;
    int EffEffPresID;
    int EffColID;

public slots:
    // erzeugt auf Basis der Attribute ein JSON das vom Microcontroller empfangen werden kann
    QJsonDocument erzeugeJSON(CEffekte EffektWerte);

private:

};

#endif // CEFFEKTE_H
