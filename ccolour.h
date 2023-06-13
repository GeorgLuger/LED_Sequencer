#ifndef CCOLOUR_H
#define CCOLOUR_H
#include <QString>
#include <QVector>


class CColour
{
public:
    CColour();
    QList<int> Colours;

private:
    int ColID;
    QString ColName;

};

#endif // CCOLOUR_H
