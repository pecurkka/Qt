#include <QMessageBox>	//für QT_REQUIRE_VERSION
#include <QDebug>

#include "mainwindow.h"


int main( int argc, char ** argv )
{
    qDebug("\nThis is: %s (%s %s) (Qt %s).\n", argv[0], "MPREIS mLIST", "2.0", qVersion());

    Application a (argc, argv);

    if (qApp) {

        if (!a.init())
            return 1;

        int retval = a.exec();

        return retval;
    }
    else
    {
        qDebug ("qAPP liess sich nicht instanzieren!\n");
        return 1;
    }

    return 0;
}


//EOF
