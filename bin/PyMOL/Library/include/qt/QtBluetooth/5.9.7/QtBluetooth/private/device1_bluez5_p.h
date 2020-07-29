/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -p device1_bluez5 -v org.bluez.Device1.xml
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef DEVICE1_BLUEZ5_H_1396951960
#define DEVICE1_BLUEZ5_H_1396951960

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.bluez.Device1
 */
class OrgBluezDevice1Interface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.bluez.Device1"; }

public:
    OrgBluezDevice1Interface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgBluezDevice1Interface();

    Q_PROPERTY(QDBusObjectPath Adapter READ adapter)
    inline QDBusObjectPath adapter() const
    { return qvariant_cast< QDBusObjectPath >(property("Adapter")); }

    Q_PROPERTY(QString Address READ address)
    inline QString address() const
    { return qvariant_cast< QString >(property("Address")); }

    Q_PROPERTY(QString Alias READ alias WRITE setAlias)
    inline QString alias() const
    { return qvariant_cast< QString >(property("Alias")); }
    inline void setAlias(const QString &value)
    { setProperty("Alias", QVariant::fromValue(value)); }

    Q_PROPERTY(ushort Appearance READ appearance)
    inline ushort appearance() const
    { return qvariant_cast< ushort >(property("Appearance")); }

    Q_PROPERTY(bool Blocked READ blocked WRITE setBlocked)
    inline bool blocked() const
    { return qvariant_cast< bool >(property("Blocked")); }
    inline void setBlocked(bool value)
    { setProperty("Blocked", QVariant::fromValue(value)); }

    Q_PROPERTY(uint Class READ classProperty)
    inline uint classProperty() const
    { return qvariant_cast< uint >(property("Class")); }

    Q_PROPERTY(bool Connected READ connected)
    inline bool connected() const
    { return qvariant_cast< bool >(property("Connected")); }

    Q_PROPERTY(QString Icon READ icon)
    inline QString icon() const
    { return qvariant_cast< QString >(property("Icon")); }

    Q_PROPERTY(bool LegacyPairing READ legacyPairing)
    inline bool legacyPairing() const
    { return qvariant_cast< bool >(property("LegacyPairing")); }

    Q_PROPERTY(QString Modalias READ modalias)
    inline QString modalias() const
    { return qvariant_cast< QString >(property("Modalias")); }

    Q_PROPERTY(QString Name READ name)
    inline QString name() const
    { return qvariant_cast< QString >(property("Name")); }

    Q_PROPERTY(bool Paired READ paired)
    inline bool paired() const
    { return qvariant_cast< bool >(property("Paired")); }

    Q_PROPERTY(short RSSI READ rSSI)
    inline short rSSI() const
    { return qvariant_cast< short >(property("RSSI")); }

    Q_PROPERTY(bool Trusted READ trusted WRITE setTrusted)
    inline bool trusted() const
    { return qvariant_cast< bool >(property("Trusted")); }
    inline void setTrusted(bool value)
    { setProperty("Trusted", QVariant::fromValue(value)); }

    Q_PROPERTY(QStringList UUIDs READ uUIDs)
    inline QStringList uUIDs() const
    { return qvariant_cast< QStringList >(property("UUIDs")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> CancelPairing()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("CancelPairing"), argumentList);
    }

    inline QDBusPendingReply<> Connect()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Connect"), argumentList);
    }

    inline QDBusPendingReply<> ConnectProfile(const QString &UUID)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(UUID);
        return asyncCallWithArgumentList(QLatin1String("ConnectProfile"), argumentList);
    }

    inline QDBusPendingReply<> Disconnect()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Disconnect"), argumentList);
    }

    inline QDBusPendingReply<> DisconnectProfile(const QString &UUID)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(UUID);
        return asyncCallWithArgumentList(QLatin1String("DisconnectProfile"), argumentList);
    }

    inline QDBusPendingReply<> Pair()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Pair"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace org {
  namespace bluez {
    typedef ::OrgBluezDevice1Interface Device1;
  }
}
#endif
