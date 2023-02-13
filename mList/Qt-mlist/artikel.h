#ifndef ARTIKEL_H
#define ARTIKEL_H

#include <QString>

class Artikel
{
public:
    Artikel();

    void set_artikelNr(int artikel_nr);
    void set_artikelBez(QString artikel_bez);
    void set_gruppe(QString gruppe);
    void set_priority(int priority);
    void set_preis(float preis);
    void set_refArtikel(int ref_artikel);
    void set_it(bool it);
    void set_technik(bool technik);
    void set_enabled(bool enabled);

    int get_artikelNr();
    QString get_artikelBez();
    QString get_gruppe();
    int get_priority();
    float get_preis();
    int get_refArtikel();
    bool get_it();
    bool get_technik();
    bool get_enabled();


private:
    int m_artikel_nr;
    QString m_artikel_bez;
    QString m_gruppe;
    int m_priority;
    float m_preis;
    int m_ref_artikel;
    bool m_it;
    bool m_technik;
    bool m_enabled;
};

#endif // ARTIKEL_H
