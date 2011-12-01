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
