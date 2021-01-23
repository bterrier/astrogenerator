######################################################################
# Automatically generated by qmake (3.1) Fri Jan 1 02:04:27 2021
######################################################################

TEMPLATE = app
TARGET = astroGenerator

CONFIG += \
          lrelease \
          embed_translations \
          c++17

QT += core gui widgets sql xml printsupport

INCLUDEPATH += .
DESTDIR = $$top_builddir

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
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
           WidgetHeure.h
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
           main.cpp \
           Objet.cpp \
           ObjetCP.cpp \
           ObjetCPObs.cpp \
           ObjetObs.cpp \
           ObjetPlaneteObs.cpp \
           Onglet.cpp \
           Soiree.cpp \
           WidgetHeure.cpp

TRANSLATIONS += \
    astroGenerator_en.ts \
    astroGenerator_fr.ts

RC_ICONS = $$top_srcdir/logoastrogenerator.ico
VERSION = 3.0.0
DEFINES += VERSION=$${VERSION} \
           VERSION_STRING=\\\"$${VERSION}\\\"

INSTALLS += ag_txt

ag_txt.path = $$top_builddir
ag_txt.files += \
    $$PWD/../*.txt \
    $$PWD/../icones \
    $$PWD/../VSOP87 \
    $$PWD/../dbastrogenerator \
    $$PWD/../*.ico \
    $$PWD/../doc.pdf

RESOURCES += \
    ../resources.qrc

