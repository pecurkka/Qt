message("qmake mlist.pro")
#--------------------------------------------
SOURCELIBPATH = .
DEVELOPMENTROOT= ../Qt-AllLibs
INSTALLLIBPATH = $${DEVELOPMENTROOT}/lib
INSTALLINCPATH = $${DEVELOPMENTROOT}/include
INSTALLDOCPATH = $${DEVELOPMENTROOT}/doc
#--------------------------------------------
INCLUDEPATH += ./ $${INSTALLINCPATH}
LIBS += -L$${INSTALLLIBPATH} \
    -lhbocore \
    -lhbodbcore \
    -lhbolog \
    -lhbooptions \
    -lhbosettings \
    -lbo_mitarbeiter \
    -ldb_mitarbeiter \
    -ldb_mitarbeiter_lvs \
    -ldb_appmanager \
    -lbo_icons \
    -lbow_search \
    -lm

TARGET = mlist

QMAKE_CXXFLAGS += -std=c++0x

QT += gui \
      sql \
      network \
      widgets \
      positioning \
      webengine \
      webenginewidgets

HEADERS	+= \
        application.h \
        artikel.h \
        mainwindow.h \
        application_global.h \
        cache.h \
        person.h \
        konsum.h \
        feedback.h


SOURCES	+= \
        application.cpp \
        artikel.cpp \
        mainwindow.cpp \
        main.cpp \ 
        cache.cpp \
        person.cpp \
        konsum.cpp \
        feedback.cpp

FORMS += \
        mainwindow.ui

# -----------------------------------------------------
TRANSLATIONS += $${TARGET}_de.ts
LANGUAGE = C++
TEMPLATE = app
#--------------------------------------------
UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

#--------------------------------------------
# DEPENDS
#--------------------------------------------
#PRE_TARGETDEPS      += modules
QMAKE_EXTRA_TARGETS += modules
modules.commands = installModules.sh \
                    ../Qt-AllLibs/installcore.pro \
                    ../Qt-AllLibs/db/db_appmanager.pro \
                    ../Qt-AllLibs/bo_mitarbeiter/bo_mitarbeiter.pro \
                    ../Qt-AllLibs/bow_search/bow_search.pro \
                    ../Qt-AllLibs/bo_helpers/bo_helpers.pro \
                    ../Qt-AllLibs/bo_icons/bo_icons.pro \
                    ../Qt-AllLibs/bow_search/bow_search.pro


# -----------------------------------------------------
CONFIG(release,debug|release) {

   TEMPLATE = app

   INSTALLTMPPATH = /home/hafina/production/tmp
   INSTALLPATH = /home/hafina/production/bin

   INSTALLHOST = hermes

   executable.files = $${INSTALLTARGET}
   executable.path  = $${INSTALLTMPPATH}
   executable.extra = cp $${TARGET} $${INSTALLTMPPATH}/$${TARGET}; installRelease.sh -host $${INSTALLHOST} $${TARGET}

   INSTALLS += executable
   DEFINES -=CL_DEBUG

   message(Unique Logfiles)
   message(Release Build - $${TARGET} will be installed in $${INSTALLHOST}:/$${INSTALLPATH} if make is called with -install)
}
else: {
    DEFINES+=CL_DEBUG
    message(Debug Build Only - Rotating Logfiles!)
}

message(CONFIG  = $$CONFIG)l
message(DEFINES = $$DEFINES)
message(qmake done!)

RESOURCES += \
    qrc_mlist.qrc

