#define _CRT_SECURE_NO_WARNINGS

#include <hbooptions.h>
#include <bo_mitarbeiter.h>
#include <application.h>

#include <application_global.h>


using namespace hbo;

Application* Application::c_instance = NULL;
BO_Mitarbeiter ActiveUser;

Application::Application(int &argc, char **argv) :
    QApplication( argc, argv )
{
    Q_ASSERT(!Application::c_instance);
    c_instance = this;

    setQuitOnLastWindowClosed(true);
}

Application::~Application()
{
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents,1);
}

bool Application::init()
{
    //Init HBO and Graphics
    HBOCore::HBOInit();
    HBOLog::setup(1, -1, "hh:mm");

    if(!initOptions())
        return false;

    m_isInitialized = true;
    return (m_isInitialized);
}

int Application::exec(void)
{
    if (!HBODbCore::createOracleConnection(conWWS, "ContainerTracking", HBODbCore::WWS, conWWS))
    {
        critical() << "Application: Cannot get connection for local Database (serves as WWS)";
        exit (1);
    }


    if (!BO_Mitarbeiter::activeUser(&ActiveUser, conWWS))
    {
        log() << "Kann den user nicht feststellen";
        return (false);
    }
    log() << "ActiveUser = " << ActiveUser.name();


    //DEFINE MAINWINDOW
    m_mainwindow = new MainWindow(ActiveUser.adUser());

    if(!m_mainwindow->init())
    {
        critical()<<"Application::exec()... Failed to init Mainwindow!";
        return (1);
    }

    //SHOW MAINWINDOW
    m_mainwindow->show();

    return (QApplication::exec());
}

bool Application::initOptions()
{
    //SET LOG LEVEL
    setLogLevel(3);

    QString opt;
    OptionsAPM myoptions;

    //CREATE OPTIONS
    myoptions.setOption("ADMINISTRATOR",    "-a", "--admin",    Options::NoArg,     "0",    "Administrator");
    myoptions.setOption("KOSTENSTELLE",     "-k", "--ks",       Options::Number,    "",     "Start mit kostenstelle");
    myoptions.setOption("ARTIKEL-EDITOR",     "-e", "--artikeleditor",       Options::NoArg,    "0",     "Artikel editor");

    //PARSE COMMAND LINE
    if(!myoptions.parseCmdLine())
    {
        if(myoptions.helpRequested())
            return (false);

        return(false);
    }

    //GET DEFAULT DEBUG OPTION AND SET DEBUG LEVEL
    opt=myoptions.getOption("-d");
    if(opt.toInt()<=0)
    {
        warning() << "Application::initOptions()... Invalid debug level - setting it to 3!";
        m_logLevel=3;
    }
    else
        m_logLevel=opt.toInt();

    HBOLog::setDebugLevel(m_logLevel);
    log()<<"Application::initOptions()... LOGLEVEL set to <"  <<m_logLevel<<">";

    return (true);
}



void Application::setLogLevel(int loglevel)
{
    m_logLevel=loglevel;
    HBOLog::setDebugLevel(m_logLevel);
    log() << "Application::setLogLevel(" << loglevel << "): LOGLEVEL <" << m_logLevel << "> set";
}

int Application::getLogLevel(void) const
{
    return (m_logLevel);
}
//EOF
