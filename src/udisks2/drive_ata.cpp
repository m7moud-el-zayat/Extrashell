#include <qdbusconnection.h>
#include <qdbuspendingcall.h>
#include <qdbuspendingreply.h>
#include <qlogging.h>

#include "dbus_interfaces.h"
#include "drive_ata.h"

#include "../dbus/utils.h"

#include "udisks2_drive_ata_interface.h"

Q_LOGGING_CATEGORY(logUDisks2DriveAta, "extrashell.udisks2.drive.ata", QtWarningMsg)

namespace extrashell::udisks2 {

DriveAta::DriveAta(const QString& path, const QVariantHash& initials, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2DriveAta).noquote()
            << "Failed to initialize DriveAta for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2DriveAtaInterface(UDISKS2_SERVICE, path, systemBus, this);
    
    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

dbus::AsyncCall* DriveAta::pmGetState(const QVariantMap& options) {
    return new dbus::AsyncCall(
        logUDisks2DriveAta,
        m_iface,
        QStringLiteral("getSecretConfiguration"),
        &UDisks2DriveAtaInterface::PmGetState,
        options
    );
}

dbus::AsyncCall* DriveAta::smartGetAttributes(const QVariantMap& options) {
    return new dbus::AsyncCall(
        logUDisks2DriveAta,
        m_iface,
        QStringLiteral("getSecretConfiguration"),
        &UDisks2DriveAtaInterface::SmartGetAttributes,
        options
    );
}

void DriveAta::pmStandby(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2DriveAta,
        m_iface,
        QStringLiteral("pmStandby"),
        &UDisks2DriveAtaInterface::PmStandby,
        options
    );
}

void DriveAta::pmWakeup(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2DriveAta,
        m_iface,
        QStringLiteral("pmWakeup"),
        &UDisks2DriveAtaInterface::PmWakeup,
        options
    );
}

void DriveAta::securityEraseUnit(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2DriveAta,
        m_iface,
        QStringLiteral("securityEraseUnit"),
        &UDisks2DriveAtaInterface::SecurityEraseUnit,
        options
    );
}

void DriveAta::smartSelftestAbort(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2DriveAta,
        m_iface,
        QStringLiteral("smartSelftestAbort"),
        &UDisks2DriveAtaInterface::SmartSelftestAbort,
        options
    );
}

void DriveAta::smartUpdate(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2DriveAta,
        m_iface,
        QStringLiteral("smartUpdate"),
        &UDisks2DriveAtaInterface::SmartUpdate,
        options
    );
}

void DriveAta::smartSelftestStart(const QString& type, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2DriveAta,
        m_iface,
        QStringLiteral("smartSelftestStart"),
        &UDisks2DriveAtaInterface::SmartSelftestStart,
        type, options
    );
}

void DriveAta::smartSetEnabled(bool value, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2DriveAta,
        m_iface,
        QStringLiteral("smartSetEnabled"),
        &UDisks2DriveAtaInterface::SmartSetEnabled,
        value, options
    );
}

} // namespace extrashell::udisks2

#include "moc_drive_ata.cpp"