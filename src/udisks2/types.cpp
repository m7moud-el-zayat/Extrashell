#include <qchar.h>

#include "types.h"

namespace extrashell::udisks2 {

QDBusArgument &operator<<(QDBusArgument &argument, const ActionAvailability &actionAvailability) {
    argument.beginStructure();
    argument << actionAvailability.availability << actionAvailability.binaryName;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ActionAvailability &actionAvailability) {
    argument.beginStructure();
    argument >> actionAvailability.availability >> actionAvailability.binaryName;
    argument.endStructure();

    return argument;
}

QString ResizeFlags::toString(Flags flags) {
    QStringList parts;

    if (flags & OfflineShrink) parts << QStringLiteral("OfflineShrink");
    if (flags & OfflineGrow) parts << QStringLiteral("OfflineGrow");
    if (flags & OnlineShrink) parts << QStringLiteral("OnlineShrink");
    if (flags & OnlineGrow) parts << QStringLiteral("OnlineGrow");

    return parts.join(QStringLiteral(" | "));
}

QDBusArgument &operator<<(QDBusArgument &argument, const ResizeAvailability &resizeAvailability) {
    argument.beginStructure();
    argument << resizeAvailability.availability << resizeAvailability.flags << resizeAvailability.binaryName;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ResizeAvailability &resizeAvailability) {
    quint64 flags;
    
    argument.beginStructure();
    argument >> resizeAvailability.availability >> flags >> resizeAvailability.binaryName;
    argument.endStructure();

    resizeAvailability.flags = ResizeFlags::Flags::fromInt(flags);

    return argument;
}

QDBusArgument &operator<<(QDBusArgument& argument, const BlockConfigurationItem& blockConfigurationItem) {
    argument.beginStructure();
    argument << blockConfigurationItem.type << blockConfigurationItem.details;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument& argument, BlockConfigurationItem& blockConfigurationItem) {
    argument.beginStructure();
    argument >> blockConfigurationItem.type >> blockConfigurationItem.details;
    argument.endStructure();

    return argument;
}

QDBusArgument &operator<<(QDBusArgument& argument, const BtrfsSubvolume& btrfsSubvolume) {
    argument.beginStructure();
    argument << btrfsSubvolume.id << btrfsSubvolume.parent_id << btrfsSubvolume.path;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument& argument, BtrfsSubvolume& btrfsSubvolume) {
    argument.beginStructure();
    argument >> btrfsSubvolume.id >> btrfsSubvolume.parent_id >> btrfsSubvolume.path;
    argument.endStructure();

    return argument;
}

QString SMARTAttributePrettyUnit::toString(Enum pretty_unit) {
    switch (pretty_unit) {
    case Unknown: return QStringLiteral("Unknown");
    case Dimensionless: return QStringLiteral("Dimensionless");
    case Milliseconds: return QStringLiteral("Milliseconds");
    case Sectors: return QStringLiteral("Sectors");
    case Millikelvin: return QStringLiteral("Millikelvin");
    default: return QStringLiteral("Unknown");
    }
}

QDBusArgument &operator<<(QDBusArgument& argument, const SMARTAttribute& smartAttribute) {
    argument.beginStructure();
    argument
        << smartAttribute.id << smartAttribute.name << smartAttribute.flags
        << smartAttribute.value << smartAttribute.worst << smartAttribute.threshold
        << smartAttribute.pretty << smartAttribute.pretty_unit
        << smartAttribute.expansion;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument& argument, SMARTAttribute& smartAttribute) {
    qint32 pretty_unit;

    argument.beginStructure();
    argument
        >> smartAttribute.id >> smartAttribute.name >> smartAttribute.flags
        >> smartAttribute.value >> smartAttribute.worst >> smartAttribute.threshold
        >> smartAttribute.pretty >> pretty_unit
        >> smartAttribute.expansion;
    argument.endStructure();

    smartAttribute.pretty_unit = static_cast<SMARTAttributePrettyUnit::Enum>(pretty_unit);
     
    return argument;
}

QDebug operator<<(QDebug debug, const BlockConfigurationItem& blockConfigurationItem) {
    QDebugStateSaver saver(debug);
    debug.nospace()
        << "BlockConfigurationItem(type=" << blockConfigurationItem.type
        << ", details=" << blockConfigurationItem.details << ')';

    return debug;
}

} // namespace extrashell::udisks2

#include "moc_types.cpp"