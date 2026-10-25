#include <qdbusconnection.h>
#include <qdbusextratypes.h>
#include <qlogging.h>

#include "filesystem_btrfs.h"
#include "dbus_interfaces.h"

#include "../dbus/utils.h"

#include "udisks2_filesystem_btrfs_interface.h"

Q_LOGGING_CATEGORY(logUDisks2FilesystemBtrfs, "extrashell.udisks2.filesystem.btrfs", QtWarningMsg)

namespace extrashell::udisks2 {

FilesystemBtrfs::FilesystemBtrfs(const QString& path, const QVariantHash& initials, QObject* parent) : QObject(parent)  {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2FilesystemBtrfs).noquote()
            << "Failed to initialize FilesystemBtrfs for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2FilesystemBTRFSInterface(UDISKS2_SERVICE, path, systemBus, this);
    
    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

dbus::AsyncCall* FilesystemBtrfs::getDefaultSubvolumeID(const QVariantMap& options) {
    return new dbus::AsyncCall(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("getDefaultSubvolumeID"),
        &UDisks2FilesystemBTRFSInterface::GetDefaultSubvolumeID,
        options
    );
}

dbus::AsyncCall* FilesystemBtrfs::getSubvolumes(bool snapshots_only, const QVariantMap& options) {
    using Interface = UDisks2FilesystemBTRFSInterface;
    using Reply = QDBusPendingReply<BtrfsSubvolumes, int>;

    return new dbus::AsyncCall(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("getSubvolumes"),
        static_cast<Reply (Interface::*)(bool, const QVariantMap&)>(&Interface::GetSubvolumes),
        snapshots_only, options
    );
}

void FilesystemBtrfs::createSnapshot(const QString& source, const QString& dest, bool ro, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("createSnapshot"),
        &UDisks2FilesystemBTRFSInterface::CreateSnapshot,
        source, dest, ro, options
    );
}

void FilesystemBtrfs::createSubvolume(const QString& name, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("createSubvolume"),
        &UDisks2FilesystemBTRFSInterface::CreateSubvolume,
        name, options
    );
}

void FilesystemBtrfs::removeSubvolume(const QString& name, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("removeSubvolume"),
        &UDisks2FilesystemBTRFSInterface::RemoveSubvolume,
        name, options
    );
}

void FilesystemBtrfs::addDevice(const QString& device, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("addDevice"),
        &UDisks2FilesystemBTRFSInterface::AddDevice,
        QDBusObjectPath(UDISKS2_OBJECT + "/block_devices/" + device), options
    );
}

void FilesystemBtrfs::removeDevice(const QString& device, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("removeDevice"),
        &UDisks2FilesystemBTRFSInterface::RemoveDevice,
        QDBusObjectPath(UDISKS2_OBJECT + "/block_devices/" + device), options
    );
}

void FilesystemBtrfs::setLabel(const QString& label, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("setLabel"),
        &UDisks2FilesystemBTRFSInterface::SetLabel,
        label, options
    );
}

void FilesystemBtrfs::resize(qulonglong size, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("resize"),
        &UDisks2FilesystemBTRFSInterface::Resize,
        size, options
    );
}

void FilesystemBtrfs::setDefaultSubvolumeID(uint id, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("setDefaultSubvolumeID"),
        &UDisks2FilesystemBTRFSInterface::SetDefaultSubvolumeID,
        id, options
    );
}

void FilesystemBtrfs::repair(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2FilesystemBtrfs,
        m_iface,
        QStringLiteral("repair"),
        &UDisks2FilesystemBTRFSInterface::Repair,
        options
    );
}

} // namespace extrashell::udisks2

#include "moc_filesystem_btrfs.cpp"