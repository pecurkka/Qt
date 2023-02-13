#ifndef CACHE_H
#define CACHE_H

#include <QMap>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <application_global.h>
#include <artikel.h>

#include <hbocore.h>
#include <hbolog.h>
#include <hbodbcore.h>
#include <hbooptions.h>

#include <db_mitarbeiter.h>

class Cache
{
private:
    Cache();
public:
    virtual ~Cache();

    static Cache * instance(void);
    static Cache * cache(void);
    Cache(Cache const&)       	   = delete;
    void operator=(Cache const&)   = delete;

public:

    //INIT
    void initKonsumMap();
    void initKonsumPriceMap();
    void initFeedbackVoteMap();
    void initMitarbeiterMap();
    void initDevicesMap();

    //GET
    QPalette getPalette();
    void setItemFontColor(QTreeWidget *tw, QTreeWidgetItem *item);

    QString getKonsumName(int konsumTyp);
    float getKonsumPrice(int konsumTyp);
    QString getFeedbackVoteName(QString vote);
    QIcon getFeedbackIcon(QString feedbackCode);
    QString getMitarbeiterName(QString adUser);
    QString getMitarbeiterName(int personalNr);
    QString getDeviceName(QString deviceSN);
    QString getGruppeName(QString gruppe);

private:

    //KONSUM CACHE
    QMap<int, QString> m_konsumTypMap;
    QMap<int, Artikel> m_artikelMap;
    QMap<int, double> m_konsumPriceMap;

    //FEEDBACK CACHE
    QMap<QString, QString> m_feedbackVoteMap;
    QMap<QString, QIcon> m_feedbackIconMap;

    //MITARBEITER MAP
    QMap<QString, QString> m_mitarbMap;
    QMap<int, QString> m_mitarbPNRMap;

    //DEVICES
    QMap<QString, QString> m_devicesMap;
};

#endif
/* eof */
