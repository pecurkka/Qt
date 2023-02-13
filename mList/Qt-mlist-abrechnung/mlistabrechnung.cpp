#include "mlistabrechnung.h"

//BASE URL
static QString baseUrl="https://mpreisfunction.azurewebsites.net/api/mlist?";

//CALCULATING MODES
static QString modCalcGetAllUsers="MOD_CALC_ALLUSERS";
static QString modCalcGetKonsumForAll="MOD_CALC_KONSUMFORALL";
static QString modGetArtikels="MOD_GET_ARTIKELS";
static QString modGetWeekExport="MOD_GET_WEEK_EXPORT";

mListAbrechnung::mListAbrechnung(QObject *parent, Application *app) :
    QObject(parent),
    m_app(app)
{
    //SETUP PROXY
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("10.1.1.250");
    proxy.setPort(3128);
    QNetworkProxy::setApplicationProxy(proxy);

    //DEFINE NETWORK ACCESS MANAGER
    m_networkAM = new QNetworkAccessManager(this);
}

mListAbrechnung::~mListAbrechnung()
{
    log()<< "Arrivederci mListAbrechnung";
}

bool mListAbrechnung::exec()
{
    log(9) << "mListAbrechnung::exec(): ...";

    //CREATE EVENT LOOP
    QEventLoop eventLoop;
    eventLoop.connect(this, SIGNAL(resumeLoop()), &eventLoop, SLOT(quit()));

    //GET JSON WITH ALL IT USERS
    getJSON(baseUrl + "&typ=getArtikels", modGetArtikels);

    //PAUSE LOOP
    if(eventLoop.exec()!=0)
    {
        critical()<<"MainWindow::slotCalculateKonsumAndSendEmails():  ERROR BY EXEC from eventLoop for all users";
        return (false);
    }

    //GET JSON WITH ALL IT USERS
    getJSON(baseUrl + "&typ=getAllUsers&kostenstelle=9993", modCalcGetAllUsers);

    //PAUSE LOOP
    if(eventLoop.exec()!=0)
    {
        critical()<<"MainWindow::slotCalculateKonsumAndSendEmails():  ERROR BY EXEC from eventLoop for all users";
        return (false);
    }

    //GET JSON WITH ALL TECHNIK USERS
    getJSON(baseUrl + "&typ=getAllUsers&kostenstelle=9991", modCalcGetAllUsers);

    //PAUSE LOOP
    if(eventLoop.exec()!=0)
    {
        critical()<<"MainWindow::slotCalculateKonsumAndSendEmails():  ERROR BY EXEC from eventLoop for all users";
        return (false);
    }

    //GET JSON WITH ALL USERS
    getJSON(baseUrl + "&typ=getKonsumForAll", modCalcGetKonsumForAll);

    //PAUSE LOOP
    if(eventLoop.exec()!=0)
    {
        critical()<<"MainWindow::slotCalculateKonsumAndSendEmails():  ERROR BY EXEC from eventLoop for all konsum";
        return (false);
    }

    //DEFINE ABRECHUNGN LIST
    QList<Abrechnung> abrechnungList;

    //GO TROUGHT KONSUMS AND FILL ABRECHUNNG LIST
    for(Konsum k : m_konsumList)
    {
        //GO TO NEXT IF THIS MONTH
        QDate currentDate=QDate::currentDate();
        if(k.getKonsumTime().date()>=currentDate)
            continue;

        //GET MONTH AND YEAR
        QString monthYear=k.getKonsumTime().toString("MM.yyyy");

        Abrechnung a;
        a.set_id(k.getID());
        a.set_monthYear(monthYear);
        a.set_personalNr(k.getPeronalNr());
        a.set_konsumTyp(k.getKonsumTyp());
        a.set_konsumPreis(k.getPreis());

        abrechnungList.append(a);
    }

    //GO TROUGHT USERS LIST AND FILL KONSUM MAP
    for(Person p : m_personenList)
    {
        //GET PERSONAL NR
        int personalNr=p.getPersonalNr();

        //CREATE MONTHS GROUPS
        QMap<QString, bool> monthsGroup;
        for(Abrechnung a : abrechnungList)
        {
            //GO TO NEXT IF OTHER PERSONAL NR
            if(a.getPeronalNr()!=personalNr)
                continue;

            if(!monthsGroup.contains(a.getMonthYear()))
                monthsGroup[a.getMonthYear()]=true;
        }

        //DEFINE EMAIL
        QString email;

        QString mitarb=QString("%1 %2 (%3)")
                .arg(p.getVorname())
                .arg(p.getNachname())
                .arg(p.getPersonalNr());

        email="Hallo " + mitarb + ",\n";
        email+="deine mList Abrechnung ist fertig.\n";

        log()<<"=============================================================";
        log()<<"ABRECHNUNG FUER " << p.getVorname() << p.getNachname();

        //DEFINE END SUM
        double endSum=0.0;

        //GO TROUGHT MAP AND FILL EMAIL
        QMap<QString, bool>::iterator it;
        for (it = monthsGroup.begin(); it != monthsGroup.end(); ++it)
        {
            //DEFINE KONSUM COUNT
            QMap<int, int> konsumCount;
            QMultiMap<int, QPair<float, int>> mkMap;

            //GO TROUGHT ABRECHNUNG LIST AND GENERATE EMAIL
            for(Abrechnung a : abrechnungList)
            {
                //GO TO NEXT IF OTHER PERSONAL NR
                if(a.getPeronalNr()!=personalNr)
                    continue;

                //GO TO NEXT IF OTHER GROUP
                if(a.getMonthYear()!=it.key())
                    continue;

                if(!mkMap.contains(a.getKonsumTyp()))
                    mkMap.insert(a.getKonsumTyp(), QPair<float, int>(a.getPreis(), 1));
                else
                {
                    //PREIS UNGLEICH
                    if(mkMap.value(a.getKonsumTyp()).first!=a.getPreis())
                        mkMap.insert(a.getKonsumTyp(), QPair<float, int>(a.getPreis(), 1));
                    else
                    {
                        int count=mkMap.value(a.getKonsumTyp()).second;
                        mkMap.remove(a.getKonsumTyp(), QPair<float, int>(a.getPreis(), mkMap.value(a.getKonsumTyp()).second));
                        mkMap.insert(a.getKonsumTyp(), QPair<float, int>(a.getPreis(), count+1));
                    }
                }

                //CHECK IF BESPRECHUNG OR DEFEKT
                if(a.getKonsumTyp()==101 || a.getKonsumTyp()==102
                        || a.getKonsumTyp()==201 || a.getKonsumTyp()==202
                        || a.getKonsumTyp()==301 || a.getKonsumTyp()==302
                        || a.getKonsumTyp()==103 || a.getKonsumTyp()==104
                        || a.getKonsumTyp()==303 || a.getKonsumTyp()==304)
                    continue;

                if(!konsumCount.contains(a.getKonsumTyp()))
                    konsumCount[a.getKonsumTyp()]=1;
                else
                    konsumCount[a.getKonsumTyp()]=konsumCount[a.getKonsumTyp()]+1;
            }

            //GO TO NEXT MONTH IF EMPTY
            if(konsumCount.size()==0)
                continue;

            //MAKE TEXT FOR EMAIL
            QString msg="\n\n-----------------------------------------\n";
            msg+="Monat: " + it.key() + "\n\n";

            log()<<"MONAT " << it.key();

            QMap<int, int>::iterator itCount;
            double monthSum=0.0;

            /*
            for (itCount = konsumCount.begin(); itCount != konsumCount.end(); ++itCount)
            {
                //QString konsumName=Cache::instance()->getKonsumName(itCount.key());
                QString konsumName=m_artikelMap[itCount.key()].get_artikelBez();
                //double konsumSumPrice=Cache::instance()->getKonsumPrice(itCount.key()) * itCount.value();
                //double konsumSumPrice=konsumItemPreisMap[itCount]
                double konsumSumPrice=Cache::instance()->getKonsumPrice(itCount.key()) * itCount.value();
                msg+=QString("%1 - %2x = %3€")
                        .arg(konsumName)
                        .arg(itCount.value())
                        .arg(QString::number(konsumSumPrice, 'f', 2));

                log()<<QString("%1 - %2x = %3€")
                       .arg(konsumName)
                       .arg(itCount.value())
                       .arg(QString::number(konsumSumPrice, 'f', 2));

                monthSum=monthSum+konsumSumPrice;
                msg+="\n";
            }
            */

            QMultiMap<int, QPair<float, int>>::iterator itMK;

            for(itMK= mkMap.begin(); itMK != mkMap.end(); ++itMK)
            {
                QString konsumName="Unbekannt";
                if(m_artikelMap.contains(itMK.key()))
                {
                    konsumName=QString("%1 (%2€)")
                            .arg(m_artikelMap[itMK.key()].get_artikelBez())
                            .arg(QString::number(itMK.value().first, 'f', 2));
                }
                int konsumCount=itMK.value().second;
                float konsumSumPrice=itMK.value().first * konsumCount;

                msg+=QString("%1 - %2x = %3€")
                        .arg(konsumName)
                        .arg(konsumCount)
                        .arg(QString::number(konsumSumPrice, 'f', 2));

                log()<<QString("%1 - %2x = %3€")
                       .arg(konsumName)
                       .arg(konsumCount)
                       .arg(QString::number(konsumSumPrice, 'f', 2));

                monthSum=monthSum+konsumSumPrice;
                msg+="\n";

            }

            msg+="\nSumme: " + QString::number(monthSum, 'f', 2) + "€\n";
            msg+="-----------------------------------------\n";

            log()<<"SUMME FUER MONAT: " << QString::number(monthSum, 'f', 2) + "€";
            endSum=endSum+monthSum;
            email+=msg;
        }

        if(endSum==0)
            continue;

        log()<<"ENDSUMME: " << QString::number(endSum, 'f', 2) << "€";
        log()<<"=============================================================";


        QString zahlenBei="Moni";
        if(p.getKostenstelle()==9991)
            zahlenBei="Julia";
        email+=QString("\n\nBitte bei %1 %2€ bezahlen.\n")
                .arg(zahlenBei)
                .arg(QString::number(endSum, 'f', 2));

        email+="Danke";

        //CHECK IF EMAIL NULL OR EMPTY
        if(p.getEmail().isEmpty() || p.getEmail().isNull() || p.getEmail()==QString())
        {
            int retval;
            retval=system(QString("echo \"Konsum Abrechnung Fehler: Benutzer [%1] hat keine E-Mail Adresse!\"|mail -a \"Content-Type: text/plain; charset=UTF-8\" -s \"Konsum Abrechnung\"  %3")
                          .arg(p.getPersonalNr())
                          .arg("dmi@mpreis.at").toUtf8().data());

            if(retval>0)
                critical()<<"ERROR WHILE SENDING ERROR EMAIL TO [dmi@mpreis.at]";
            else
                log()<<"ERROR EMAIL SENT TO [dmi@mpreis.at]";
        }

        log()<<email;
        QString mail_cmd=QString("echo \"%1\"|mail -a \"Content-Type: text/plain; charset=UTF-8\" -s \"mList Abrechnung\"  %2")
                .arg(email)
                .arg(p.getEmail());

        int retval;
        retval=system(mail_cmd.toUtf8().data());

        if(retval>0)
            critical()<<"ERROR WHILE SENDINGG EMAIL TO [" << p.getEmail() << "]";
        else
        {
            log()<<"EMAIL SENT TO [" << p.getEmail() << "]";
        }
    }

    return (true);
}

bool mListAbrechnung::weekExport()
{
    log(8)<<"mListAbrechnung::weekExport(): ...";

    m_retval=false;

    //CREATE EVENT LOOP
    QEventLoop eventLoop;
    eventLoop.connect(this, SIGNAL(resumeLoop()), &eventLoop, SLOT(quit()));

    //GET JSON WITH ALL IT USERS
    getJSON(baseUrl + "&typ=getWeekExport", modGetWeekExport);

    //PAUSE LOOP
    if(eventLoop.exec()!=0)
    {
        critical()<<"MainWindow::slotCalculateKonsumAndSendEmails():  ERROR BY EXEC from eventLoop for all users";
        return (false);
    }

    return m_retval;
}



//MODE
void mListAbrechnung::setMode(QString mode)
{
    m_mode=mode;
}

QString mListAbrechnung::getMode()
{
    return (m_mode);
}



//GET
bool mListAbrechnung::getJSON(QString url, QString mode)
{
    log(6)<<"MainWindow::getJSON(" << url << ", " << mode << "): ...";

    //SET MODE
    setMode(mode);

    QNetworkReply *reply = m_networkAM->get(QNetworkRequest(QUrl(url)));
    connect(reply, SIGNAL(finished()), this, SLOT(slotReadJSON()));

    return (true);
}



//FILL
bool mListAbrechnung::fillPersonenFromJSON(QJsonDocument jsonDoc)
{
    log(8)<<"MainWindow::fillPersonenFromJSON(QJsonDocument): ...";

    //READ JSON ARRAY
    QJsonArray jsonArray = jsonDoc.array();

    //GO TROUGHT ARRAY AND FILL PERSONEN
    foreach (const QJsonValue & value, jsonArray)
    {
        //CREATE JSON OBJECT
        QJsonObject obj = value.toObject();

        //FILL PERSON CLASS
        Person person;

        person.setPersonalNr(obj["personalNr"].toString().toInt());
        person.setRFIDkey(obj["rfidKey"].toString());
        person.setVorname(obj["vorname"].toString());
        person.setNachname(obj["nachname"].toString());
        person.setEmail(obj["email"].toString());
        person.setBoolAktiv(obj["personAktiv"].toString().toInt());
        person.setKostenstelle(obj["kostenstelle"].toString().toInt());

        //FILL PERSON TO LIST
        m_personenList.append(person);
    }

    log()<<"GOT <" << m_personenList.count() << "> USERS";
    emit resumeLoop();

    return (true);
}

void mListAbrechnung::fillPaymentsForUsers(QJsonDocument jsonDoc)
{
    log(8)<<"MainWindow::fillPaymentsForUsers(QJsonDocument): ...";

    //READ JSON ARRAY
    QJsonArray jsonArray = jsonDoc.array();

    //GO TROUGHT ARRAY AND FILL PERSONEN
    foreach (const QJsonValue & value, jsonArray)
    {
        //CREATE JSON OBJECT
        QJsonObject obj = value.toObject();

        //FILL PERSON CLASS
        Konsum konsum;

        konsum.setID(obj["id"].toString().toInt());
        konsum.setPersonalNr(obj["personalNr"].toString().toInt());
        konsum.setKonsumTyp(obj["konsumTyp"].toString().toInt());
        konsum.setKonsumTime(QDateTime::fromString(obj["konsumZeit"].toString(), "dd.MM.yyyy hh:mm"));

        konsum.setBoolBezalt(obj["boolBezalt"].toString().toInt());
        konsum.setPreis(obj["preis"].toString().toFloat());

        m_konsumList.append(konsum);
    }

    log()<<"GOT <" << m_konsumList.count() << "> KONSUMS";
    emit resumeLoop();
    return;

}

void mListAbrechnung::fillArtikels(QJsonDocument jsonDoc)
{
    log(8)<<"MainWindow::fillArtikels(QJsonDocument): ...";

    m_artikelMap.clear();
    QJsonArray jsonArray=jsonDoc.array();

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
    emit resumeLoop();
    return;
}

void mListAbrechnung::fillAndExportWeekList(QJsonDocument jsonDoc)
{
    log()<<"MainWindow::fillAndExportWeekList(QJsonDocument): ...";

    m_artikelMap.clear();
    QJsonArray jsonArray=jsonDoc.array();

    QString fileName=QString("/home/hafina/production/tmp/mlist_ilike_export_%1.csv")
            .arg(QDate::currentDate().toString("yyyyMMdd"));

    //OPEN FILE
    QFile file(fileName);
    if(file.exists())
    {
        if(!file.remove())
        {
            log()<<"ERROR WHILE REMOVING FILE [" << fileName << "]";
            emit resumeLoop();
            return;
        }
    }

    //OPEN FILE AND WRITE LINES
    if(!file.open(QIODevice::ReadWrite))
    {
        log()<<"ERROR WHILE OPENING FILE [" << fileName << "]";
        emit resumeLoop();
        return;
    }
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << "PERSONAL_NR,PERSON,ARTIKEL_NR,ARTIKEL_BEZ,PREIS,ANZAHL" << "\n";


    foreach (const QJsonValue & value, jsonArray)
    {
        QJsonObject obj = value.toObject();

        stream <<  obj["personal_nr"].toString() << ","
               << "\"" << obj["person"].toString() << "\","
               << obj["artikel_nr"].toString() << ","
               << "\"" << obj["artikel_bez"].toString() << "\","
               << "\"" << obj["preis"].toString() << "\","
               << obj["anzahl"].toString() << "\n";
    }
    file.close();

    QString command=QString("unoconv -f xlsx -i FilterOptions=44,34,76,1,,1031,true %1")
            .arg(fileName);

    int retval=system(command.toLatin1().data());
    if(retval!=0)
    {
        critical() << "FAILED to convert CSV to XLSX <" << command <<"> - RETVAL: "  << retval;
        emit resumeLoop();
        return;
    }

    log()<<"FILE <" << fileName << "> CONVERTED TO XLSX!";
    fileName=fileName.replace(".csv", ".xlsx");
    if(!SMTP::instance()->sendHTMLEmail("root@hermes.mpreis.co.at",
                                        QStringList("monika.riedl@mpreis.at"),
                                        QStringList(),QStringList(),
                                        "mList ILIKE Export",SMTP::KeineZuordnung,
                                        "Die Exportdatei befindet sich im Anhang!",
                                        QStringList(fileName)))
    {
        emit resumeLoop();
        return;
    }

    m_retval=true;
    emit resumeLoop();
    return;
}


//SLOTS
bool mListAbrechnung::slotReadJSON()
{
    log(8)<<"MainWindow::slotReadJSON(): ...";

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    //IF NO ERROR
    if(reply->error()==QNetworkReply::NoError)
    {
        //READ JSON
        QString strReply = (QString)reply->readAll();

        //MAKE JSON DOCUMENT
        QJsonDocument document = QJsonDocument::fromJson(strReply.toUtf8());

        //FILL PERSONEN
        if(getMode()==modCalcGetAllUsers)
            fillPersonenFromJSON(document);

        //FILL PAYMENTS FOR USERS
        else if(getMode()==modCalcGetKonsumForAll)
            fillPaymentsForUsers(document);

        //FILL ARTIKES MAP
        else if(getMode()==modGetArtikels)
            fillArtikels(document);

        //EXPORT WEEK LIST
        else if(getMode()==modGetWeekExport)
            fillAndExportWeekList(document);

        delete reply;
    }
    else
    {
        critical()<<"MainWindow::slotGetJSONData(): ERROR by getting JSON data: " << reply->error();
        return (false);
    }

    return (true);
}

