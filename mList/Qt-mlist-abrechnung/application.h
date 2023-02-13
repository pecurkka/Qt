#ifndef HAF_APPLICATION_H
#define HAF_APPLICATION_H

#include <QCoreApplication>

#include <application_globals.h>

#include <hbocore.h>
#include <hbolog.h>
#include <hbooptions.h>
#include <hbooptionsAPM.h>

#include <mlistabrechnung.h>

class mListAbrechnung;

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv);
    virtual ~Application();

private:
    Q_DISABLE_COPY(Application)

public:
    static Application* Instance();

    int exec(void);
    int init(void);
	

private:
	static Application* _instance;
    int abrechnen(void);


private:
    bool                m_initialized;

    mListAbrechnung     *m_mlistabrechnung;

};

#endif /* HAF_APPLICATION_H */
