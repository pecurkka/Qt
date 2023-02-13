message("qmake mlist_abrechnung.pro")
#--------------------------------------------
SOURCELIBPATH = .
DEVELOPMENTROOT= ../Qt-AllLibs
INSTALLLIBPATH = $${DEVELOPMENTROOT}/lib
INSTALLINCPATH = $${DEVELOPMENTROOT}/include
INSTALLDOCPATH = $${DEVELOPMENTROOT}/doc
#--------------------------------------------
INCLUDEPATH += ./ $${INSTALLINCPATH}
LIBS += -L$${INSTALLLIBPATH} \
        -lhbolog \
        -lhbooptions \
        -lhbodbcore \
        -lhbocore \
        -lhbosettings \
        -ldb_appmanager \
        -lbo_mitarbeiter \
        -ldb_mitarbeiter \
        -ldb_mitarbeiter_lvs \
        -lbo_icons \
        -lbow_search \
        -lsmtpclient \
        -lm

QMAKE_CXXFLAGS += -std=c++0x -static-libstdc++
QMAKE_LFLAGS += -static-libstdc++


QT += 	sql \
      	network \
        dbus \
        core


TARGET = mlist_abrechnung
HEADERS += \
        application.h \
        application_globals.h \
        artikel.h \
        cache.h \
        mlistabrechnung.h \
        person.h \
        konsum.h \
        abrechnung.h

SOURCES += \
        artikel.cpp \
        main.cpp \
        application.cpp \
        cache.cpp \
        mlistabrechnung.cpp \
        person.cpp \
        konsum.cpp \
        abrechnung.cpp

TRANSLATIONS += $${TARGET}_de.ts
#--------------------------------------------
UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj
# -----------------------------------------------------
LANGUAGE = C++
TEMPLATE = app

#--------------------------------------------
# DEPENDS
#--------------------------------------------
#PRE_TARGETDEPS      += modules
QMAKE_EXTRA_TARGETS += modules
modules.commands = installModules.sh \
                    ../Qt-AllLibs/db/db_appmanager.pro \
                    ../Qt-AllLibs/bo_mitarbeiter/bo_mitarbeiter.pro \
                    ../Qt-AllLibs/bow_search/bow_search.pro \
                    ../Qt-AllLibs/bo_helpers/bo_helpers.pro \
                    ../Qt-AllLibs/bo_icons/bo_icons.pro \
                    ../Qt-AllLibs/bow_search/bow_search.pro \
                    ../Qt-AllLibs/SmtpClient/smtpclient.pro



# -----------------------------------------------------
# INSTALL IN PRODUCTION-ROOT MAIA (dont forget SVN add/commit!!!)
CONFIG(release,debug|release) {

   TEMPLATE = app

   INSTALLTMPPATH = /home/hafina/production/tmp
   INSTALLPATH = /home/hafina/production/batches

   INSTALLHOST = hermes

   executable.files = $${INSTALLTARGET}
   executable.path  = $${INSTALLTMPPATH}
   executable.extra = cp $${TARGET} $${INSTALLTMPPATH}/$${TARGET}; installRelease.sh -host $${INSTALLHOST} -path $${INSTALLPATH} $${TARGET}

   INSTALLS += executable
   DEFINES -=CL_DEBUG

   message(Unique Logfiles)
   message(Release Build - $${TARGET} will be installed in $${INSTALLHOST}:/$${INSTALLPATH} if make is called with -install)
}
else: {
    DEFINES+=CL_DEBUG
    message(Debug Build Only - Rotating Logfiles!)
}

message(CONFIG  = $$CONFIG)
message(DEFINES = $$DEFINES)
message(qmake done!)
