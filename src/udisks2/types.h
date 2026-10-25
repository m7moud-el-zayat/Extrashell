#pragma once

#include <qbytearray.h>
#include <qcontainerfwd.h>
#include <qdbusargument.h>
#include <qflags.h>
#include <qlist.h>
#include <qmap.h>
#include <qmetatype.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <qvariant.h>

namespace extrashell::udisks2 {

class ActionAvailability {
    Q_GADGET

    Q_PROPERTY(bool availability MEMBER availability)
    Q_PROPERTY(QString binaryName MEMBER binaryName)

  public:
    bool availability;
    QString binaryName;

    bool operator==(const ActionAvailability&) const = default;
};

QDBusArgument &operator<<(QDBusArgument& argument, const ActionAvailability& actionAvailability);
const QDBusArgument &operator>>(const QDBusArgument& argument, ActionAvailability& actionAvailability);

class ResizeFlags : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

  public:
    enum Flag : quint8 {
        OfflineShrink = 2,
        OfflineGrow = 4,
        OnlineShrink = 8,
        OnlineGrow = 16
    };

    Q_ENUM(Flag)
    Q_DECLARE_FLAGS(Flags, Flag)
    Q_FLAG(Flags)

    Q_INVOKABLE static QString toString(Flags flags);
};

class ResizeAvailability {
    Q_GADGET

    Q_PROPERTY(bool availability MEMBER availability)
    Q_PROPERTY(ResizeFlags::Flags flags MEMBER flags)
    Q_PROPERTY(QString binaryName MEMBER binaryName)

  public:
    bool availability;
    ResizeFlags::Flags flags;
    QString binaryName;

    bool operator==(const ResizeAvailability&) const = default;
};

QDBusArgument &operator<<(QDBusArgument& argument, const ResizeAvailability& resizeAvailability);
const QDBusArgument &operator>>(const QDBusArgument& argument, ResizeAvailability& resizeAvailability);

class BlockConfigurationItem {
    Q_GADGET

    Q_PROPERTY(QString type MEMBER type)
    Q_PROPERTY(QVariantMap details MEMBER details)

  public:
    QString type;
    QVariantMap details;

    bool operator==(const BlockConfigurationItem&) const = default;
};

QDBusArgument &operator<<(QDBusArgument& argument, const BlockConfigurationItem& blockConfigurationItem);
const QDBusArgument &operator>>(const QDBusArgument& argument, BlockConfigurationItem& blockConfigurationItem);

class BtrfsSubvolume {
    Q_GADGET

    Q_PROPERTY(quint64 id MEMBER id)
    Q_PROPERTY(quint64 parent_id MEMBER parent_id)
    Q_PROPERTY(QString path MEMBER path)

  public:
    quint64 id;
    quint64 parent_id;
    QString path;
};

QDBusArgument &operator<<(QDBusArgument& argument, const BtrfsSubvolume& btrfsSubvolume);
const QDBusArgument &operator>>(const QDBusArgument& argument, BtrfsSubvolume& btrfsSubvolume);

class SMARTAttributePrettyUnit : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

  public:
    enum Enum : quint8 {
        Unknown,
        Dimensionless,
        Milliseconds,
        Sectors,
        Millikelvin
    };

    Q_ENUM(Enum)

    Q_INVOKABLE static QString toString(Enum pretty_unit);
};

class SMARTAttribute {
    Q_GADGET

    Q_PROPERTY(quint8 id MEMBER id)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(quint16 flags MEMBER flags)
    Q_PROPERTY(qint32 value MEMBER value)
    Q_PROPERTY(qint32 worst MEMBER worst)
    Q_PROPERTY(qint32 threshold MEMBER threshold)
    Q_PROPERTY(qint64 pretty MEMBER pretty)
    Q_PROPERTY(SMARTAttributePrettyUnit::Enum pretty_unit MEMBER pretty_unit)
    Q_PROPERTY(QVariantMap expansion MEMBER expansion)

  public:
    quint8 id;
    QString name;
    quint16 flags;
    qint32 value = -1;
    qint32 worst = -1;
    qint32 threshold = -1;
    qint64 pretty;
    SMARTAttributePrettyUnit::Enum pretty_unit;
    QVariantMap expansion;
};

QDBusArgument &operator<<(QDBusArgument& argument, const SMARTAttribute& smartAttribute);
const QDBusArgument &operator>>(const QDBusArgument& argument, SMARTAttribute& smartAttribute);

using BlockConfiguration = QList<BlockConfigurationItem>;
using BtrfsSubvolumes = QList<BtrfsSubvolume>;
using SMARTAttributes = QList<SMARTAttribute>;

QDebug operator<<(QDebug debug, const BlockConfigurationItem& blockConfigurationItem);

} // namespace extrashell::udisks2

Q_DECLARE_METATYPE(extrashell::udisks2::ActionAvailability)
Q_DECLARE_METATYPE(extrashell::udisks2::ResizeAvailability)
Q_DECLARE_METATYPE(extrashell::udisks2::BlockConfigurationItem)
Q_DECLARE_METATYPE(extrashell::udisks2::BlockConfiguration)
Q_DECLARE_METATYPE(extrashell::udisks2::BtrfsSubvolume)
Q_DECLARE_METATYPE(extrashell::udisks2::BtrfsSubvolumes)
Q_DECLARE_METATYPE(extrashell::udisks2::SMARTAttribute)
Q_DECLARE_METATYPE(extrashell::udisks2::SMARTAttributes)

Q_DECLARE_OPERATORS_FOR_FLAGS(extrashell::udisks2::ResizeFlags::Flags)