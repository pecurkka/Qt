#include "cache.h"

using namespace hbo;

Cache::Cache()
{
    initKonsumMap();
    initKonsumPriceMap();
    initFeedbackVoteMap();
    initMitarbeiterMap();
    initDevicesMap();
}

Cache::~Cache()
{
}

Cache * Cache::cache()
{
    return instance();
}

Cache * Cache::instance()
{
    static Cache	INSTANCE;

    return & INSTANCE;
}


//INIT
void Cache::initKonsumMap()
{
    QNetworkAccessManager *nAM = new QNetworkAccessManager();

    QNetworkReply *reply = nAM->get(QNetworkRequest(QUrl(baseUrl + "&typ=getArtikels")));
    QObject::connect(reply, &QNetworkReply::finished, [reply, this](){

        if(reply->error()==QNetworkReply::NoError)
        {
            QString strReply = (QString)reply->readAll();

            QJsonDocument document = QJsonDocument::fromJson(strReply.toUtf8());
            QJsonArray jsonArray = document.array();
            m_artikelMap.clear();

            foreach (const QJsonValue & value, jsonArray)
            {
                QJsonObject obj = value.toObject();

                Artikel a;

                a.set_artikelNr(obj["artikel_nr"].toString().toInt());
                a.set_artikelBez(obj["artikel_bez"].toString().simplified());
                a.set_gruppe(obj["gruppe"].toString());
                a.set_priority(obj["priority"].toString().toInt());
                a.set_preis(obj["preis"].toString().toFloat());
                a.set_refArtikel(obj["ref_artikel"].toString()=="" ? -1 : obj["ref_artikel"].toString().toInt());
                a.set_it(obj["bool_it"].toString().toInt());
                a.set_technik(obj["bool_technik"].toString().toInt());
                a.set_enabled(obj["bool_enabled"].toString().toInt());

                m_artikelMap[a.get_artikelNr()]=a;
            }

            log()<<"CACHED <" << m_artikelMap.size() <<"> ARTIKELS";
            delete reply;
        }
        else
        {
            critical()<<"ERROR by getting JSON data: " << reply->error();
        }
    });

    /*

    //ALLE
    m_konsumTypMap[100]="Getränk";

    //IT
    m_konsumTypMap[101]="Getränk (Besprechung)";
    m_konsumTypMap[102]="Getränk (Defekt)";

    //TECHNIK
    m_konsumTypMap[103]="Getränk (Besprechung)";
    m_konsumTypMap[104]="Getränk (Defekt)";

    //ALLE
    m_konsumTypMap[200]="Nespresso";
    m_konsumTypMap[201]="Nespresso (Besprechung)";
    m_konsumTypMap[202]="Nespresso (Defekt)";

    //ALLE
    m_konsumTypMap[300]="Kaffee";

    //IT
    m_konsumTypMap[301]="Kaffee (Besprechung)";
    m_konsumTypMap[302]="Kaffee (Defekt)";

    //TECHNIK
    m_konsumTypMap[303]="Kaffee (Besprechung)";
    m_konsumTypMap[304]="Kaffee (Defekt)";

    //ALLE
    m_konsumTypMap[900]="Chefteller (Fleisch)";
    m_konsumTypMap[901]="Chefteller (Vegetarisch)";
    m_konsumTypMap[902]="Chefteller (Suppe)";

    m_konsumTypMap[950]="ILIKE Fleisch/Fish (warm)";
    m_konsumTypMap[951]="ILIKE Veggi (warm)";
    m_konsumTypMap[952]="ILIKE (kalt)";
    */
}

void Cache::initKonsumPriceMap()
{
    /*
    m_konsumPriceMap[100]=0.5;
    m_konsumPriceMap[101]=0.0;
    m_konsumPriceMap[102]=0.0;
    m_konsumPriceMap[103]=0.0;
    m_konsumPriceMap[104]=0.0;

    m_konsumPriceMap[200]=0.3;
    m_konsumPriceMap[201]=0.0;
    m_konsumPriceMap[202]=0.0;

    m_konsumPriceMap[300]=0.15;
    m_konsumPriceMap[301]=0.0;
    m_konsumPriceMap[302]=0.0;
    m_konsumPriceMap[303]=0.0;
    m_konsumPriceMap[304]=0.0;

    m_konsumPriceMap[900]=3.5;
    m_konsumPriceMap[901]=3.0;
    m_konsumPriceMap[902]=3.0;

    m_konsumPriceMap[950]=3.0;
    m_konsumPriceMap[951]=2.5;
    m_konsumPriceMap[952]=2.0;
    */
}

void Cache::initFeedbackVoteMap()
{
    m_feedbackVoteMap["0"]="Neutral";
    m_feedbackVoteMap["1"]="Schlecht";
    m_feedbackVoteMap["10"]="Gut";

    m_feedbackIconMap["0"]=QIcon(ICON_FEEDBACK_NEUTRAL);
    m_feedbackIconMap["1"]=QIcon(ICON_FEEDBACK_BAD);
    m_feedbackIconMap["10"]=QIcon(ICON_FEEDBACK_GOOD);
}

void Cache::initMitarbeiterMap()
{
    DB_MitarbeiterTransporter t(conWWS);
    QList<DB_Mitarbeiter> l;
    t.loadWc(&l, "");

    for (DB_Mitarbeiter & m: l)
    {
        if(m.get_adUser()!="")
            m_mitarbMap[m.get_adUser()]=QString("%1 %2").arg(m.get_vorname()).arg(m.get_nachname());

        m_mitarbPNRMap[m.get_personalnr()]=QString("%1 %2").arg(m.get_vorname()).arg(m.get_nachname());
    }

    log()<<"CACHED <" << m_mitarbMap.size() << ">MITARBEITERN";
}

void Cache::initDevicesMap()
{
    m_devicesMap["F173022883"]="Büro DMI";
    m_devicesMap["F173023061"]="IT - Küche";
    m_devicesMap["F173023034"]="IT - Kühlschrank";
    m_devicesMap["F173023005"]="Technik";
}

//GET
QPalette Cache::getPalette()
{
    QPalette palette;

    palette.setColor(QPalette::Disabled,    QPalette::ButtonText,   Qt::black);
    palette.setColor(QPalette::Disabled,    QPalette::WindowText,   Qt::black);
    palette.setColor(QPalette::Disabled,    QPalette::Text,         Qt::black);

    return (palette);
}

void Cache::setItemFontColor(QTreeWidget *tw, QTreeWidgetItem *item)
{
    //DEFINE BRUSH
    QBrush brush;
    QColor brush_color("#666666");
    brush.setColor(brush_color);

    for(int i=0; i<tw->columnCount(); i++)
        item->setForeground(i, brush);

}

QString Cache::getKonsumName(int konsumTyp)
{

    if(!m_artikelMap.contains(konsumTyp))
    {
        critical()<<"ARTIKEL <" << konsumTyp << "> NOT FOUND IN CACHE";
        return (QString::number(konsumTyp));
    }

    return (m_artikelMap[konsumTyp].get_artikelBez());

    /*
    if(!m_konsumTypMap.contains(konsumTyp))
    {
        critical()<<"KONSUM TYP <" << konsumTyp << "> NOT FOUND IN KONSUM TYP MAP";
        return (QString::number(konsumTyp));
    }

    return (m_konsumTypMap[konsumTyp]);
    */
}

float Cache::getKonsumPrice(int konsumTyp)
{
    if(!m_artikelMap.contains(konsumTyp))
    {
        critical()<<"ARTIKEL <" << konsumTyp << "> NOT FOUND IN CACHE";
        return (0.00);
    }
    return (m_artikelMap[konsumTyp].get_preis());
    /*
    if(!m_konsumPriceMap.contains(konsumTyp))
    {
        critical()<<"KONSUM TYP <" << konsumTyp << "> NOT FOUND IN KONSUM PRICE MAP";
        return (konsumTyp);
    }

    return (m_konsumPriceMap[konsumTyp]);
    */
}

QString Cache::getFeedbackVoteName(QString vote)
{
    if(!m_feedbackVoteMap.contains(vote))
    {
        critical()<<"FEEDBACK VOTE <" << vote << "> NOT FOUND IN FEEDBACK MAP";
        return (vote);
    }

    return (m_feedbackVoteMap[vote]);
}

QIcon Cache::getFeedbackIcon(QString feedbackCode)
{
    if(!m_feedbackIconMap.contains(feedbackCode))
    {
        critical()<<"FEEDBACK ICON <" << feedbackCode << "> NOT FOUND IN FEEDBACK MAP";
        return (QIcon());
    }

    return (m_feedbackIconMap[feedbackCode]);
}

QString Cache::getMitarbeiterName(QString adUser)
{
    if(!m_mitarbMap.contains(adUser))
    {
        critical()<<"MITARBEITER <" << adUser << "> NOT FOUND IN MITARBEITER MAP";
        return (adUser);
    }

    return (m_mitarbMap[adUser]);
}

QString Cache::getMitarbeiterName(int personalNr)
{
    if(!m_mitarbPNRMap.contains(personalNr))
    {
        critical()<<"MITARBEITER WITH PERSONALNR <" << personalNr << "> NOT FOUND IN MITARBEITER MAP";
        return (QString::number(personalNr));
    }

    return (m_mitarbPNRMap[personalNr]);
}

QString Cache::getDeviceName(QString deviceSN)
{
    if(!m_devicesMap.contains(deviceSN))
    {
        critical()<<"DEVICE <" << deviceSN << "> NOT FOUND IN DEVICES MAP";
        return (deviceSN);
    }

    return (m_devicesMap[deviceSN]);
}

QString Cache::getGruppeName(QString gruppe)
{
    if(gruppe=="GETRAENK")
        return ("Getränk");
    else if(gruppe=="NESPRESSO")
        return ("Nespresso");
    else if(gruppe=="KAFFEE")
        return ("Kaffee");
    else if(gruppe=="CHEFTELLER")
        return ("Chefteller");
    else if(gruppe=="ILIKE")
        return ("ILIKE");
    else
        return (gruppe);
}

/* eof */

