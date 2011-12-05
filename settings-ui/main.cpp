/**************************************************************************
**
** This file is part of Calendar Feed
**
** Copyright (C) 2011  Denis Kormalev <kormalev.denis@gmail.com>
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** This application is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
**
**************************************************************************/

#include <QtGui/QApplication>

#include <QtDeclarative>
#include <MDeclarativeCache>
#include <QLocale>
#include <QTranslator>
#include "gconfitemqmlproxy.h"
#include "settingshelper.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(MDeclarativeCache::qApplication(argc, argv));
    QScopedPointer<QDeclarativeView> view(MDeclarativeCache::qDeclarativeView());

    QTranslator translator;
    bool useTranslator = false;
    QString localePrefix = QLocale::system().name();
    if (localePrefix.length() > 2)
        localePrefix = localePrefix.left(2);
    if (translator.load("calendarfeed_" + localePrefix, "/usr/share/l10n/meegotouch/")) {
        app->installTranslator(&translator);
    } else if (translator.load("calendarfeed", "/usr/share/l10n/meegotouch/")) {
        app->installTranslator(&translator);
    }

    qmlRegisterType<GConfItemQmlProxy>("CalendarFeed", 1, 0, "GConfItem");
    view->rootContext()->setContextProperty("settingsHelper", new SettingsHelper(app.data()));
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->setAttribute(Qt::WA_LockPortraitOrientation, true);

    view->setSource(QUrl("qrc:/main.qml"));
    QObject::connect(view->engine(), SIGNAL(quit()), view.data(), SLOT(close()));
    view->showFullScreen();


    return app->exec();
}

