#ifndef MPREIS_2384_STAMMGS_05_APPLICATION_H
#define MPREIS_2384_STAMMGS_05_APPLICATION_H

#include <QApplication>
#include <hbocore.h>
#include <hbodbcore.h>
#include <hbooptions.h>
#include <hbooptionsAPM.h>
#include <bo_mitarbeiter.h>
#include <application_global.h>

#include <mainwindow.h>

QT_BEGIN_NAMESPACE
class MainWindow;
QT_END_NAMESPACE

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv );
    virtual ~Application();

private:



public:
    int exec();
    bool init();
    bool initOptions();

    void setLogLevel(int loglevel);
    int getLogLevel(void) const;


private:
    static Application          *c_instance;

    bool                        m_isInitialized;
    MainWindow                  *m_mainwindow;
    int                         m_logLevel;

    int                         m_userKey;
};


#endif /* MPREIS_2384_STAMMGS_05_APPLICATION_H */

//EOF
