#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person
{
public:
    Person();

    //SET
    void setPersonalNr(int personalNr);
    void setRFIDkey(QString rfidKey);
    void setVorname(QString vorname);
    void setNachname(QString nachname);
    void setEmail(QString email);
    void setBoolAktiv(bool aktiv);
    void setKostenstelle(int kostenstelle);

    //GET
    int getPersonalNr();
    QString getRFIDkey();
    QString getVorname();
    QString getNachname();
    QString getEmail();
    bool getBoolAktiv();
    int getKostenstelle();

private:

    //DEFINE ITEMS
    int m_personalNr;
    QString m_rfidKey;
    QString m_vorname;
    QString m_nachname;
    QString m_email;
    bool m_boolAktiv;
    int m_kostenstelle;
};

#endif // PERSON_H
