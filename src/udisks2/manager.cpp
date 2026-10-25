#include <qdbusconnection.h>
#include <qdbuspendingcall.h>
#include <qdbuspendingreply.h>
#include <qjsengine.h>
#include <qlogging.h>

#include "dbus_interfaces.h"
#include "manager.h"

#include "../dbus/utils.h"

#include "udisks2_manager_interface.h"

Q_LOGGING_CATEGORY(logUDisks2Manager, "extrashell.udisks2.manager", QtWarningMsg)

namespace extrashell::udisks2 {

Manager::Manager(const QVariantHash& initials, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2Manager).noquote()
            << "Failed to initialize Manager for" << (UDISKS2_MANAGER_OBJECT + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2ManagerInterface(UDISKS2_SERVICE, UDISKS2_MANAGER_OBJECT, systemBus, this);
    
    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

dbus::AsyncCall* Manager::canCheck(const QString& type) {
    return new dbus::AsyncCall(
        logUDisks2Manager,
        m_iface,
        QStringLiteral("canCheck"),
        &UDisks2ManagerInterface::CanCheck,
        type
    );
}

dbus::AsyncCall* Manager::canFormat(const QString& type) {
    return new dbus::AsyncCall(
        logUDisks2Manager,
        m_iface,
        QStringLiteral("canFormat"),
        &UDisks2ManagerInterface::CanFormat,
        type
    );
}

dbus::AsyncCall* Manager::canRepair(const QString& type) {
    return new dbus::AsyncCall(
        logUDisks2Manager,
        m_iface,
        QStringLiteral("canRepair"),
        &UDisks2ManagerInterface::CanRepair,
        type
    );
}

dbus::AsyncCall* Manager::canResize(const QString& type) {
    return new dbus::AsyncCall(
        logUDisks2Manager,
        m_iface,
        QStringLiteral("canResize"),
        &UDisks2ManagerInterface::CanResize,
        type
    );
}

void Manager::enableModule(const QString& name, bool enable) {
    dbus::callAsync(
        logUDisks2Manager,
        m_iface,
        QStringLiteral("enableModule"),
        &UDisks2ManagerInterface::EnableModule,
        name, enable
    );
}

} // namespace extrashell::udisks2

#include "moc_manager.cpp"