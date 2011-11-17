TEMPLATE = lib
TARGET = calendarfeed-client
VERSION = 0.1.0

DEPENDPATH += .
INCLUDEPATH += . \
                /usr/include/libsynccommon \
                /usr/include/libsyncprofile \
                /usr/include/gq

LIBS += -lsyncpluginmgr -lsyncprofile -lgq-gconf

CONFIG += debug plugin meegotouchevents

CONFIG += mobility
MOBILITY += organizer

QT -= gui

#input
SOURCES += \
    calendarfeedplugin.cpp

HEADERS +=\
    calendarfeedplugin.h

QMAKE_CXXFLAGS = -Wall \
    -g \
    -Wno-cast-align \
    -O2 -finline-functions

#install
target.path = /usr/lib/sync/

client.path = /etc/sync/profiles/client
client.files = xml/calendarfeed.xml

sync.path = /etc/sync/profiles/sync
sync.files = xml/sync/calendarfeed.xml

service.path = /etc/sync/profiles/service
service.files = xml/service/calendarfeed.xml

settingsdesktop.path = /usr/share/duicontrolpanel/desktops
settingsdesktop.files = settings/calendarfeed.desktop

settingsxml.path = /usr/share/duicontrolpanel/uidescriptions
settingsxml.files = settings/calendarfeed.xml

INSTALLS += target client sync service settingsdesktop settingsxml

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_harmattan/postinst \
    qtc_packaging/debian_harmattan/prerm
