#include <QSettings>
#include <QShortcut>

#include "mainwindow.h"

using namespace hbo;

extern QString const WWSconn;

//MODES
static QString modAllUsers="MOD_ALLUSERS";
static QString modAllKonsum="MOD_ALLKONSUM";
static QString modNewUser="MOD_NEWUSER";
static QString modEditUser="MOD_EDITUSER";
static QString modKonsumForAll="MOD_KONSUMFORALL";
static QString modSetPayed="MOD_SETPAYED";
static QString modGetFeedback="MOD_FEEDBACK";
static QString modKonsumBericht="MOD_KONSUM_BERICHT";
static QString modArtikel="MOD_ARTIKEL";
static QString modNewArtikel="MOD_NEW_ARTIKEL";
static QString modEditArtikel="MOD_EDIT_ARTIKEL";

MainWindow::MainWindow(QString adUser) : ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //SET AD_USER
    m_adUser=adUser;

    setup_mw();
    return;
}

MainWindow::~MainWindow()
{
}


//SETUP
void MainWindow::setup_mw(void)
{
    //SET TITLE
    setWindowTitle("MPREIS mList");

    //SET ICON
    setWindowIcon(BO_Icons::instance()->getLogoIcon(BO_Icons::Logo::Logo_MPREIS));

    //SET WINDOW TO CENTER
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), QGuiApplication::screens().at(0)->availableGeometry()));

    //WEB
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("10.1.1.250");
    proxy.setPort(3128);
    QNetworkProxy::setApplicationProxy(proxy);

    //SETUP TREEWIDGET
    setup_treeWidgets();

    //SETUP TOOLBAR
    setup_toolBar();

    //INSTAL EVENT FILTER
    installEventFilter(this);

    //DEFINE NETWORK ACCESS MANAGER
    m_networkAM = new QNetworkAccessManager(this);

    //SET VALIDATORS
    ui->le_artikel_nr->setValidator(new QIntValidator(0, 999999));
    ui->le_ref_artikel->setValidator(new QIntValidator(0, 999999));
    ui->le_priority->setValidator(new QIntValidator(0, 999));
    QDoubleValidator* dv = new QDoubleValidator(0.00, 999.99, 2, ui->le_preis);
    dv->setNotation(QDoubleValidator::StandardNotation);
    ui->le_preis->setValidator(dv);

}

void MainWindow::setup_treeWidgets()
{
    log(8)<<"MainWindow::setup_treeWidget(): ...";

    //USERS
    QStringList h;
    h << "Vorname"
      << "Nachname"
      << "Personal Nr."
      << "Zu zahlen";

    ui->tw_personen->setHeaderLabels(h);
    ui->tw_personen->setAlternatingRowColors(true);
    ui->tw_personen->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tw_personen->setSortingEnabled(false);
    ui->tw_personen->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->bow_search->setTreeWidget(ui->tw_personen);
    ui->bow_search->setKeyTarget(ui->tw_personen);
    ui->tw_personen->installEventFilter(ui->bow_search->getEventFilter());


    //KONSUM
    QStringList kh;
    kh << "Konsum"
       << "Preis"
       << "Zeit"
       << "Gerät";

    ui->tw_konsum->setHeaderLabels(kh);
    ui->tw_konsum->setAlternatingRowColors(true);
    ui->tw_konsum->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tw_konsum->setSortingEnabled(false);
    ui->tw_konsum->setContextMenuPolicy(Qt::CustomContextMenu);


    //FEEDBCK
    QStringList fh;
    fh << "Bewertung"
       << "Zeit";

    ui->tw_feedback->setHeaderLabels(fh);
    ui->tw_feedback->setAlternatingRowColors(true);
    ui->tw_feedback->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tw_feedback->setSortingEnabled(false);
    ui->tw_feedback->setContextMenuPolicy(Qt::CustomContextMenu);


    //KONSUM BERICHT
    QStringList kbh;
    kbh << "Monat"
        << "Konsum"
        << "Summe";

    ui->tw_konsum_bericht->setHeaderLabels(kbh);
    ui->tw_konsum_bericht->setAlternatingRowColors(true);
    ui->tw_konsum_bericht->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tw_konsum_bericht->setSortingEnabled(false);
    ui->tw_konsum_bericht->setContextMenuPolicy(Qt::CustomContextMenu);


    //ARTIKELN
    QStringList ah;
    ah << "Artikelnr."
       << "Bezeichnung"
       << "Gruppe"
       << "Priority"
       << "Preis"
       << "Ref. Artikel"
       << "IT"
       << "Technik"
       << "Status";

    ui->tw_artikeln->setHeaderLabels(ah);
    ui->tw_artikeln->setAlternatingRowColors(true);
    ui->tw_artikeln->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tw_artikeln->setSortingEnabled(false);
    ui->tw_artikeln->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tw_artikeln->setSortingEnabled(true);
    ui->bow_search_artikel->setTreeWidget(ui->tw_artikeln);
    ui->bow_search_artikel->setKeyTarget(ui->tw_artikeln);
    ui->tw_artikeln->installEventFilter(ui->bow_search_artikel->getEventFilter());

}

void MainWindow::setup_toolBar()
{
    log(8)<<"MainWindow::setup_toolBar(): ...";

    //new
    m_menuNew=new QAction(BO_Icons::instance()->getStandardIcon(BO_Icons::Standard_Hinzufuegen), "Neuer Mitarbeiter",this);
    ui->toolBar->addAction(m_menuNew);

    //edit
    m_menuEdit=new QAction(BO_Icons::instance()->getStandardIcon(BO_Icons::Standard_Aendern), "Mitarbeiter bearbeiten",this);
    ui->toolBar->addAction(m_menuEdit);

    //seperator
    ui->toolBar->addSeparator();

    //kostenstelle
    QLabel *lbl=new QLabel;
    lbl->setText("Kostenstelle ");
    ui->toolBar->addWidget(lbl);

    m_cmbKS = new QComboBox;
    m_cmbKS->addItem("IT/Energie und Umwelt", 9993);
    m_cmbKS->addItem("Technik", 9991);
    ui->toolBar->addWidget(m_cmbKS);
    connect(m_cmbKS, SIGNAL(currentIndexChanged(int)), SLOT(slotKSchanged()));

    //seperator
    ui->toolBar->addSeparator();

    //filter
    QLabel *lblfilter=new QLabel;
    lblfilter->setText("Filter ");
    ui->toolBar->addWidget(lblfilter);

    m_cmbfilter = new QComboBox;
    m_cmbfilter->addItem("Nur aktive Mitarbeiter", true);
    m_cmbfilter->addItem("Alle Mitarbeiter", false);
    ui->toolBar->addWidget(m_cmbfilter);
    m_filteraktivemitarbeiter=m_cmbfilter->currentData().toBool();
    connect(m_cmbfilter, SIGNAL(currentIndexChanged(int)), SLOT(slotFilterChanged()));

    //DISABLE EDIT MENU
    m_menuEdit->setEnabled(false);

    //DISABLE MENU
    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->menuBar->setContextMenuPolicy(Qt::PreventContextMenu);

}

void MainWindow::connect_slots()
{
    log(8)<<"MainWindow::connect_slots(): ...";

    //TREEWIDGET
    connect(ui->tw_personen, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(slotLoadUser(QTreeWidgetItem*,int)));
    //connect(ui->tw_personen, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(slotLoadUser(QTreeWidgetItem*,int)));
    connect(ui->tw_personen, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slotLoadUser(QTreeWidgetItem*,int)));
    connect(ui->tw_konsum, SIGNAL(customContextMenuRequested(QPoint)),  this, SLOT(slotShowKonsumMenu(QPoint)));
    connect(ui->tw_artikeln, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(slotEditArtikel()));

    //PUSHBUTTONS
    connect(ui->pb_cancel, SIGNAL(clicked(bool)), this, SLOT(slotCancel()));
    connect(ui->pb_save, SIGNAL(clicked(bool)), this, SLOT(slotSave()));
    connect(ui->pb_at, SIGNAL(clicked(bool)), this, SLOT(slotAddAt()));
    connect(ui->pb_artikel_save, SIGNAL(clicked(bool)), this, SLOT(slotSaveArtikel()));
    connect(ui->pb_artikel_cancel, SIGNAL(clicked(bool)), this, SLOT(slotCancelArtikel()));
    connect(ui->pb_artikel_neu, SIGNAL(clicked(bool)), this, SLOT(slotNeuArtikel()));

    //TOOLBAR
    connect(m_menuNew, SIGNAL(triggered(bool)), this, SLOT(slotNewUser()));
    connect(m_menuEdit, SIGNAL(triggered(bool)), this, SLOT(slotEditUser()));

    //MENU
    connect(ui->action_close, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->action_refresh, SIGNAL(triggered(bool)), this, SLOT(slotRefresh()));

    //TABWIDGET
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabChanged(int)));

    //LINEEDIT
    connect(ui->le_ref_artikel, SIGNAL(textChanged(QString)), SLOT(slotLoadArtikelBez(QString)));
    connect(ui->le_preis, &QLineEdit::textChanged, this, [this](){ui->le_preis->setText(ui->le_preis->text().replace(",","."));});
}

bool MainWindow::init()
{
    log(8)<<"MainWindow::init(): ...";

    //FIRST GET OPTIONS
    m_adminUser=Options::instance()->getOption("-a")=="1";
    if(Options::instance()->getOption("-k")!="")
        m_kostenstelle=Options::instance()->getOption("-k").toInt();
    m_editor=Options::instance()->getOption("-k")!="";
    m_artikelEditor=Options::instance()->getOption("-e")=="1";

    //DISABLE TABS
    ui->tabWidget->setTabEnabled(0, m_editor || m_adminUser);
    ui->tabWidget->setTabEnabled(1, m_editor || m_adminUser);
    ui->tabWidget->setTabEnabled(2, m_editor || m_adminUser);
    ui->tabWidget->setTabEnabled(3, m_artikelEditor || m_adminUser);

    //WRITE ERROR MSG
    if(!m_artikelEditor && Options::instance()->getOption("-k")=="" && !m_adminUser)
    {
        showMessage(MSG_TITLE_ERROR, QString(MSG_ERR_NO_KOSTENSTELLE).arg(m_adUser), true, this);
        return (false);
    }

    //SET KOSTENSTELLE
    if(m_adminUser && Options::instance()->getOption("-k")=="")
        m_kostenstelle=9993;
    m_cmbKS->setCurrentIndex(m_cmbKS->findData(m_kostenstelle));

    //ENABLE KOSTENSTELLE CMB IF ADMIN USER
    m_cmbKS->setEnabled(m_adminUser);


    //FILL BOOL_AKTIV CMB
    ui->cmb_bool_aktiv->addItem("Aktiv", 1);
    ui->cmb_bool_aktiv->addItem("Inaktiv", 0);

    //FILL KOSTENSTELLEN
    ui->cmb_kostenstelle->addItem("IT/Energie und Umwelt", 9993);
    ui->cmb_kostenstelle->addItem("Technik", 9991);

    //FILL ARTIKEL STATUS
    ui->cmb_artikel_status->addItem("Aktiv", 1);
    ui->cmb_artikel_status->addItem("Inaktiv", 0);

    //FILL ARTIKEL GRUPPEN
    ui->cmb_artikel_gruppe->addItem("Getränk", "GETRAENK");
    ui->cmb_artikel_gruppe->addItem("Nespresso", "NESPRESSO");
    ui->cmb_artikel_gruppe->addItem("Kaffee", "KAFFEE");
    ui->cmb_artikel_gruppe->addItem("Chefteller", "CHEFTELLER");
    ui->cmb_artikel_gruppe->addItem("ILIKE", "ILIKE");

    //SETUP PALETTE
    ui->frm_mitarbeiter->setPalette(Cache::instance()->getPalette());
    ui->frm_mitarbeiter->setEnabled(false);

    //SET LAYOUT MODE
    setLayoutMode(LYT_START);

    //CONNECT SLOTS
    connect_slots();

    //SELECT TAB
    if(m_editor || m_adminUser)
        ui->tabWidget->setCurrentIndex(0);
    else if(m_artikelEditor)
    {
        ui->tabWidget->setCurrentIndex(3);
        slotTabChanged(3);
        ui->toolBar->setEnabled(false);
    }

    //LOAD MITARBEITERS
    slotKSchanged();

    return (true);
}



//MSGBOX
void MainWindow::showMessage(const QString& header, const QString& text, bool iserror, QWidget *parrent)
{
    log(8) << "MainWindow::showMessage(" << header << ", " << text << ", " << iserror << ", " << parrent << ")...";

    //SET PARRENT QWIDGET
    if(parrent==0)
        parrent=this;

    if(iserror)
    {
        warning() << "MainWindow::showMessage(): " << header << ", " << text;
        QMessageBox::warning(parrent,header,text,QMessageBox::Ok);
    }
    else
    {
        log(5) << "MainWindow::showMessage(): " << header << "-" << text;
        QMessageBox::information(this,header,text,QMessageBox::Ok);
    }
}

bool MainWindow::showQuestionMessage(const QString& header, const QString& text, QWidget *parrent)
{
    log(3) << "MainWindow::showQuestionMessage(" << header << "," << text << ")...";
    if(parrent==0)
        parrent=this;

    QMessageBox::StandardButton removeMsg;
    removeMsg=QMessageBox::question(parrent, header, text,QMessageBox::Yes|QMessageBox::No);

    if (removeMsg == QMessageBox::Yes)
    {
        log(5) << "MainWindow::showQuestionMessage(" << header << "," << text << "): User pressed YES!";
        return(true);
    }
    else
    {
        log(5) << "MainWindow::showQuestionMessage(" << header << "," << text << "): User pressed NO!";
        return(false);
    }
}




//DB
bool MainWindow::doCommit(bool silent)
{
    log(6) << "MainWindow::doCommit(" << silent << "): ...";

    HBODbCore::HBOCommit(conWWS);

    return(true);
}

bool MainWindow::doRollback(bool silent)
{
    log(6) << "MainWindow::doRollback(" << silent << "): ...";

    HBODbCore::HBORollback(conWWS, silent);

    return (true);
}



//MODE
void MainWindow::setMode(QString mode)
{
    m_mode=mode;
}

QString MainWindow::getMode()
{
    return (m_mode);
}



//LAYOUT
void MainWindow::clearLayout()
{
    //PERSON
    ui->le_persnr->clear();
    ui->le_vorname->clear();
    ui->le_nachname->clear();
    ui->le_email->clear();

    //ARTIKELN
    ui->le_artikel_nr->clear();
    ui->le_artikel_bez->clear();
    ui->le_ref_artikel->clear();
    ui->lab_ref_artikel_bez->clear();
    ui->cb_it->setChecked(false);
    ui->cb_technik->setChecked(false);

    //QTREEWIDGETS
    ui->tw_konsum->clear();
    ui->tw_artikeln->clear();
}

void MainWindow::setLayoutMode(QString mode)
{
    log(8)<<"MainWindow::setLayoutMode(" << mode << "): ...";
    m_lytMode=mode;

    //DISABLE TOOLBAR ITEMS
    m_menuNew->setEnabled(false);
    m_menuEdit->setEnabled(false);

    //DISABLE EDITING
    ui->frm_mitarbeiter->setEnabled(false);

    //HIDE BUTTONS
    ui->wid_buttons->setVisible(false);

    //ENABLE TREEWIDGETS
    ui->tw_personen->setEnabled(true);
    ui->tw_konsum->setEnabled(true);

    //ENABLE SEARCH
    ui->bow_search->setEnabled(true);

    //ENABLE MENU
    ui->action_close->setEnabled(true);
    ui->action_refresh->setEnabled(true);

    //ARTIKEL
    ui->frm_artikeln->setEnabled(false);
    ui->pb_artikel_neu->setEnabled(true);
    ui->tw_artikeln->setEnabled(true);
    ui->bow_search_artikel->setEnabled(true);

    //START
    if(getLayoutMode()==LYT_START)
    {
        //ENABLE TOOLBAR ITEM NEW
        m_menuNew->setEnabled(true);
    }

    else if(getLayoutMode()==LYT_FOUND)
    {
        //ENABLE TOOLBAR ITEMS
        m_menuNew->setEnabled(true);
        m_menuEdit->setEnabled(true);
    }

    else if(getLayoutMode()==LYT_NEW || getLayoutMode()==LYT_EDIT)
    {
        //ENABLE EDITING
        ui->frm_mitarbeiter->setEnabled(true);

        //DISABLE PERSONALNR EDITING
        ui->le_persnr->setEnabled(getLayoutMode()==LYT_NEW);

        //DISABLE TOOLBAR ITEMS
        m_menuNew->setEnabled(false);
        m_menuEdit->setEnabled(false);

        //SHOW SAVE AND CANCEL BUTTONS
        ui->wid_buttons->setVisible(true);

        //DISABLE TREEWIDGETS
        ui->tw_personen->setEnabled(false);
        ui->tw_konsum->setEnabled(false);

        //DISABLE SEARCH
        ui->bow_search->setEnabled(false);

        //DISABLE MENU
        ui->action_close->setEnabled(false);
        ui->action_refresh->setEnabled(false);
    }

    else if(getLayoutMode()==LYT_NEW_ARTIKEL || getLayoutMode()==LYT_EDIT_ARTIKEL)
    {
        ui->frm_artikeln->setEnabled(true);
        ui->pb_artikel_neu->setEnabled(false);
        ui->tw_artikeln->setEnabled(false);
        ui->bow_search_artikel->setEnabled(false);

        ui->le_artikel_nr->clear();
        ui->le_artikel_bez->clear();
        ui->le_preis->setText("0.00");
        ui->le_priority->setText("0");
        ui->le_ref_artikel->clear();
        ui->lab_ref_artikel_bez->clear();
        ui->cb_it->setChecked(false);
        ui->cb_technik->setChecked(false);
    }
}

QString MainWindow::getLayoutMode()
{
    return (m_lytMode);
}



//GET
bool MainWindow::getJSON(QString url, QString mode)
{
    log()<<"MainWindow::getJSON(" << url << ", " << mode << "): ...";

    //FIRST DISABLE MAINWINDOW
    this->setEnabled(false);

    //SET MODE
    setMode(mode);

    QNetworkReply *reply = m_networkAM->get(QNetworkRequest(QUrl(url)));
    connect(reply, SIGNAL(finished()), this, SLOT(slotReadJSON()));

    return (true);
}

QString MainWindow::getUser(int personalNr)
{
    log(8)<<"MainWindow::getUser(" << personalNr << "): ...";

    QString retval=QString::number(personalNr);

    //LOOP TROUGHT LIST AND FIND MITARBEITER
    for(Person p : m_personenList)
    {
        //GO TO NEXT IF NOT THE SAME PERSONALNR
        if(p.getPersonalNr()!=personalNr)
            continue;

        retval=QString("%1 %2 (%3)")
                .arg(p.getVorname())
                .arg(p.getNachname())
                .arg(p.getPersonalNr());
    }

    if(retval==QString::number(personalNr))
    {
        retval=QString("%1 (%2)")
                .arg(Cache::instance()->getMitarbeiterName(personalNr))
                .arg(personalNr);
    }

    return (retval);
}

void MainWindow::getPaymentsForAll()
{
    log(8)<<"MainWindow::getPaymentsForAll(): ...";

    //FIRST SETUP URL FOR GETTING ALL KONSUM FOR USERS
    QString url=baseUrl;
    url+=QString("&typ=getKonsumForAll&kostenstelle=%1").arg(m_kostenstelle);

    //GET JSON
    getJSON(url, modKonsumForAll);
}



//FILL
bool MainWindow::fillPersonenFromJSON(QJsonDocument jsonDoc)
{
    log(8)<<"MainWindow::fillPersonenFromJSON(QJsonDocument): ...";

    //READ JSON ARRAY
    QJsonArray jsonArray = jsonDoc.array();

    //CLEAR PERSONEN LIST AND TREEWIDGET
    m_personenList.clear();
    ui->tw_personen->clear();

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
        if(m_filteraktivemitarbeiter && !person.getBoolAktiv())
            continue;

        m_personenList.append(person);
    }

    //FILL TREEWIDGET
    for(Person p : m_personenList)
    {
        QTreeWidgetItem *item=new QTreeWidgetItem();
        item->setText(S_PERSON_VORNAME, p.getVorname());
        item->setText(S_PERSON_NACHNAME, p.getNachname());
        item->setText(S_PERSON_PERNR, QString::number(p.getPersonalNr()));

        //SET ICON IF USER NOT ACTIV
        if(!p.getBoolAktiv())
            item->setIcon(0, BO_Icons::instance()->getStandardIcon(BO_Icons::Standard::Standard_Nein));

        ui->tw_personen->insertTopLevelItem(ui->tw_personen->topLevelItemCount(), item);
    }

    //FILL PAYMENTS FOR USERS
    getPaymentsForAll();

    //RESIZE COLUMNS
    ui->tw_personen->header()->resizeSections(QHeaderView::ResizeToContents);

    ui->lab_mitarb_count->setText(QString::number(m_personenList.count()) + " Mitarbeitern");
    log(3)<<"MainWindow::fillPersonenFromJSON(): FLLED <" << m_personenList.count() << "> PERSONEN";

    return (true);
}

bool MainWindow::fillKonsumFromJSON(QJsonDocument jsonDoc)
{
    log(8)<<"MainWindow::fillKonsumFromJSON(QJsonDocument): ...";

    //READ JSON ARRAY
    QJsonArray jsonArray = jsonDoc.array();

    //CLEAR KONSUM LIST AND TREEWIDGET
    m_konsumList.clear();
    ui->tw_konsum->clear();

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

        konsum.setBoolBezalt(obj["boolBezahlt"].toString().toInt());


        konsum.setBezahltAm(QDateTime::fromString(obj["bezahltAm"].toString(), "dd.MM.yyyy hh:mm"));
        konsum.setBezahltBei(obj["bezahltBei"].toString());
        konsum.setDeviceSN(obj["device"].toString());
        konsum.setPreis(obj["preis"].toString().toFloat());

        //FILL KONSUM TO LIST
        m_konsumList.append(konsum);
    }

    //FILL TREEWIDGET
    for(Konsum k : m_konsumList)
    {
        //FIRST GET MONTH NAME
        QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
        QString month=QLocale().monthName(k.getKonsumTime().date().month());

        //DEFINE TREEWIDGET GROUP TITLE
        QString groupName=QString("%1.%2")
                .arg(month)
                .arg(k.getKonsumTime().date().year());

        //DEFINE ITEM
        QTreeWidgetItem *item=nullptr;

        //CHECK GROUP EXIST
        for(int i=0; i<ui->tw_konsum->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *group=ui->tw_konsum->topLevelItem(i);
            if(group->text(0)==groupName)
            {
                item=new QTreeWidgetItem(group);
                group->setExpanded(true);
            }
        }

        //GROUP NOT FOUND CREATE NEW
        if(item==nullptr)
        {
            QTreeWidgetItem *group=new QTreeWidgetItem;
            group->setText(S_KONSUM_KONSUM, groupName);
            group->setData(S_KONSUM_KONSUM, Qt::UserRole, k.getKonsumTime().date().month());
            group->setData(S_KONSUM_ZEIT, Qt::UserRole, k.getKonsumTime().date().year());
            ui->tw_konsum->insertTopLevelItem(ui->tw_konsum->topLevelItemCount(), group);
            group->setExpanded(true);
            item=new QTreeWidgetItem(group);
        }

        //SET FONT COLOR
        Cache::instance()->setItemFontColor(ui->tw_konsum, item);

        //FILL VALUES
        item->setText(S_KONSUM_KONSUM, Cache::instance()->getKonsumName(k.getKonsumTyp()));
        item->setText(S_KONSUM_PREIS, QString::number(k.getPreis(), 'f', 2) + "€");
        item->setText(S_KONSUM_ZEIT, k.getKonsumTime().toString("dd.MM.yyyy hh:mm"));
        item->setText(S_KONSUM_DEVICE, Cache::instance()->getDeviceName(k.getDeviceSN()));

        //FILL DATA
        item->setData(S_KONSUM_KONSUM, Qt::UserRole, k.getBoolBezalt());
        item->setData(S_KONSUM_ZEIT, Qt::UserRole, k.getKonsumTyp());
        item->setData(S_KONSUM_BEZAHLT_AM, Qt::UserRole, k.getBezahltAm().toString("dd.MM.yyyy hh:mm"));
        item->setData(S_KONSUM_BEZAHLT_BEI, Qt::UserRole, k.getBezahltBei());
        item->setData(S_KONSUM_PREIS, Qt::UserRole, k.getPreis());


        //TOOLTIP FOR BESPRECHUNG AND DEFEKT
        if(ui->le_persnr->text()=="815" || ui->le_persnr->text()=="816"
                || ui->le_persnr->text()=="817" || ui->le_persnr->text()=="818")
        {
            QString user=getUser(k.getPeronalNr());
            item->setToolTip(S_KONSUM_KONSUM, user);
        }

        //FILL ITEM TO TREEWIDGET
        ui->tw_konsum->insertTopLevelItem(ui->tw_konsum->topLevelItemCount(), item);
    }

    //FILL PRICE FOR KONSUM PER MOUNTHS
    if(ui->le_persnr->text()!="815" && ui->le_persnr->text()!="816"
            && ui->le_persnr->text()!="817" && ui->le_persnr->text()!="818")
        fillKonsumPrice();

    //RESIZE COLUMNS
    ui->tw_konsum->header()->resizeSections(QHeaderView::ResizeToContents);

    log(3)<<"MainWindow::fillKonsumFromJSON(): FLLED <" << m_konsumList.count() << "> KONSUMS";

    return (true);
}

bool MainWindow::fillKonsumBericht(QJsonDocument jsonDoc)
{
    log(8)<<"MainWindow::fillKonsumBericht(QJsonDocument): ...";

    //READ JSON ARRAY
    QJsonArray jsonArray = jsonDoc.array();

    //CLEAR BERICHT LIST AND TREEWIDGET
    m_konsumBerichtList.clear();
    ui->tw_konsum_bericht->clear();

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

        konsum.setBoolBezalt(obj["boolBezahlt"].toString().toInt());


        konsum.setBezahltAm(QDateTime::fromString(obj["bezahltAm"].toString(), "dd.MM.yyyy hh:mm"));
        konsum.setBezahltBei(obj["bezahltBei"].toString());
        konsum.setPreis(obj["preis"].toString().toFloat());

        //FILL KONSUM TO LIST
        m_konsumBerichtList.append(konsum);
    }

    //FILL TREEWIDGET
    for(Konsum k : m_konsumBerichtList)
    {
        //FIRST GET MONTH NAME
        QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
        QString month=QLocale().monthName(k.getKonsumTime().date().month());

        //DEFINE TREEWIDGET GROUP TITLE
        QString groupName=QString("%1.%2")
                .arg(month)
                .arg(k.getKonsumTime().date().year());

        //DEFINE ITEM
        QTreeWidgetItem *item=nullptr;

        //CHECK GROUP EXIST
        for(int i=0; i<ui->tw_konsum_bericht->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *group=ui->tw_konsum_bericht->topLevelItem(i);
            if(group->text(0)==groupName)
            {
                item=new QTreeWidgetItem(group);
                group->setExpanded(true);
            }
        }

        //GROUP NOT FOUND CREATE NEW
        if(item==nullptr)
        {
            QTreeWidgetItem *group=new QTreeWidgetItem;
            group->setText(S_KONSUM_BERICHT_MONAT, groupName);
            ui->tw_konsum_bericht->insertTopLevelItem(ui->tw_konsum_bericht->topLevelItemCount(), group);
            group->setExpanded(true);
            item=new QTreeWidgetItem(group);
        }

        //SET FONT COLOR
        Cache::instance()->setItemFontColor(ui->tw_konsum_bericht, item);

        //DEFINE FOUND KONSUM
        bool konsumExist=false;

        //FIRST CHECK IF KONSUM EXIST
        for(int i=0; i<item->parent()->childCount(); i++)
        {
            //GET KONSUM ITEM
            QTreeWidgetItem *konsum=item->parent()->child(i);

            if(konsum->data(S_KONSUM_BERICHT_TYP, Qt::UserRole).toInt()==k.getKonsumTyp())
            {
                int count=konsum->text(S_KONSUM_BERICHT_COUNT).toInt();
                konsum->setText(S_KONSUM_BERICHT_COUNT, QString::number(count+1));
                konsumExist=true;
                delete item;
                continue;
            }
        }

        //FILL VALUES
        if(!konsumExist)
        {
            item->setText(S_KONSUM_BERICHT_TYP, Cache::instance()->getKonsumName(k.getKonsumTyp()));
            item->setData(S_KONSUM_BERICHT_TYP, Qt::UserRole, k.getKonsumTyp());
            item->setText(S_KONSUM_BERICHT_COUNT, "1");

            //FILL ITEM TO TREEWIDGET
            ui->tw_konsum_bericht->insertTopLevelItem(ui->tw_konsum_bericht->topLevelItemCount(), item);
        }
    }

    //RESIZE COLUMNS
    ui->tw_konsum_bericht->header()->resizeSections(QHeaderView::ResizeToContents);

    return (true);
}

bool MainWindow::fillFeedbackFromJSON(QJsonDocument jsonDoc)
{
    log(8)<<"MainWindow::fillFeedbackFromJSON(QJsonDocument): ...";

    //READ JSON ARRAY
    QJsonArray jsonArray = jsonDoc.array();

    //CLEAR FEEDBACK LIST AND TREEWIDGET
    m_feedbackList.clear();
    ui->tw_feedback->clear();

    //GO TROUGHT ARRAY AND FILL PERSONEN
    foreach (const QJsonValue & value, jsonArray)
    {
        //CREATE JSON OBJECT
        QJsonObject obj = value.toObject();

        //FILL PERSON CLASS
        Feedback f;

        f.setFeedbackID(obj["id"].toString().toInt());
        f.setFeedbackCode(obj["codFeedback"].toString());
        f.setFeedbackZeit(QDateTime::fromString(obj["feedbackZeit"].toString(), "dd.MM.yyyy hh:mm"));

        //FILL FEEDBACK TO LIST
        m_feedbackList.append(f);
    }

    //CREATE LOCALE
    QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));

    //ADD DAYS TO TREEWIDGET
    for(int i=1; i<=7; i++)
    {
        QString groupName=QLocale().dayName(i);

        QTreeWidgetItem *group=new QTreeWidgetItem;
        group->setText(S_FEEDBACK_FEEDBACK, groupName);
        ui->tw_feedback->insertTopLevelItem(ui->tw_feedback->topLevelItemCount(), group);
    }

    //FILL TREEWIDGET
    for(Feedback f : m_feedbackList)
    {
        //DEFINE ITEM
        QTreeWidgetItem *item=nullptr;
        QString groupName=QLocale().dayName(f.getFeedbackZeit().date().dayOfWeek());

        //CHECK GROUP EXIST
        for(int i=0; i<ui->tw_feedback->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *group=ui->tw_feedback->topLevelItem(i);
            if(group->text(0)==groupName)
            {
                item=new QTreeWidgetItem(group);
                group->setExpanded(true);
            }
        }

        //SET FONT COLOR
        Cache::instance()->setItemFontColor(ui->tw_feedback, item);

        //FILL VALUES
        item->setText(S_FEEDBACK_FEEDBACK, Cache::instance()->getFeedbackVoteName(f.getFeedbackCode()));
        item->setData(S_FEEDBACK_FEEDBACK, Qt::UserRole, f.getFeedbackCode());
        item->setIcon(S_FEEDBACK_FEEDBACK, Cache::instance()->getFeedbackIcon(f.getFeedbackCode()));
        item->setText(S_FEEDBACK_ZEIT, f.getFeedbackZeit().toString("dd.MM.yyyy hh:mm"));

        //FILL ITEM TO TREEWIDGET
        ui->tw_feedback->insertTopLevelItem(ui->tw_feedback->topLevelItemCount(), item);
    }

    //DEFINE LIST FOR REMOVING
    QList<QTreeWidgetItem*> removingList;

    //CLEAR EMPTY GROUPS AND WRITE PERCENTS
    for(int i=0; i<ui->tw_feedback->topLevelItemCount(); i++)
    {
        //DEFINE ITEM
        QTreeWidgetItem *item=ui->tw_feedback->topLevelItem(i);

        //GET COUNT
        int count=item->childCount();

        //DAY HAS NO FEEDBACKS
        if(count==0)
        {
            removingList.append(item);
            continue;
        }

        QMap<int, int> fbMap;
        //WRITE PERCENTS
        for(int x=0; x<item->childCount(); x++)
        {
            //DEFINE ITEM CHILD
            QTreeWidgetItem *feedback=item->child(x);

            //KONSUM TYP
            int fb=feedback->data(S_FEEDBACK_FEEDBACK, Qt::UserRole).toInt();
            if(!fbMap.contains(fb))
                fbMap[fb]=1;
            else
                fbMap[fb]=fbMap[fb]+1;
        }

        //WRITE PERCENTS
        double percent=100 / (double) count;

        QStringList text;
        QMap<int, int>::iterator it;
        for (it = fbMap.begin(); it != fbMap.end(); ++it)
        {
            double result=it.value() * percent;
            text.append(QString("%1 - %2%")
                        .arg(Cache::instance()->getFeedbackVoteName(QString::number(it.key())))
                        .arg(QString::number(result, 'f', 2)));
        }
        item->setToolTip(S_FEEDBACK_FEEDBACK, text.join("\n"));
    }

    //REMOVE ITEMS
    for(QTreeWidgetItem *item : removingList)
        delete item;

    //RESIZE COLUMNS
    ui->tw_feedback->header()->resizeSections(QHeaderView::ResizeToContents);

    log(3)<<"MainWindow::fillFeedbackFromJSON(): FLLED <" << m_feedbackList.count() << "> FEEDBACKS";

    return (true);
}

bool MainWindow::fillArtikelnFromJSON(QJsonDocument jsonDoc)
{
    log(8)<<"MainWindow::fillArtikelnFromJSON(QJsonDocument): ...";

    //READ JSON ARRAY
    QJsonArray jsonArray = jsonDoc.array();

    //CLEAR ARTIKEL LIST AND TREEWIDGET
    m_artikelList.clear();
    ui->tw_artikeln->clear();
    m_artikelMap.clear();

    //GO TROUGHT ARRAY AND FILL PERSONEN
    foreach (const QJsonValue & value, jsonArray)
    {
        //CREATE JSON OBJECT
        QJsonObject obj = value.toObject();

        //FILL ARTIKEL CLASS
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

        //FILL ARTIKEL TO LIST
        m_artikelList.append(a);
    }

    //CREATE LOCALE
    QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));

    //FILL TREEWIDGET
    for(Artikel a : m_artikelList)
    {
        QTreeWidgetItem *item=new QTreeWidgetItem;
        item->setText(S_ARTIKEL_NR, QString::number(a.get_artikelNr()));
        item->setText(S_ARTIKEL_BEZ, a.get_artikelBez());
        item->setText(S_ARTIKEL_GRUPPE, Cache::instance()->getGruppeName(a.get_gruppe()));
        item->setText(S_ARTIKEL_PRIORITY, QString::number(a.get_priority()));
        item->setText(S_ARTIKEL_PREIS, QString::number(a.get_preis(), 'f', 2) + "€");
        item->setText(S_ARTIKEL_REF_ARTIKEL, a.get_refArtikel()==-1 ? "" : QString("%1 - %2").arg(a.get_refArtikel()).arg(m_artikelMap[a.get_refArtikel()].get_artikelBez()));
        item->setText(S_ARTIKEL_IT, a.get_it() ? "Ja" : "Nein");
        item->setText(S_ARTIKEL_TECHNIK, a.get_technik() ? "Ja" : "Nein");
        item->setText(S_ARTIKEL_ENABLED, a.get_enabled() ? "Aktiv" : "Inaktiv");

        //FILL ITEM TO TREEWIDGET
        ui->tw_artikeln->insertTopLevelItem(ui->tw_artikeln->topLevelItemCount(), item);
    }

    //RESIZE COLUMNS
    ui->tw_artikeln->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tw_artikeln->sortByColumn(S_ARTIKEL_NR, Qt::AscendingOrder);

    log(3)<<"MainWindow::fillArtikelnFromJSON(): FLLED <" << m_artikelList.count() << "> ARTIKELN";

    return (true);
}

void MainWindow::fillKonsumPrice()
{
    log(8)<<"MainWindow::fillKonsumPrice(): ...";


    //LOOP TROUGHT LIST AND SET PRICE
    for (int i=0; i<ui->tw_konsum->topLevelItemCount(); i++)
    {
        //DEFINE MOUNTH ITEM
        QTreeWidgetItem *month=ui->tw_konsum->topLevelItem(i);

        if(month->childCount()==0)
            continue;

        float finalSum=0.00;
        bool bezahlt=true;

        QString bezahltAm;
        QString bezahltBei;
        QMap<int, int> konsumMap;

        //GO TROUGHT CHILDREN
        for(int x=0; x<month->childCount(); x++)
        {
            //DEFINE KONSUM ITEM
            QTreeWidgetItem *konsum=month->child(x);

            int konsumTyp=konsum->data(S_KONSUM_ZEIT, Qt::UserRole).toInt();
            if(!konsumMap.contains(konsumTyp))
                konsumMap[konsumTyp]=1;
            else
                konsumMap[konsumTyp]=konsumMap[konsumTyp] + 1;

            //ADD TO FINAL PRICE
            //finalSum=finalSum+Cache::instance()->getKonsumPrice(konsum->data(S_KONSUM_DEVICE, Qt::UserRole).toFloat());
            finalSum=finalSum+konsum->data(S_KONSUM_PREIS, Qt::UserRole).toFloat();

            //CHECK IF BEZAHLT
            if(konsum->data(S_KONSUM_KONSUM, Qt::UserRole).toInt()!=1)
                bezahlt=false;

            bezahltAm=konsum->data(S_KONSUM_BEZAHLT_AM, Qt::UserRole).toString();
            bezahltBei=konsum->data(S_KONSUM_BEZAHLT_BEI, Qt::UserRole).toString();

        }

        QString monthText=month->text(S_KONSUM_KONSUM);
        QString bezaltText=" - Bezahlt";
        if(!bezahlt)
            bezaltText="";

        //GET KONSUM BERICHT
        QString konsumBericht;
        QMap<int, int>::iterator it;
        for (it = konsumMap.begin(); it != konsumMap.end(); ++it)
            konsumBericht+=Cache::instance()->getKonsumName(it.key()) + ": " + QString::number(it.value()) + "x \n";

        month->setText(S_KONSUM_KONSUM, QString("%1 (%2€%3)").arg(monthText).arg(QString::number(finalSum, 'f', 2)).arg(bezaltText));
        if(bezahlt)
            month->setToolTip(S_KONSUM_KONSUM, QString ("Bei %1 am %2").arg(Cache::instance()->getMitarbeiterName(bezahltBei)).arg(bezahltAm));

        month->setToolTip(S_KONSUM_KONSUM, konsumBericht.remove(konsumBericht.length()-2, 2));
    }
}

void MainWindow::fillPaymentsForUsers(QJsonDocument jsonDoc)
{
    log(8)<<"MainWindow::fillPaymentsForUsers(QJsonDocument): ...";

    //READ JSON ARRAY
    QJsonArray jsonArray = jsonDoc.array();

    //DEFINE KONSUM LIST
    QList<Konsum> konsumList;

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

        //FILL KONSUM TO LIST
        konsumList.append(konsum);
    }

    //LOOP TROUGHT USERS AND FILL PAYMENTS
    for(int i=0; i<ui->tw_personen->topLevelItemCount(); i++)
    {
        //DEFINE ITEM
        QTreeWidgetItem *userItem=ui->tw_personen->topLevelItem(i);

        //GET PERSONAL NR
        int personalNr=userItem->text(S_PERSON_PERNR).toInt();

        //DEFINE KONSUM SUM
        double konsumPrice=0.0;

        //LOOP TROUGHT LIST AND FILL MAP WITH KONSUM
        for(Konsum k : konsumList)
        {
            if(k.getPeronalNr()!=personalNr)
                continue;

            konsumPrice=konsumPrice+k.getPreis();
        }

        //ADD CHILD
        if(konsumPrice>0 && personalNr!=815 && personalNr!=816)
            userItem->setText(S_PERSON_PAYMENT, QString("%2€").arg(QString::number(konsumPrice, 'f', 2)));
    }

}

Person MainWindow::fillPersonFromGUI()
{
    log(8)<<"MainWindow::fillPersonFromGUI(): ...";

    Person p;

    //PERSONAL NR
    p.setPersonalNr(ui->le_persnr->text().toInt());

    //VORNAME
    p.setVorname(ui->le_vorname->text());

    //NACHNAME
    p.setNachname(ui->le_nachname->text());

    //EMAIL
    p.setEmail(ui->le_email->text());

    //BOOL_AKTIV
    p.setBoolAktiv(ui->cmb_bool_aktiv->currentData().toInt());

    //KOSTENSTELLE
    p.setKostenstelle(ui->cmb_kostenstelle->currentData().toInt());

    return (p);
}




//SLOTS
void MainWindow::slotKSchanged()
{
    if(!m_editor && !m_adminUser)
        return;
    log(3)<<"MainWindow::slotKSchanged(): ...";

    m_kostenstelle=m_cmbKS->currentData().toInt();
    getJSON(baseUrl + QString("&typ=getAllUsers&kostenstelle=%1").arg(m_kostenstelle), modAllUsers);

    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::slotFilterChanged()
{
    log(8)<<"MainWindow::slotFilterChanged(): ...";

    m_filteraktivemitarbeiter=m_cmbfilter->currentData().toBool();
    slotKSchanged();
}

bool MainWindow::slotReadJSON()
{
    log(8)<<"MainWindow::slotReadJSON(): ...";

    //FIRST ENABLE MAINWINDOW
    this->setEnabled(true);

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    //IF NO ERROR
    if(reply->error()==QNetworkReply::NoError)
    {
        //READ JSON
        QString strReply = (QString)reply->readAll();

        //MAKE JSON DOCUMENT
        QJsonDocument document = QJsonDocument::fromJson(strReply.toUtf8());

        //FILL PERSONEN
        if(getMode()==modAllUsers)
            fillPersonenFromJSON(document);

        //FILL ALL KONSUM
        else if (getMode()==modAllKonsum)
            fillKonsumFromJSON(document);

        //FILL KONSUM BERICHT
        else if(getMode()==modKonsumBericht)
            fillKonsumBericht(document);

        //FILL FEEDBACK
        else if(getMode()==modGetFeedback)
            fillFeedbackFromJSON(document);

        //FILL ARTIKELN
        else if(getMode()==modArtikel)
            fillArtikelnFromJSON(document);

        //NEW EDIT ARTIKEL
        else if(getMode()==modNewArtikel || getMode()==modEditArtikel)
        {
            if(strReply!="OK")
            {
                showMessage(MSG_TITLE_ERROR, MSG_ERR_BY_SAVING, true, this);
                return (false);
            }

            //RELOAD USERS AND SET LAYOUT MODE
            getJSON(baseUrl + QString("&typ=getArtikels"), modArtikel);
            setLayoutMode(LYT_START);
        }


        //NEW EDIT
        else if(getMode()==modNewUser || getMode()==modEditUser)
        {
            if(strReply!="OK")
            {
                showMessage(MSG_TITLE_ERROR, MSG_ERR_BY_SAVING, true, this);
                return (false);
            }

            //RELOAD USERS AND SET LAYOUT MODE
            getJSON(baseUrl + QString("&typ=getAllUsers&kostenstelle=%1").arg(m_kostenstelle), modAllUsers);
            setLayoutMode(LYT_FOUND);
        }

        //FILL PAYMENTS FOR USERS
        else if(getMode()==modKonsumForAll)
            fillPaymentsForUsers(document);

        //SET PAYED
        else if(getMode()==modSetPayed)
        {
            if(strReply!="OK")
            {
                showMessage(MSG_TITLE_ERROR, MSG_ERR_BY_SETTING_AS_PAYED, true, this);
                return (false);
            }

            //RELOAD KONSUM
            getJSON(baseUrl + QString("&perNr=%1&typ=getAllKonsum").arg(ui->le_persnr->text()), modAllKonsum);
        }

        delete reply;
    }
    else
    {
        critical()<<"MainWindow::slotGetJSONData(): ERROR by getting JSON data: " << reply->error();
        showMessage(MSG_TITLE_ERROR, QString(MSG_ERR_QNETWORKREPLY).arg(reply->error()), true, this);
    }

    return (true);
}

void MainWindow::slotLoadUser(QTreeWidgetItem *item, int index)
{
    log(8)<<"MainWindow::slotLoadUser(QTreeWidgetItem *, " << index << "): ...";

    //BREAK IF IN NEW OR EDIT MODE
    if(getLayoutMode()=="NEW" || getLayoutMode()=="EDIT")
        return;

    //FIRST CLEAR LAYOUT
    clearLayout();

    //GET PERSONAL NR
    int personalNr=item->text(2).toInt();

    for(Person p : m_personenList)
    {
        if(p.getPersonalNr()!=personalNr)
            continue;

        //FILL USER DATA
        ui->le_persnr->setText(QString::number(p.getPersonalNr()));
        ui->le_vorname->setText(p.getVorname());
        ui->le_nachname->setText(p.getNachname());
        ui->le_email->setText(p.getEmail());

        ui->cmb_bool_aktiv->setCurrentIndex(ui->cmb_bool_aktiv->findData(p.getBoolAktiv()));
        ui->cmb_kostenstelle->setCurrentIndex(ui->cmb_kostenstelle->findData(p.getKostenstelle()));
    }

    //GET ALL KONSUM
    QString typ="getAllKonsum";
    if(personalNr==816 || personalNr==818)
        typ="getBesprechungKonsum";
    else if(personalNr==815 || personalNr==817)
        typ="getDefektKonsum";
    getJSON(baseUrl + QString("&perNr=%1&typ=%2&kostenstelle=%3").arg(personalNr).arg(typ).arg(m_kostenstelle), modAllKonsum);

    //SET LAYOUT MODE TO FOUND
    setLayoutMode(LYT_FOUND);
}

void MainWindow::slotNewUser()
{
    log(8)<<"MainWindow::slotNewUser(): ...";

    //SET TAB TO 1
    ui->tabWidget->setCurrentIndex(0);

    //CLEAR LAYOUT
    clearLayout();

    setLayoutMode(LYT_NEW);
}

void MainWindow::slotRefresh()
{
    log(8)<<"MainWindow::slotRefresh(): ...";

    //CLEAR LAYOUT
    clearLayout();

    if(m_editor || m_adminUser)
    {
        //GET ALLUSERS
        getJSON(baseUrl + "&typ=getAllUsers&kostenstelle="+ui->cmb_kostenstelle->currentData().toString(), modAllUsers);

        //SELECT FIRST TAB
        ui->tabWidget->setCurrentIndex(0);
    }
    else if(m_artikelEditor)
    {
        //GET ALLUSERS
        getJSON(baseUrl + "&typ=getArtikels", modArtikel);

        //SELECT FIRST TAB
        ui->tabWidget->setCurrentIndex(3);
    }

    //SET LAYOUT MODE
    setLayoutMode(LYT_START);

}

void MainWindow::slotEditUser()
{
    log(8)<<"MainWindow::slotEditUser(): ...";

    setLayoutMode(LYT_EDIT);
}

void MainWindow::slotCancel()
{
    log(8)<<"MainWindow::slotCancel(): ...";

    //CLEAR LAYOUT
    clearLayout();

    //SET LAYOUT MODE TO START
    setLayoutMode(LYT_START);
}

void MainWindow::slotSave()
{
    log(8)<<"MainWindow::slotSave(): ...";

    //CHECK PERSONAL NR EMPTY
    if(ui->le_persnr->text()=="")
    {
        showMessage(MSG_TITLE_ERROR, MSG_ERR_PERSONALNR_NOT_ENTERED, true, this);
        return;
    }

    //FIL PERSON FROM GUI
    Person p=fillPersonFromGUI();

    //GET TYP
    QString typ;
    if(getLayoutMode()==LYT_NEW)
    {
        typ="&typ=addUser";

        //CHECK PERSONAL NR EXIST
        for(Person per : m_personenList)
        {
            if(per.getPersonalNr()==p.getPersonalNr())
            {
                showMessage(MSG_TITLE_ERROR, MSG_ERR_PERSONALNR_EXIST, true, this);
                return;
            }
        }
    }
    else if (getLayoutMode()==LYT_EDIT)
        typ="&typ=editUser";

    //SETUP URL
    QString url=baseUrl;
    url+=typ;
    url+="&perNr=" + QString::number(p.getPersonalNr());
    url+="&vorname=" + p.getVorname();
    url+="&nachname=" + p.getNachname();
    url+="&email=" + p.getEmail();
    int aktiv=p.getBoolAktiv();
    url+="&aktiv=" + QString::number(aktiv);
    url+="&kostenstelle=" + QString::number(p.getKostenstelle());

    log()<<"MainWindow::slotSave(): GOT URL FOR SAVING <" << url << ">";

    //DEFINE MODE
    QString mode;
    if(getLayoutMode()==LYT_NEW)
        mode=modNewUser;
    else if (getLayoutMode()==LYT_EDIT)
        mode=modEditUser;

    //GET JSON FROM URL
    getJSON(url, mode);
}

void MainWindow::slotShowKonsumMenu(const QPoint pos)
{
    log(8)<<"MainWindow::slotShowKonsumMenu(" << pos << "): ...";

    //DISABLE MENU IF NO STATUS ROLE
    if(getLayoutMode()!=LYT_FOUND)
        return;

    //DISBLE MENU IF FOR DEFEKT OR BESPRECHUNG
    if(ui->le_persnr->text()=="816" || ui->le_persnr->text()=="815"
            || ui->le_persnr->text()=="817" || ui->le_persnr->text()=="818")
        return;

    //DEFINE ITEM CLICKED
    QTreeWidgetItem *item = ui->tw_konsum->itemAt(pos);

    //DEFINE MENUS
    QMenu *menu = new QMenu();
    QAction *menuSetPayed=new QAction(QIcon(ICON_PAYED),"Bezahlt",this);
    menu->addAction(menuSetPayed);

    QAction *menuSetUnpayed=new QAction(QIcon(ICON_UNPAYED),"Offen",this);
    menu->addAction(menuSetUnpayed);

    //DEFINE ITEM SELECTED
    bool itemSelected=item;
    if(!itemSelected)
        return;

    //DISABLE MENU SET UNPAYED
    menuSetUnpayed->setEnabled(false);

    //DISABLE IF NOT ON MONTH
    if(itemSelected && item->childCount()==0)
    {
        menuSetPayed->setEnabled(false);
        menuSetUnpayed->setEnabled(false);
        return;
    }

    //FIRST GET DATE
    if(itemSelected && item->childCount()>0)
    {
        //BREAK IF BEZAHLT
        if(item->text(S_KONSUM_KONSUM).endsWith("Bezahlt)"))
        {
            menuSetPayed->setEnabled(false);
            menuSetUnpayed->setEnabled(true);
        }

        //DEFINE CURRENT DATE
        QDate currentDate=QDate::currentDate();

        //GET MONTH AND YEAR FROM DATA
        int month=item->data(S_KONSUM_KONSUM, Qt::UserRole).toInt();
        int year=item->data(S_KONSUM_ZEIT, Qt::UserRole).toInt();

        if(month>=currentDate.month() && year>currentDate.year())
            menuSetPayed->setEnabled(false);

        if(month==currentDate.month() && year==currentDate.year())
            menuSetPayed->setEnabled(false);
    }

    QAction *selectedMenu = menu->exec(ui->tw_konsum->mapToGlobal(pos));

    //DEFINE AS PAYED
    if(selectedMenu==menuSetPayed)
    {
        //GET MONTH AND YEAR FROM DATA
        int month=item->data(S_KONSUM_KONSUM, Qt::UserRole).toInt();
        int year=item->data(S_KONSUM_ZEIT, Qt::UserRole).toInt();

        //DEFINE URL
        QString url=baseUrl;
        url+="&typ=setPayed";
        url+=QString("&perNr=%1").arg(ui->le_persnr->text());
        url+=QString("&month=%1").arg(month);
        url+=QString("&year=%1").arg(year);
        url+=QString("&bezahltBei=%1").arg(m_adUser);

        //RUN FUNCTION
        getJSON(url, modSetPayed);
    }

    //DEFINE AS UNPAYED
    else if(selectedMenu==menuSetUnpayed)
    {
        //GET MONTH AND YEAR FROM DATA
        int month=item->data(S_KONSUM_KONSUM, Qt::UserRole).toInt();
        int year=item->data(S_KONSUM_ZEIT, Qt::UserRole).toInt();

        //DEFINE URL
        QString url=baseUrl;
        url+="&typ=setUnpayed";
        url+=QString("&perNr=%1").arg(ui->le_persnr->text());
        url+=QString("&month=%1").arg(month);
        url+=QString("&year=%1").arg(year);

        //RUN FUNCTION
        getJSON(url, modSetPayed);
    }

}

void MainWindow::slotTabChanged(int currentTab)
{
    log()<<"MainWindow::slotTabChanged(" << currentTab << "): ...";

    //SET LAYOUT MODE
    setLayoutMode(LYT_START);

    //CLER WINDOW
    clearLayout();

    //BREAK IF TAB1
    if(currentTab==0)
        return;

    //DEFINE URL AND MODE
    QString url;
    QString mode;

    if(currentTab==1)
    {
        url=baseUrl;
        url+=QString("&typ=getKonsumBericht&kostenstelle=%1").arg(m_kostenstelle);

        mode=modKonsumBericht;
    }
    else if(currentTab==2)
    {
        url=baseUrl;
        url+=QString("&typ=getFeedback&kostenstelle=%1").arg(m_kostenstelle);

        mode=modGetFeedback;
    }
    else if(currentTab==3)
    {
        url=baseUrl;
        url+=QString("&typ=getArtikels");

        mode=modArtikel;
    }

    //GET JSON
    getJSON(url, mode);
}

void MainWindow::slotAddAt()
{
    log(8)<<"MainWindow::slotAddAt(): ...";

    ui->le_email->setText(ui->le_email->text() + "@");
}

void MainWindow::slotLoadArtikelBez(QString artikel_nr)
{
    if(m_artikelMap.contains(artikel_nr.toInt()))
        ui->lab_ref_artikel_bez->setText(m_artikelMap[artikel_nr.toInt()].get_artikelBez());
    else
        ui->lab_ref_artikel_bez->clear();
}

void MainWindow::slotSaveArtikel()
{
    log(8)<<"MainWindow::slotSaveArtikel(): ...";

    QString typ;
    if(getLayoutMode()==LYT_NEW_ARTIKEL)
    {
        typ="&typ=addArtikel";

        if(m_artikelMap.contains(ui->le_artikel_nr->text().toInt()))
        {
            showMessage(MSG_TITLE_ERROR, QString(MSG_ERR_ARTIKEL_EXIST).arg(ui->le_artikel_nr->text()), true, this);
            return;
        }
    }
    else if (getLayoutMode()==LYT_EDIT_ARTIKEL)
        typ="&typ=editArtikel";

    //SETUP URL
    QString url=baseUrl;
    url+=typ;
    url+="&artikel_nr=" + ui->le_artikel_nr->text();
    url+="&artikel_bez=" + ui->le_artikel_bez->text();
    url+="&gruppe=" + ui->cmb_artikel_gruppe->currentData().toString();
    url+="&priority=" + (ui->le_priority->text()=="" ? "0" : ui->le_priority->text());
    url+="&preis=" + ui->le_preis->text().simplified();
    url+="&ref_artikel=" + (ui->le_ref_artikel->text().isEmpty()? "-1" : ui->le_ref_artikel->text());
    int it=ui->cb_it->isChecked();
    url+="&bool_it=" + QString::number(it);
    int technik=ui->cb_technik->isChecked();
    url+="&bool_technik=" + QString::number(technik);
    url+="&bool_enabled=" + QString::number(ui->cmb_artikel_status->currentData().toInt());

    log()<<"MainWindow::slotSaveArtikel(): GOT URL FOR SAVING ARTIKEL <" << url << ">";

    //DEFINE MODE
    QString mode;
    if(getLayoutMode()==LYT_NEW_ARTIKEL)
        mode=modNewArtikel;
    else if (getLayoutMode()==LYT_EDIT_ARTIKEL)
        mode=modEditArtikel;

    //GET JSON FROM URL
    getJSON(url, mode);
}

void MainWindow::slotCancelArtikel()
{
    log(8)<<"MainWindow::slotCancelArtikel(): ...";
    setLayoutMode(LYT_START);
    slotRefresh();
}

void MainWindow::slotNeuArtikel()
{
    log(8)<<"MainWindow::slotNeuArtikel(): ...";
    setLayoutMode(LYT_NEW_ARTIKEL);
}

void MainWindow::slotEditArtikel()
{
    log(8)<<"MainWindow::slotEditArtikel(): ...";

    if(!ui->tw_artikeln->currentItem())
        return;

    int artikel=ui->tw_artikeln->currentItem()->text(S_ARTIKEL_NR).toInt();
    setLayoutMode(LYT_EDIT_ARTIKEL);

    Artikel a = m_artikelMap[artikel];
    ui->le_artikel_nr->setText(QString::number(a.get_artikelNr()));
    ui->le_artikel_bez->setText(a.get_artikelBez());
    ui->le_ref_artikel->setText(a.get_refArtikel()==-1 ? "" : QString::number(a.get_refArtikel()));
    ui->cmb_artikel_gruppe->setCurrentIndex(ui->cmb_artikel_gruppe->findData(a.get_gruppe()));
    ui->le_priority->setText(QString::number(a.get_priority()));
    ui->le_preis->setText(QString::number(a.get_preis(), 'f', 2));
    ui->cb_it->setChecked(a.get_it());
    ui->cb_technik->setChecked(a.get_technik());
    ui->cmb_artikel_status->setCurrentIndex(ui->cmb_artikel_status->findData(a.get_enabled()));

}

//EOF
