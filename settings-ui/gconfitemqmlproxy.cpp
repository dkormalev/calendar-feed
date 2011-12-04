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

#include "gconfitemqmlproxy.h"

GConfItemQmlProxy::GConfItemQmlProxy(QObject *parent) :
    QObject(parent)
{
    m_gconfItem = 0;
    m_key = "";
}

const QString & GConfItemQmlProxy::key()
{
    return m_key;
}

void GConfItemQmlProxy::setKey(const QString &newKey)
{
    if (m_key == newKey)
            return;
    if (m_gconfItem)
        delete m_gconfItem;
    m_gconfItem = new GConfItem(newKey);
    m_key = newKey;

    connect(m_gconfItem, SIGNAL(valueChanged()), this, SLOT(updateValue()));
}

const QVariant & GConfItemQmlProxy::value()
{
    m_value = m_gconfItem->value(m_defaultValue);
    return m_value;
}

void GConfItemQmlProxy::setValue(const QVariant &newValue)
{
    if (m_gconfItem) {
        m_gconfItem->set(newValue);
        m_value = newValue;
    }
}

const QVariant & GConfItemQmlProxy::defaultValue()
{
    return m_defaultValue;
}

void GConfItemQmlProxy::setDefaultValue(const QVariant &newValue)
{
    m_defaultValue = newValue;
}

void GConfItemQmlProxy::updateValue()
{
    m_value = m_gconfItem->value(m_defaultValue);
    emit valueChanged();
}
