#define _CRT_SECURE_NO_WARNINGS
#include <QDebug>

#include <hbocore.h>
#include <hbodbcore.h>
#include <hbolog.h>
#include <hbooptions.h>

#include "application.h"

#ifdef __GNUC__
static const char __attribute__((unused)) *Id = "$Id: application.cpp 3022 2018-03-29 08:00:00Z draganm $";
#else
static char *Id = "$Id: application.cpp 3022 2018-03-29 08:00:00Z draganm $";
#endif

using namespace hbo;

Application* Application::_instance = NULL;

Application::Application (int &argc, char **argv)  :
    QCoreApplication( argc, argv ),
    m_mlistabrechnung(NULL)
{    
}

Application::~Application()
{

    log() << "Destroying Application";
    _instance = NULL;
}

Application* Application::Instance()
{
    Q_ASSERT(Application::_instance);

    if (!Application::_instance->m_initialized)
    {
        if (!Application::_instance->init())
            return NULL;
    }

    return Application::_instance;
}

int Application::init(void)
{
    HBOLog::setup(9,-1,"yyyyMMddhhmmss.zzz");
    m_initialized = true;

    //DEFINE OPTIONS
    OptionsAPM myoptions;
    myoptions.setOption("WeekExport", "-w", "", Options::NoArg, "", "Week Export from ILIKE (Only IT)");
    if(!myoptions.parseCmdLine())
        return(21);

    return (0);
}

int Application::exec(void)
{
    log(8)  << "Application::exec() ...";

    return(abrechnen());
}


int Application::abrechnen(void)
{
    log(6) << "Application::abrechnen(): ...";

    int import=0;

    m_mlistabrechnung = new mListAbrechnung(this, this);

    bool weekExport=Options::instance()->getOption("-w")=="1";
    if(weekExport)
        log()<<"STARTING WEEK EXPORT FROM ILIKE KONSUM (ONLY IT)";

    if(!weekExport)
    {
        if(!m_mlistabrechnung->exec())
        {
            import=1;
            goto ende;
        }
    }
    else
    {
        if(!m_mlistabrechnung->weekExport())
        {
            import=2;
            goto ende;
        }
    }

ende:

    log(3)<<"Application::abrechnen(): RETVAL <" << import << ">";

    //DELETE ABRECHNUNG CLASS
    delete m_mlistabrechnung;

    return(import);
}

/* eof */
