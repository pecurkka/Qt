#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "application.h"
#include <application_global.h>
#include <cache.h>
#include <person.h>
#include <konsum.h>
#include <feedback.h>
#include <artikel.h>

#include <QNetworkProxy>
#include <QNetworkReply>
#include <QMessageBox>
#include <QScreen>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProgressDialog>


#include <QActionGroup>
#include <QAction>

#include <hbocore.h>
#include <hbodbcore.h>
#include <hbolog.h>

#include <bo_icons.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString adUser);
    virtual ~MainWindow();


private:
    //SETUP
    void setup_mw();
    void setup_treeWidgets();
    void setup_toolBar();
    void connect_slots();
public:
    bool init();

private:
    //MSGBOX
    void showMessage(const QString& header, const QString& text, bool iserror, QWidget *parrent);
    bool showQuestionMessage(const QString& header, const QString& text, QWidget *parrent);

public:
    //DB
    bool doCommit(bool silent);
    bool doRollback(bool silent);

private:
    //MODE
    void setMode(QString mode);
    QString getMode();

    //LAYOUT
    void clearLayout();
    void setLayoutMode(QString mode);
    QString getLayoutMode();

    //GET
    bool getJSON(QString url, QString mode);
    QString getUser(int personalNr);
    void getPaymentsForAll();

    //FILL
    bool fillPersonenFromJSON(QJsonDocument jsonDoc);
    bool fillKonsumFromJSON(QJsonDocument jsonDoc);
    bool fillKonsumBericht(QJsonDocument jsonDoc);
    bool fillFeedbackFromJSON(QJsonDocument jsonDoc);
    bool fillArtikelnFromJSON(QJsonDocument jsonDoc);
    void fillKonsumPrice();
    void fillPaymentsForUsers(QJsonDocument jsonDoc);

    Person fillPersonFromGUI();

private slots:
    //SLOTS
    void slotKSchanged();
    void slotFilterChanged();
    bool slotReadJSON();
    void slotLoadUser(QTreeWidgetItem *item, int index);
    void slotNewUser();
    void slotEditUser();

    void slotRefresh();
    void slotCancel();
    void slotSave();

    void slotShowKonsumMenu(const QPoint pos);

    void slotTabChanged(int currentTab);
    void slotAddAt();

    void slotLoadArtikelBez(QString artikel_nr);
    void slotSaveArtikel();
    void slotCancelArtikel();
    void slotNeuArtikel();
    void slotEditArtikel();



public:
    Ui::MainWindow                  *ui;

    //NETWORK
    QNetworkAccessManager           *m_networkAM;

    //TOOLBAR
    QAction     *m_menuNew;
    QAction     *m_menuEdit;
    QComboBox   *m_cmbKS;
    QComboBox   *m_cmbfilter;

    //MODE
    QString m_mode;

    //LAYOUT MODE
    QString m_lytMode;

    //CACHE
    QList<Person>   m_personenList;
    QList<Konsum>   m_konsumList;
    QList<Konsum>   m_konsumBerichtList;
    QList<Feedback> m_feedbackList;
    QList<Artikel>  m_artikelList;
    QMap<int, Artikel> m_artikelMap;

    //USER
    QString  m_adUser;

    //ROLEN
    bool    m_adminUser;
    bool    m_editor;
    bool    m_artikelEditor;
    int     m_kostenstelle;
    bool    m_filteraktivemitarbeiter;


};

#endif // MAINWINDOW_H

//EOF
