include($$top_srcdir/common.pri)

TEMPLATE = lib
CONFIG += static
QT += core gui widgets sql xml printsupport

TARGET = libastrogenerator
DESTDIR = $$top_builddir/lib

HEADERS += ActionsFenetre.h \
           Calculastro.h \
           Carteciel.h \
           Constantes.h \
           Diaporama.h \
           FenCreerSoiree.h \
           FenetreBDD.h \
           FenInfosCreation.h \
           FenPersonaliser.h \
           FenPreferences.h \
           FenPrincipal.h \
           HauteurMaxTemps.h \
           Interface.h \
           InterfaceCreation.h \
           InterfaceLecture.h \
           Objet.h \
           ObjetCP.h \
           ObjetCPObs.h \
           ObjetObs.h \
           ObjetPlaneteObs.h \
           Onglet.h \
           Soiree.h \
           WidgetHeure.h \
           aboutdialog.h \
           homewidget.h \
           skymapdialog.h

SOURCES += ActionsFenetre.cpp \
           Calculastro.cpp \
           Carteciel.cpp \
           Diaporama.cpp \
           FenCreerSoiree.cpp \
           FenetreBDD.cpp \
           FenInfosCreation.cpp \
           FenPersonaliser.cpp \
           FenPreferences.cpp \
           FenPrincipal.cpp \
           HauteurMaxTemps.cpp \
           Interface.cpp \
           InterfaceCreation.cpp \
           InterfaceLecture.cpp \
           Objet.cpp \
           ObjetCP.cpp \
           ObjetCPObs.cpp \
           ObjetObs.cpp \
           ObjetPlaneteObs.cpp \
           Onglet.cpp \
           Soiree.cpp \
           WidgetHeure.cpp \
           aboutdialog.cpp \
           homewidget.cpp \
           skymapdialog.cpp

TRANSLATIONS += \
    astroGenerator_en.ts \
    astroGenerator_fr.ts

unix|win32: LIBS += -L$$top_builddir/lib/ -llibastrocalc

INCLUDEPATH += $$top_srcdir/src/libastrocalc
DEPENDPATH += $$top_srcdir/src/libastrocalc

win32:!win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/libastrocalc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/liblibastrocalc.a

FORMS += \
    aboutdialog.ui \
    homewidget.ui \
    skymapdialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../genericwidgets/release/ -lgenericwidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../genericwidgets/debug/ -lgenericwidgets
else:unix: LIBS += -L$$OUT_PWD/../genericwidgets/ -lgenericwidgets

INCLUDEPATH += $$PWD/../genericwidgets
DEPENDPATH += $$PWD/../genericwidgets

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../genericwidgets/release/libgenericwidgets.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../genericwidgets/debug/libgenericwidgets.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../genericwidgets/release/genericwidgets.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../genericwidgets/debug/genericwidgets.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../genericwidgets/libgenericwidgets.a
