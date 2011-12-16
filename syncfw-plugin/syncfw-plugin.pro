TEMPLATE = lib
TARGET = calendarfeed-client
VERSION = 0.4.6

DEPENDPATH += .
INCLUDEPATH += . \
                /usr/include/libsynccommon \
                /usr/include/libsyncprofile \
                /usr/include/gq \
                /usr/include/mkcal \
                /usr/include/kcalcoren

LIBS += -lsyncpluginmgr -lsyncprofile -lgq-gconf -lmkcal

CONFIG += debug plugin meegotouchevents meegotouch

CONFIG += mobility
MOBILITY += organizer

QT += dbus

#input
SOURCES += \
    calendarfeedplugin.cpp \
    calendarevent.cpp \
    settings.cpp

HEADERS +=\
    calendarfeedplugin.h \
    calendarevent.h \
    settings.h

QMAKE_CXXFLAGS = -Wall \
    -g \
    -Wno-cast-align \
    -O2 -finline-functions

#install
target.path = /usr/lib/sync/

INSTALLS += target





