include($$top_srcdir/common.pri)

TEMPLATE = lib
CONFIG += static
QT += core gui widgets sql xml printsupport

TARGET = libastrogenerator
DESTDIR = $$top_builddir/lib

HEADERS += ActionsFenetre.h \
           Calculastro.h \
           Carteciel.h \
           CompteRebours.h \
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
           homewidget.h

SOURCES += ActionsFenetre.cpp \
           Calculastro.cpp \
           Carteciel.cpp \
           CompteRebours.cpp \
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
           homewidget.cpp

TRANSLATIONS += \
    astroGenerator_en.ts \
    astroGenerator_fr.ts

unix|win32: LIBS += -L$$top_builddir/lib/ -llibastrocalc

INCLUDEPATH += $$top_srcdir/src/libastrocalc
DEPENDPATH += $$top_srcdir/src/libastrocalc

win32:!win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/libastrocalc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/liblibastrocalc.a

FORMS += \
    homewidget.ui
