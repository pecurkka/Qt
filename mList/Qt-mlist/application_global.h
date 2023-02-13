#ifndef APPLICATIONGLOBAL_H
#define APPLICATIONGLOBAL_H

//=============== CONNECTION ===============//
const QString conWWS ("WWS");


//=============== LAYOUT MODES ===============//
const QString LYT_START ("START");
const QString LYT_FOUND ("FOUND");
const QString LYT_NEW ("NEW");
const QString LYT_EDIT ("EDIT");
const QString LYT_NEW_ARTIKEL ("NEW_ARTIKEL");
const QString LYT_EDIT_ARTIKEL ("EDIT_ARTIKEL");


//=============== PERSON TREEWIDGET ===============//
static const int S_PERSON_VORNAME = 0;
static const int S_PERSON_NACHNAME = 1;
static const int S_PERSON_PERNR = 2;
static const int S_PERSON_PAYMENT = 3;


//=============== KONSUM TREEWIDGET ===============//
static const int S_KONSUM_KONSUM = 0;
static const int S_KONSUM_PREIS = 1;
static const int S_KONSUM_ZEIT = 2;
static const int S_KONSUM_DEVICE = 3;
static const int S_KONSUM_BEZAHLT_AM = 4;
static const int S_KONSUM_BEZAHLT_BEI = 5;


//=============== FEEDBACK TREEWIDGET ===============//
static const int S_FEEDBACK_FEEDBACK = 0;
static const int S_FEEDBACK_ZEIT = 1;

//=============== KONSUM BERICHT TREEWIDGET ===============//
static const int S_KONSUM_BERICHT_MONAT = 0;
static const int S_KONSUM_BERICHT_TYP = 1;
static const int S_KONSUM_BERICHT_COUNT = 2;


//=============== ARTIKEL TREEWIDGET ===============//
static const int S_ARTIKEL_NR = 0;
static const int S_ARTIKEL_BEZ = 1;
static const int S_ARTIKEL_GRUPPE = 2;
static const int S_ARTIKEL_PRIORITY = 3;
static const int S_ARTIKEL_PREIS = 4;
static const int S_ARTIKEL_REF_ARTIKEL = 5;
static const int S_ARTIKEL_IT = 6;
static const int S_ARTIKEL_TECHNIK = 7;
static const int S_ARTIKEL_ENABLED = 8;


//=============== ICONS ===============//
static QString ICON_ADD (":/resources/icon_add.png");
static QString ICON_EDIT (":/resources/icon_edit.png");
static QString ICON_PAYED (":/resources/icon_money.png");
static QString ICON_UNPAYED (":/resources/icon_close.png");
static QString ICON_FEEDBACK_GOOD (":/resources/icon_feedback_good.png");
static QString ICON_FEEDBACK_BAD (":/resources/icon_feedback_bad.png");
static QString ICON_FEEDBACK_NEUTRAL (":/resources/icon_feedback_neutral.png");

//=============== MESSAGES TITLE ===============//
static QString MSG_TITLE_ERROR ("FEHLER");


//=============== ERROR MESSAGES ===============//
static QString MSG_ERR_NO_KOSTENSTELLE ("Keine Kostenstelle für Benutzer [%1] festgelegt!");
static QString MSG_ERR_PERSONALNR_EXIST ("Personalnummer existiert!");
static QString MSG_ERR_PERSONALNR_NOT_ENTERED ("Personalnummer eingeben!");
static QString MSG_ERR_QNETWORKREPLY ("Fehler <QNetworkReply>\n%1");
static QString MSG_ERR_BY_SAVING ("Fehler beim Speichern!");
static QString MSG_ERR_BY_SETTING_AS_PAYED ("Fehler beim Markieren als bezahlt/Offen!");
static QString MSG_ERR_ARTIKEL_EXIST ("Artikel [%1] existiert!");


static QString baseUrl="https://mpreisfunction.azurewebsites.net/api/mlist?";


#endif  //APPLICATIONGLOBAL_H
