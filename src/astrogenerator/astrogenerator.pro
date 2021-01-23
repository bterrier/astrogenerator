include($$top_srcdir/common.pri)

TEMPLATE = app
TARGET = astroGenerator

QT += core gui widgets sql xml printsupport

INCLUDEPATH += .
DESTDIR = $$top_builddir

# Input
HEADERS += 
SOURCES += main.cpp



RC_ICONS = $$top_srcdir/logoastrogenerator.ico


INSTALLS += ag_txt

ag_txt.path = $$top_builddir
ag_txt.files += \
    $$top_srcdir/*.txt \
    $$top_srcdir/icones \
    $$top_srcdir/VSOP87 \
    $$top_srcdir/dbastrogenerator \
    $$top_srcdir/*.ico \
    $$top_srcdir/doc.pdf

RESOURCES += \
    $$top_srcdir/resources.qrc

LIBS += -L$$top_builddir/lib
unix|win32: LIBS += -llibastrogenerator

INCLUDEPATH += $$top_srcdir/src/libastrogenerator
DEPENDPATH += $$top_srcdir/src/libastrogenerator

win32:!win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/libastrogenerator.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/liblibastrogenerator.a

unix|win32: LIBS += -llibastrocalc

INCLUDEPATH += $$top_srcdir/src/libastrocalc
DEPENDPATH += $$top_srcdir/src/libastrocalc

win32:!win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/libastrocalc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/liblibastrocalc.a
