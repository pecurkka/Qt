#include "artikel.h"

Artikel::Artikel()
{    
}

void Artikel::set_artikelNr(int artikel_nr)
{
    m_artikel_nr=artikel_nr;
}
void Artikel::set_artikelBez(QString artikel_bez)
{
    m_artikel_bez=artikel_bez;
}
void Artikel::set_gruppe(QString gruppe)
{
    m_gruppe=gruppe;
}
void Artikel::set_priority(int priority)
{
    m_priority=priority;
}
void Artikel::set_preis(float preis)
{
    m_preis=preis;
}
void Artikel::set_refArtikel(int ref_artikel)
{
    m_ref_artikel=ref_artikel;
}
void Artikel::set_it(bool it)
{
    m_it=it;
}
void Artikel::set_technik(bool technik)
{
    m_technik=technik;
}
void Artikel::set_enabled(bool enabled)
{
    m_enabled=enabled;
}

int Artikel::get_artikelNr()
{
    return m_artikel_nr;
}
QString Artikel::get_artikelBez()
{
    return m_artikel_bez;
}
QString Artikel::get_gruppe()
{
    return m_gruppe;
}
int Artikel::get_priority()
{
    return m_priority;
}
float Artikel::get_preis()
{
    return m_preis;
}
int Artikel::get_refArtikel()
{
    return m_ref_artikel;
}
bool Artikel::get_it()
{
    return m_it;
}
bool Artikel::get_technik()
{
    return m_technik;
}
bool Artikel::get_enabled()
{
    return m_enabled;
}
