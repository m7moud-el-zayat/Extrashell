#pragma once

#include <qbytearray.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

#include "../dbus/properties.h"

Q_DECLARE_LOGGING_CATEGORY(logUDisks2ManagerNVMe)

class UDisks2ManagerNVMeInterface;

namespace extrashell::udisks2 {

class ManagerNVMe : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2ManagerNVMe)
    QML_UNCREATABLE("ManagerNVMe must be acquired from the UDisks2Manager.")
    
    Q_PROPERTY(QByteArray hostID READ default NOTIFY hostIDChanged BINDABLE bindableHostID)
    Q_PROPERTY(QByteArray hostNQN READ default NOTIFY hostNQNChanged BINDABLE bindableHostNQN)

  public:
    explicit ManagerNVMe(const QVariantHash& initials, QObject* parent = nullptr);

    [[nodiscard]] QBindable<QByteArray> bindableHostID() const { return &b_hostID; }
    [[nodiscard]] QBindable<QByteArray> bindableHostNQN() const { return &b_hostNQN; }

    Q_INVOKABLE void setHostID(const QString& hostid, const QVariantMap& options = {});
    Q_INVOKABLE void setHostNQN(const QString& hostnqn, const QVariantMap& options = {});

  signals:
    void hostIDChanged();
    void hostNQNChanged();
    
  private:
    UDisks2ManagerNVMeInterface* m_iface = nullptr;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    ES_DBUS_BINDABLE_PROPERTY(ManagerNVMe, QByteArray, b_hostID, hostIDChanged, "HostID", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(ManagerNVMe, QByteArray, b_hostNQN, hostNQNChanged, "HostNQN", propertiesGroup)
};

} // namespace extrashell::udisks2