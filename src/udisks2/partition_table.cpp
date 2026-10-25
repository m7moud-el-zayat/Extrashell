#include <qdbusconnection.h>

#include "dbus_interfaces.h"
#include "partition.h"
#include "partition_table.h"

#include "../dbus/utils.h"

#include "udisks2_partition_table_interface.h"

Q_LOGGING_CATEGORY(logUDisks2PartitionTable, "extrashell.udisks2.partitiontable", QtWarningMsg)

namespace extrashell::udisks2 {

PartitionTable::PartitionTable(const QString& path, const QVariantHash& initials, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2PartitionTable).noquote()
            << "Failed to initialize PartitionTable for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2PartitionTableInterface(UDISKS2_SERVICE, path, systemBus, this);
    
    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

void PartitionTable::CreatePartition(
    qulonglong offset, qulonglong size, const QString& type,
    const QString& name, const QVariantMap& options
) {
    dbus::callAsync(
        logUDisks2PartitionTable,
        m_iface,
        QStringLiteral("createPartition"),
        &UDisks2PartitionTableInterface::CreatePartition,
        offset, size, type, name, options
    );
}

void PartitionTable::CreatePartitionAndFormat(
    qulonglong offset, qulonglong size, const QString& type,
    const QString& name, const QVariantMap& options,
    const QString& format_type, const QVariantMap& format_options
) {
    dbus::callAsync(
        logUDisks2PartitionTable,
        m_iface,
        QStringLiteral("createPartition"),
        &UDisks2PartitionTableInterface::CreatePartitionAndFormat,
        offset, size, type, name, options, format_type, format_options
    );
}

} // namespace extrashell::udisks2

#include "moc_partition_table.cpp"