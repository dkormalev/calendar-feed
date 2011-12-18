TEMPLATE = app
TARGET = calendarfeed-ui
VERSION = 0.4.6

QT += declarative dbus

CONFIG += link_pkgconfig
PKGCONFIG += libmkcal

CONFIG += qdeclarative-boostable

INCLUDEPATH += . \
                /usr/include/gq \
                /usr/include/mkcal \
                /usr/include/kcalcoren

LIBS += -lgq-gconf -lmkcal

#install
target.path = /opt/calendarfeed-ui/bin

settingsdesktop.path = /usr/share/duicontrolpanel/desktops
settingsdesktop.files = calendarfeed.desktop

desktopfile.path = /usr/share/applications
desktopfile.files = calendarfeed-ui.desktop

splash.files = resources/calendar-feed-splash.png
splash.path = /opt/calendarfeed-ui/resources

icon.files = resources/calendar-feed-icon_80.png
icon.path = /usr/share/icons/hicolor/80x80/apps

INSTALLS += target settingsdesktop desktopfile splash icon

SOURCES += \
    main.cpp \
    gconfitemqmlproxy.cpp \
    settingshelper.cpp \
    calendar.cpp

OTHER_FILES += \
    qml/main.qml \
    qml/pages/MainPage.qml \
    qml/elements/PageTitle.qml \
    qml/elements/SwitchSetting.qml \
    qml/elements/SliderSetting.qml \
    qml/logic/UIConstants.js \
    qml/elements/SettingsGroup.qml \
    qml/elements/SwitchableSliderSetting.qml \
    qml/pages/AboutPage.qml \
    qml/elements/TextFieldSetting.qml \
    qml/elements/SelectionSetting.qml \
    qml/elements/SelectionWithCustomSetting.qml \
    qml/elements/SliderElement.qml \
    qml/pages/BehaviorPage.qml \
    qml/pages/AppearancePage.qml \
    qml/elements/LabelPageActivator.qml \
    qml/elements/ButtonElement.qml \
    qml/elements/MultiSelectionSetting.qml \
    qml/pages/TodosPage.qml

RESOURCES += \
    qml/qml.qrc \
    resources/resources.qrc

HEADERS += \
    gconfitemqmlproxy.h \
    settingshelper.h \
    calendar.h




