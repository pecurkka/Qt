#ifndef ABRECHNUNG_H
#define ABRECHNUNG_H

#include <QDateTime>

class Abrechnung
{
public:
    Abrechnung();

    //SET
    void set_id(int id);
    void set_monthYear(QString set_monthYear);
    void set_personalNr(int personal_nr);
    void set_konsumTyp(int konsumTyp);
    void set_konsumPreis(float preis);

    //GET
    int getId();
    QString getMonthYear();
    int getPeronalNr();
    int getKonsumTyp();
    float getPreis();

private:
    int m_id;
    QString m_monthYear;
    int m_personalNr;
    int m_konsumTyp;
    float m_preis;
};

#endif // ABRECHNUNG_H
