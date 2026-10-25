#include <qdbusconnection.h>
#include <qdbuserror.h>
#include <qdbusextratypes.h>
#include <qdbusmetatype.h>
#include <qdbuspendingcall.h>
#include <qdbuspendingreply.h>
#include <qlogging.h>

#include "object_manager.h"

#include "object_manager_interface.h"

Q_LOGGING_CATEGORY(logDBusObjectManager, "extrashell.dbus.objectmanager", QtWarningMsg)

namespace extrashell::dbus {

ObjectManager::ObjectManager(
    const QString& service,
    const QString& path,
    const QDBusConnection& connection,
    QObject* parent) : QObject(parent)
{
    qDBusRegisterMetaType<DBusObjectManagerInterfaces>();
    qDBusRegisterMetaType<DBusObjectManagerObjectPaths>();

    if (!connection.isConnected()) {
        qCWarning(logDBusObjectManager).noquote()
            << "Failed to initialize ObjectManager for" << (service + ':')
            << "D-Bus connection" << connection.name() << "is not connected.";

        return;
    }

    m_iface = new DBusObjectManagerInterface(service, path, connection, this);

    if (!isValid()) {
        qCWarning(logDBusObjectManager).noquote()
            << "Failed to connect InterfacesAdded/Removed signals for" << (service + ':')
            << "D-Bus ObjectManager interface is invalid.";

        return;
    }

    connect(m_iface, &DBusObjectManagerInterface::InterfacesAdded, this, &ObjectManager::interfacesAdded);
    connect(m_iface, &DBusObjectManagerInterface::InterfacesAdded, this, &ObjectManager::logInterfacesAdded);
    connect(m_iface, &DBusObjectManagerInterface::InterfacesRemoved, this, &ObjectManager::interfacesRemoved);
    connect(m_iface, &DBusObjectManagerInterface::InterfacesRemoved, this, &ObjectManager::logInterfacesRemoved);
}

void ObjectManager::getManagedObjects() {
    auto watcher = new QDBusPendingCallWatcher(m_iface->GetManagedObjects(), this);

    auto onFinished = [this](QDBusPendingCallWatcher* watcher) {
        QDBusPendingReply<DBusObjectManagerObjectPaths> reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            qCWarning(logDBusObjectManager).noquote()
                << "Failed to get managed objects for" << (m_iface->service() + ':')
                << reply.error().message();
            
            return;
        }

        for (const auto& [object_path, interfaces_and_properties] : reply.value().asKeyValueRange()) {
            emit interfacesAdded(object_path, interfaces_and_properties);

            logInterfacesAdded(object_path, interfaces_and_properties);
        }
    };

    connect(watcher, &QDBusPendingCallWatcher::finished, this, onFinished);
}

bool ObjectManager::isValid() const { return m_iface && m_iface->isValid(); }

void ObjectManager::logInterfacesAdded(
    const QDBusObjectPath& object_path,
    const DBusObjectManagerInterfaces& interfaces_and_properties
) {
    qCDebug(logDBusObjectManager).noquote()
        << "Interfaces" << ('(' + interfaces_and_properties.keys().join(", ") + ')')
        << "were added to" << object_path.path();
}

void ObjectManager::logInterfacesRemoved(const QDBusObjectPath& object_path, const QStringList& interfaces) {
    qCDebug(logDBusObjectManager).noquote()
        << "Interfaces" << ('(' + interfaces.join(", ") + ')')
        << "were removed from" << object_path.path();
}

} // namespace extrashell::dbus

#include "moc_object_manager.cpp"