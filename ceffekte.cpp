#include "ceffekte.h"

CEffekte::CEffekte()
{

}

QJsonDocument CEffekte::erzeugeJSON(CEffekte EffektWerte)
{
    CColour FarbWerte;
    QSqlQuery query_colours("select * from Colour");

    // Befuellen des 2D Colour Arrays
    query_colours.seek(EffektWerte.EffColID);
    query_colours.previous();
    for (int i = 1; i < 11; i++)
    {
        FarbWerte.Colours.append(query_colours.value(i).toInt());
    }
    QJsonArray arr_col;
    for (int i = 0; i < 3; i++) {
        QJsonArray arr_color;
        for (int j = 0; j < 3; j++) {
            int index = i * 3 + j + 1;
            arr_color.append(FarbWerte.Colours.value(index));
        }
        arr_col.append(arr_color);
    }

    // Befuellen der restlichen Daten
    QJsonObject obj_seg;
    obj_seg["col"] = arr_col;
    obj_seg["fx"] = EffektWerte.EffPresWLEDNR;
    obj_seg["sx"] = EffektWerte.EffPar1;
    obj_seg["ix"] = EffektWerte.EffPar2;

    QJsonArray arr_seg;
    arr_seg.append(obj_seg);

    QJsonObject obj_state;
    obj_state["seg"] = arr_seg;

    QJsonObject obj_root;
    obj_root["bri"] = EffektWerte.EffHelligkeit;
    obj_root["seg"] = arr_seg;

    // Serialisieren des JSON Documents
    QJsonDocument doc(obj_root);

    return doc;
}
