#ifndef MLISTABRECHNUNG_H
#define MLISTABRECHNUNG_H

#include <application.h>
#include <cache.h>
#include <abrechnung.h>
#include <person.h>
#include <konsum.h>
#include <artikel.h>

#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <smtp.h>

class Application;

class mListAbrechnung : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(mListAbrechnung)

public:
    mListAbrechnung(QObject *parent, Application *app);
    virtual ~mListAbrechnung();

    bool exec();
    bool weekExport();

private:

    //MODE
    void setMode(QString mode);
    QString getMode();

    //GET
    bool getJSON(QString url, QString mode);

    //FILL
    bool fillPersonenFromJSON(QJsonDocument jsonDoc);
    void fillPaymentsForUsers(QJsonDocument jsonDoc);
    void fillArtikels(QJsonDocument jsonDoc);
    void fillAndExportWeekList(QJsonDocument jsonDoc);

private slots:
    //SLOTS
    bool slotReadJSON();


signals:
        void resumeLoop();
        void resumeLoopError();

private:
    Application *m_app;
    QNetworkAccessManager *m_networkAM;

    //MODE
    QString m_mode;

    //CACHE
    QList<Person>   m_personenList;
    QList<Konsum>   m_konsumList;
    QMap<int, Artikel> m_artikelMap;
    QStringList     m_weekExportList;
    bool            m_retval;


};



#endif // MLISTABRECHNUNG_H
