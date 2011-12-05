TEMPLATE = app
TARGET = calendarfeed-ui
VERSION = 0.3.6

QT += declarative dbus

CONFIG += qdeclarative-boostable

INCLUDEPATH += . \
                /usr/include/gq

LIBS += -lgq-gconf


#install
target.path = /opt/calendarfeed-ui/bin

settingsdesktop.path = /usr/share/duicontrolpanel/desktops
settingsdesktop.files = calendarfeed.desktop

desktopfile.path = /usr/share/applications
desktopfile.files = calendarfeed-ui.desktop

splash.files = resources/calendar-feed-splash.png
splash.path = /opt/calendarfeed-ui/resources

INSTALLS += target settingsdesktop desktopfile splash

SOURCES += \
    main.cpp \
    gconfitemqmlproxy.cpp \
    settingshelper.cpp

OTHER_FILES += \
    qml/main.qml \
    qml/MainPage.qml \
    qml/PageTitle.qml \
    qml/SwitchSetting.qml \
    qml/SliderSetting.qml \
    qml/UIConstants.js \
    qml/SettingsGroup.qml \
    qml/SwitchableSliderSetting.qml \
    qml/AboutPage.qml \
    qml/TextFieldSetting.qml \
    qml/SelectionSetting.qml

RESOURCES += \
    qml/qml.qrc \
    resources/resources.qrc

HEADERS += \
    gconfitemqmlproxy.h \
    settingshelper.h





















