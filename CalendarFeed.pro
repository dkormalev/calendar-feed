TEMPLATE = subdirs
SUBDIRS = syncfw-plugin settings-ui
VERSION = 0.4.6

#install
system ("cd translations && lrelease -markuntranslated '' -idbased *.ts")

client.path = /etc/sync/profiles/client
client.files = xml/calendarfeed.xml

sync.path = /etc/sync/profiles/sync
sync.files = xml/sync/calendarfeed.xml

service.path = /etc/sync/profiles/service
service.files = xml/service/calendarfeed.xml

translations.path = /usr/share/l10n/meegotouch
translations.files += translations/*qm


INSTALLS += client sync service translations

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
