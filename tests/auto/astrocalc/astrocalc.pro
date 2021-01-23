QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    tst_astrocalc.cpp
    
unix|win32: LIBS += -L$$top_builddir/lib/ -llibastrocalc

INCLUDEPATH += $$top_srcdir/src/libastrocalc
DEPENDPATH += $$top_srcdir/src/libastrocalc

win32:!win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/libastrocalc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$top_builddir/lib/liblibastrocalc.a
