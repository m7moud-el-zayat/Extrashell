#include <qdbusconnection.h>
#include <qlogging.h>

#include "dbus_interfaces.h"
#include "filesystem.h"

#include "../dbus/utils.h"

#include "udisks2_filesystem_interface.h"

Q_LOGGING_CATEGORY(logUDisks2Filesystem, "extrashell.udisks2.filesystem", QtWarningMsg)

namespace extrashell::udisks2 {

Filesystem::Filesystem(const QString& path, const QVariantHash& initials, QObject* parent) : QObject(parent)  {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2Filesystem).noquote()
            << "Failed to initialize Filesystem for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2FilesystemInterface(UDISKS2_SERVICE, path, systemBus, this);
    
    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

void Filesystem::updateSize() { propertiesGroup.getPropertyViaGet(QStringLiteral("Size")); }

dbus::AsyncCall* Filesystem::mount(const QVariantMap& options) {
    return new dbus::AsyncCall(
        logUDisks2Filesystem,
        m_iface,
        QStringLiteral("mount"),
        &UDisks2FilesystemInterface::Mount,
        options
    );
}

dbus::AsyncCall* Filesystem::check(const QVariantMap& options) {
    return new dbus::AsyncCall(
        logUDisks2Filesystem,
        m_iface,
        QStringLiteral("check"),
        &UDisks2FilesystemInterface::Check,
        options
    );
}

dbus::AsyncCall* Filesystem::repair(const QVariantMap& options) {
    return new dbus::AsyncCall(
        logUDisks2Filesystem,
        m_iface,
        QStringLiteral("repair"),
        &UDisks2FilesystemInterface::Repair,
        options
    );
}

void Filesystem::unmount(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Filesystem,
        m_iface,
        QStringLiteral("unmount"),
        &UDisks2FilesystemInterface::Unmount,
        options
    );
}

void Filesystem::takeOwnership(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Filesystem,
        m_iface,
        QStringLiteral("takeOwnership"),
        &UDisks2FilesystemInterface::TakeOwnership,
        options
    );
}

void Filesystem::setLabel(const QString& label, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Filesystem,
        m_iface,
        QStringLiteral("setLabel"),
        &UDisks2FilesystemInterface::SetLabel,
        label, options
    );
}

void Filesystem::setUUID(const QString& uuid, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Filesystem,
        m_iface,
        QStringLiteral("setUUID"),
        &UDisks2FilesystemInterface::SetUUID,
        uuid, options
    );
}

void Filesystem::resize(qulonglong size, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Filesystem,
        m_iface,
        QStringLiteral("resize"),
        &UDisks2FilesystemInterface::Resize,
        size, options
    );
}

} // namespace extrashell::udisks2

#include "moc_filesystem.cpp"