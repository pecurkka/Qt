#ifndef CACHE_H
#define CACHE_H

#include <qmap.h>
#include <hbodbcore.h>
#include <application_globals.h>
#include <artikel.h>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class Cache
{
public:
    Cache();
    virtual ~Cache();

public:
    static Cache * instance(void);
    static Cache * cache(void);
    Cache(Cache const&) = delete;
    void operator=(Cache const&) = delete;

    //INIT
private:
    void initKonsumMap();
    void initKonsumPriceMap();

    //GET
public:
    QString getKonsumName(int konsumTyp);
    double getKonsumPrice(int konsumTyp);

private:
    //KONSUM CACHE
    QMap<int, QString> m_konsumTypMap;
    QMap<int, Artikel> m_artikelMap;
    QMap<int, double> m_konsumPriceMap;
};

#endif // CACHE_H
