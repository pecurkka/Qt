#ifndef KONSUM_H
#define KONSUM_H

#include <QDateTime>

class Konsum
{
public:
    Konsum();

    //SET
    void setID(int id);
    void setPersonalNr(int personalNr);
    void setKonsumTyp(int konsumTyp);
    void setKonsumTime(QDateTime konsumTime);
    void setBoolBezalt(bool bezalt);
    void setBezahltAm(QDateTime bezahltAm);
    void setBezahltBei(QString bezahltBei);
    void setPreis(float preis);

    //GET
    int getID();
    int getPeronalNr();
    int getKonsumTyp();
    QDateTime getKonsumTime();
    bool getBoolBezalt();
    QDateTime getBezahltAm();
    QString getBezahltBei();
    float getPreis();

private:
    int m_id;
    int m_personalNr;
    int m_konsumTyp;
    QDateTime m_konsumTime;
    bool m_boolBezalt;
    QDateTime m_bezahltAm;
    QString m_bezahltBei;
    float m_preis;

};

#endif // KONSUM_H
