#include <qdbusconnection.h>
#include <qlogging.h>

#include "dbus_interfaces.h"
#include "swapspace.h"

#include "../dbus/utils.h"

#include "udisks2_swapspace_interface.h"

Q_LOGGING_CATEGORY(logUDisks2Swapspace, "extrashell.udisks2.swapspace", QtWarningMsg)

namespace extrashell::udisks2 {

Swapspace::Swapspace(const QString& path, const QVariantHash& initials, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2Swapspace).noquote()
            << "Failed to initialize Swapspace for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2SwapspaceInterface(UDISKS2_SERVICE, path, systemBus, this);

    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

void Swapspace::start(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Swapspace,
        m_iface,
        QStringLiteral("start"),
        &UDisks2SwapspaceInterface::Start,
        options
    );
}

void Swapspace::stop(const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Swapspace,
        m_iface,
        QStringLiteral("stop"),
        &UDisks2SwapspaceInterface::Stop,
        options
    );
}

void Swapspace::setLabel(const QString& label, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Swapspace,
        m_iface,
        QStringLiteral("setLabel"),
        &UDisks2SwapspaceInterface::SetLabel,
        label, options
    );
}

void Swapspace::setUUID(const QString& uuid, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2Swapspace,
        m_iface,
        QStringLiteral("setUUID"),
        &UDisks2SwapspaceInterface::SetUUID,
        uuid, options
    );
}

} // namespace extrashell::udisks2

#include "moc_swapspace.cpp"