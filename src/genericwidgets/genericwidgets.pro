include($$top_srcdir/common.pri)

QT += widgets

TEMPLATE = lib
CONFIG += staticlib


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clockwidget.cpp \
    genericwidgets.cpp \
    timerwidget.cpp

HEADERS += \
    clockwidget.h \
    genericwidgets.h \
    timerwidget.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
