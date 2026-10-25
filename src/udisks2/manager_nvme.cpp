#include <qdbusconnection.h>

#include "dbus_interfaces.h"
#include "manager_nvme.h"

#include "../dbus/utils.h"

#include "udisks2_manager_nvme_interface.h"

Q_LOGGING_CATEGORY(logUDisks2ManagerNVMe, "extrashell.udisks2.manager.nvme", QtWarningMsg)

namespace extrashell::udisks2 {

ManagerNVMe::ManagerNVMe(const QVariantHash& initials, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2ManagerNVMe).noquote()
            << "Failed to initialize ManagerNVMe for" << (UDISKS2_MANAGER_OBJECT + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new UDisks2ManagerNVMeInterface(UDISKS2_SERVICE, UDISKS2_MANAGER_OBJECT, systemBus, this);
    
    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllFromObjectManager(initials);
}

void ManagerNVMe::setHostID(const QString& hostid, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2ManagerNVMe,
        m_iface,
        QStringLiteral("setHostID"),
        &UDisks2ManagerNVMeInterface::SetHostID,
        hostid.toUtf8(), options
    );
}

void ManagerNVMe::setHostNQN(const QString& hostnqn, const QVariantMap& options) {
    dbus::callAsync(
        logUDisks2ManagerNVMe,
        m_iface,
        QStringLiteral("setHostNQN"),
        &UDisks2ManagerNVMeInterface::SetHostNQN,
        hostnqn.toUtf8(), options
    );
}

} // namespace extrashell::udisks2

#include "moc_manager_nvme.cpp"