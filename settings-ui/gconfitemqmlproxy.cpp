#include "gconfitemqmlproxy.h"

GConfItemQmlProxy::GConfItemQmlProxy(QObject *parent) :
   QObject(parent)
{
   m_gconfItem = NULL;
   m_key = "";
}

const QString & GConfItemQmlProxy::key()
{
   return m_key;
}

void GConfItemQmlProxy::setKey(const QString &newKey)
{
   if (m_gconfItem) {
       delete m_gconfItem;
   }
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
