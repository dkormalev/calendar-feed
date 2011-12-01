TEMPLATE = subdirs
SUBDIRS = syncfw-plugin settings-ui
VERSION = 0.3.0

#install
system ("cd translations && lrelease -markuntranslated '' -idbased *.ts")

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

translations.path = /usr/share/l10n/meegotouch
translations.files += translations/*qm

INSTALLS += client sync service settingsdesktop settingsxml translations

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
