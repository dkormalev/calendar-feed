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

#ifndef GCONFITEMQMLPROXY_H
#define GCONFITEMQMLPROXY_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <gconfitem.h>

class GConfItemQmlProxy : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QString key READ key WRITE setKey)
   Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
   Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE setDefaultValue)

public:
   explicit GConfItemQmlProxy(QObject *parent = 0);

   const QString &key();
   void setKey(const QString &newKey);
   const QVariant &value();
   void setValue(const QVariant &newValue);
   const QVariant &defaultValue();
   void setDefaultValue(const QVariant &newValue);

signals:
   void valueChanged();

public slots:
   void updateValue();

private:
   GConfItem *m_gconfItem;
   QString m_key;
   QVariant m_value;
   QVariant m_defaultValue;
};

#endif // GCONFITEMQMLPROXY_H
