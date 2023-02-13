#include "abrechnung.h"

Abrechnung::Abrechnung()
{
}

//SET
void Abrechnung::set_id(int id)
{
    m_id=id;
}

void Abrechnung::set_monthYear(QString monthYear)
{
    m_monthYear=monthYear;
}

void Abrechnung::set_personalNr(int personal_nr)
{
        m_personalNr=personal_nr;
}

void Abrechnung::set_konsumTyp(int konsumTyp)
{
    m_konsumTyp=konsumTyp;
}

void Abrechnung::set_konsumPreis(float preis)
{
    m_preis=preis;
}



//GET
int Abrechnung::getId()
{
    return (m_id);
}

QString Abrechnung::getMonthYear()
{
    return (m_monthYear);
}

int Abrechnung::getPeronalNr()
{
    return (m_personalNr);
}

int Abrechnung::getKonsumTyp()
{
    return (m_konsumTyp);
}

float Abrechnung::getPreis()
{
    return (m_preis);
}
