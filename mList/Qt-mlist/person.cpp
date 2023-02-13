#include "person.h"

Person::Person()
{
}

//SET
void Person::setPersonalNr(int personalNr)
{
    m_personalNr=personalNr;
}

void Person::setRFIDkey(QString rfidKey)
{
    m_rfidKey=rfidKey;
}

void Person::setVorname(QString vorname)
{
    m_vorname=vorname;
}

void Person::setNachname(QString nachname)
{
    m_nachname=nachname;
}

void Person::setEmail(QString email)
{
    m_email=email;
}

void Person::setBoolAktiv(bool aktiv)
{
    m_boolAktiv=aktiv;
}

void Person::setKostenstelle(int kostenstelle)
{
    m_kostenstelle=kostenstelle;
}


//GET
int Person::getPersonalNr()
{
    return (m_personalNr);
}

QString Person::getRFIDkey()
{
    return (m_rfidKey);
}

QString Person::getVorname()
{
    return (m_vorname);
}

QString Person::getNachname()
{
    return (m_nachname);
}

QString Person::getEmail()
{
    return (m_email);
}

bool Person::getBoolAktiv()
{
    return (m_boolAktiv);
}

int Person::getKostenstelle()
{
    return (m_kostenstelle);
}
