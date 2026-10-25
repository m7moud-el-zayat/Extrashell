#pragma once

#include <qcontainerfwd.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>

#include "types.h"

Q_DECLARE_LOGGING_CATEGORY(logDBusObjectManager)

class QDBusConnection;
class QDBusObjectPath;
class DBusObjectManagerInterface;

namespace extrashell::dbus {

class ObjectManager : public QObject {
    Q_OBJECT

  public:
    explicit ObjectManager(
        const QString& service,
        const QString& path,
        const QDBusConnection& connection,
        QObject* parent = nullptr
    );

    void getManagedObjects();
    bool isValid() const;

  signals:
    void interfacesAdded(const QDBusObjectPath& object_path, const DBusObjectManagerInterfaces& interfaces_and_properties);
    void interfacesRemoved(const QDBusObjectPath& object_path, const QStringList& interfaces);

  private slots:
    void logInterfacesAdded(const QDBusObjectPath& object_path, const DBusObjectManagerInterfaces& interfaces_and_properties);
    void logInterfacesRemoved(const QDBusObjectPath& object_path, const QStringList& interfaces);

  private:
    DBusObjectManagerInterface* m_iface = nullptr;
};

} // namespace extrashell::dbus