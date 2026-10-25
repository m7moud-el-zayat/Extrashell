#include <qbytearray.h>
#include <qdbusconnection.h>
#include <qdbusextratypes.h>
#include <qlist.h>

#include "qml.h"
#include "block.h"
#include "dbus_interfaces.h"

// IWYU pragma: begin_keep
#include "filesystem_btrfs.h"
#include "drive_ata.h"
// IWYU pragma: end_keep

#include "../dbus/utils.h"

#include "udisks2_block_interface.h"

Q_LOGGING_CATEGORY(logUDisks2Block, "extrashell.udisks2.block", QtWarningMsg)

namespace extrashell::udisks2 {

Block::Block(const QString& path, const QVariantHash& initials, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2Block).noquote()
            << "Failed to initialize Block for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2BlockInterface(UDISKS2_SERVICE, path, systemBus, this);

    b_drive.setBinding([this]() {
        const auto& drives = UDisks2::instance()->bindableDrivesMap().value();

        return drives.value(b_blockDrive.value().path());
    });
    
    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

void Block::updateConfigurationItem(
    const BlockConfigurationItem& old_item,
    const BlockConfigurationItem& new_item,
    const QVariantMap& options
) {
    dbus::callAsync(
        logUDisks2Block,
        m_iface,
        QStringLiteral("updateConfigurationItem"),
        &UDisks2BlockInterface::UpdateConfigurationItem,
        old_item, new_item, options
    );
}

void Block::addConfigurationItem(const BlockConfigurationItem& item, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Block,
        m_iface,
        QStringLiteral("addConfigurationItem"),
        &UDisks2BlockInterface::AddConfigurationItem,
        item, options
    );
}

void Block::removeConfigurationItem(const BlockConfigurationItem& item, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Block,
        m_iface,
        QStringLiteral("removeConfigurationItem"),
        &UDisks2BlockInterface::RemoveConfigurationItem,
        item, options
    );
}

void Block::format(const QString& type, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Block,
        m_iface,
        QStringLiteral("format"),
        &UDisks2BlockInterface::Format,
        type, options
    );
}

void Block::restoreEncryptedHeader(const QString& backup_file, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Block,
        m_iface,
        QStringLiteral("restoreEncryptedHeader"),
        &UDisks2BlockInterface::RestoreEncryptedHeader,
        backup_file, options
    );
}

dbus::AsyncCall* Block::getSecretConfiguration(const QVariantMap& options) {
    return new dbus::AsyncCall(
        logUDisks2Block,
        m_iface,
        QStringLiteral("getSecretConfiguration"),
        &UDisks2BlockInterface::GetSecretConfiguration,
        options
    );
}

void Block::rescan(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Block,
        m_iface,
        QStringLiteral("rescan"),
        &UDisks2BlockInterface::Rescan,
        options
    );
}

} // namespace extrashell::udisks2

#include "moc_block.cpp"