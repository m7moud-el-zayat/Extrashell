#include <qdbusconnection.h>

#include "block.h"
#include "dbus_interfaces.h"
#include "partition.h"
#include "partition_table.h"
#include "qml.h"

// IWYU pragma: begin_keep
#include "filesystem.h"
#include "filesystem_btrfs.h"
#include "swapspace.h"
#include "drive.h"
#include "drive_ata.h"
// IWYU pragma: end_keep

#include "../dbus/utils.h"

#include "udisks2_partition_interface.h"

Q_LOGGING_CATEGORY(logUDisks2Partition, "extrashell.udisks2.partition", QtWarningMsg)

namespace extrashell::udisks2 {

Partition::Partition(const QString& path, const QVariantHash& initials, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2Partition).noquote()
            << "Failed to initialize Partition for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2PartitionInterface(UDISKS2_SERVICE, path, systemBus, this);

    b_table.setBinding([this]() -> PartitionTable* {
        const auto& blocks = UDisks2::instance()->bindableBlocksMap().value();

        if (auto it = blocks.constFind(b_partitionTable.value().path()); it != blocks.cend())
            return (*it)->bindablePartitionTable().value();

        return nullptr;
    });
    
    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

void Partition::deletePartition(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Partition,
        m_iface,
        QStringLiteral("deletePartition"),
        &UDisks2PartitionInterface::Delete,
        options
    );
}

void Partition::setName(const QString& name, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Partition,
        m_iface,
        QStringLiteral("setName"),
        &UDisks2PartitionInterface::SetName,
        name, options
    );
}

void Partition::setType(const QString& type, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Partition,
        m_iface,
        QStringLiteral("setType"),
        &UDisks2PartitionInterface::SetType,
        type, options
    );
}

void Partition::setUUID(const QString& uuid, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Partition,
        m_iface,
        QStringLiteral("setUUID"),
        &UDisks2PartitionInterface::SetUUID,
        uuid, options
    );
}

void Partition::setFlags(qulonglong flags, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Partition,
        m_iface,
        QStringLiteral("setFlags"),
        &UDisks2PartitionInterface::SetFlags,
        flags, options
    );
}

void Partition::resize(qulonglong size, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Partition,
        m_iface,
        QStringLiteral("resize"),
        &UDisks2PartitionInterface::Resize,
        size, options
    );
}

} // namespace extrashell::udisks2

#include "moc_partition.cpp"