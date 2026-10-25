#include <qdbusconnection.h>
#include <qlogging.h>

#include "dbus_interfaces.h"
#include "drive.h"

#include "../dbus/utils.h"

#include "udisks2_drive_interface.h"

Q_LOGGING_CATEGORY(logUDisks2Drive, "extrashell.udisks2.drive", QtWarningMsg)

namespace extrashell::udisks2 {

Drive::Drive(const QString & path, const QVariantHash& initials, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2Drive).noquote()
            << "Failed to initialize Drive for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2DriveInterface(UDISKS2_SERVICE, path, systemBus, this);

    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

void Drive::eject(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Drive,
        m_iface,
        QStringLiteral("eject"),
        &UDisks2DriveInterface::Eject,
        options
    );
}

void Drive::powerOff(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Drive,
        m_iface,
        QStringLiteral("powerOff"),
        &UDisks2DriveInterface::PowerOff,
        options
    );
}

void Drive::setConfiguration(const QVariantMap& value, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Drive,
        m_iface,
        QStringLiteral("setConfiguration"),
        &UDisks2DriveInterface::SetConfiguration,
        value, options
    );
}

} // namespace extrashell::udisks2

#include "moc_drive.cpp"