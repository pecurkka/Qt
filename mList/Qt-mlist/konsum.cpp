#include "konsum.h"

Konsum::Konsum()
{
}

//SET
void Konsum::setID(int id)
{
    m_id=id;
}

void Konsum::setPersonalNr(int personalNr)
{
    m_personalNr=personalNr;
}

void Konsum::setKonsumTyp(int konsumTyp)
{
    m_konsumTyp=konsumTyp;
}

void Konsum::setKonsumTime(QDateTime konsumTime)
{
    m_konsumTime=konsumTime;
}

void Konsum::setBoolBezalt(bool bezalt)
{
    m_boolBezalt=bezalt;
}

void Konsum::setBezahltAm(QDateTime bezahltAm)
{
    m_bezahltAm=bezahltAm;
}

void Konsum::setBezahltBei(QString bezahltBei)
{
    m_bezahltBei=bezahltBei;
}

void Konsum::setDeviceSN(QString deviceSN)
{
    m_deviceSN=deviceSN;
}

void Konsum::setPreis(float preis)
{
    m_preis=preis;
}


//GET
int Konsum::getID()
{
    return (m_id);
}

int Konsum::getPeronalNr()
{
    return (m_personalNr);
}

int Konsum::getKonsumTyp()
{
    return (m_konsumTyp);
}

QDateTime Konsum::getKonsumTime()
{
    return (m_konsumTime);
}

bool Konsum::getBoolBezalt()
{
    return (m_boolBezalt);
}

QDateTime Konsum::getBezahltAm()
{
    return (m_bezahltAm);
}

QString Konsum::getBezahltBei()
{
    return (m_bezahltBei);
}

QString Konsum::getDeviceSN()
{
    return (m_deviceSN);
}

float Konsum::getPreis()
{
    return (m_preis);
}
