TEMPLATE = app
TARGET = calendarfeed-ui
VERSION = 0.3.0

QT += declarative dbus

CONFIG += qdeclarative-boostable

INCLUDEPATH += . \
                /usr/include/gq

LIBS += -lgq-gconf


#install
target.path = /opt/calendarfeed-ui/bin

settingsdesktop.path = /usr/share/duicontrolpanel/desktops
settingsdesktop.files = calendarfeed.desktop

settingsxml.path = /usr/share/duicontrolpanel/uidescriptions
settingsxml.files = calendarfeed.xml

desktopfile.path = /usr/share/applications
desktopfile.files = calendarfeed-ui.desktop


INSTALLS += target settingsxml settingsdesktop desktopfile

SOURCES += \
    main.cpp \
    gconfitemqmlproxy.cpp

OTHER_FILES += \
    qml/main.qml \
    qml/MainPage.qml \
    qml/PageTitle.qml \
    qml/SwitchSetting.qml

RESOURCES += \
    qml/qml.qrc

HEADERS += \
    gconfitemqmlproxy.h









