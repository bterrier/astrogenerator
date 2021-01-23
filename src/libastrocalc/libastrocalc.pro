include($$top_srcdir/common.pri)

TEMPLATE = lib
CONFIG += static
QT += core
QT -= gui
DESTDIR = $$top_builddir/lib

HEADERS += \
    astrocalc.h

SOURCES += \
    astrocalc.cpp


