#include "application.h"
#ifdef __GNUC__
static const char __attribute__((unused)) *Id = "$Id: main.cpp 2989 2018-03-29 08:00:00Z draganm $";
#else
static char *Id = "$Id: main.cpp 2989 2018-03-29 08:00:00Z draganm $";
#endif

using namespace  hbo;

int main( int argc, char ** argv)
{
    qDebug("\nMSG : This is: %s (%s %s) (Qt %s).\n",argv[0], "MPREIS mLIST JOB", "1.0", qVersion());

    Application a(argc,argv);
    int retval=0;

    if (qApp) {
        retval=a.init();

        if(retval==0)
            retval=a.exec();
    }
    else
    {
        qDebug ("qAPP liess sich nicht instanzieren!\n");
        retval=2;
    }

    log()<<"main retval <" << retval << ">";

    return retval;
}

/* eof */
