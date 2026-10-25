#pragma once

#include <qcontainerfwd.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qtmetamacros.h>

#include "../dbus/properties.h"

Q_MOC_INCLUDE("manager_nvme.h")

Q_DECLARE_LOGGING_CATEGORY(logUDisks2Manager)

class UDisks2ManagerInterface;

namespace extrashell::dbus {

class AsyncCall;

} // namespace extrashell::dbus

namespace extrashell::udisks2 {

class ManagerNVMe;

class Manager : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2Manager)
    QML_UNCREATABLE("Manager must be acquired from the UDisks2.")

    Q_PROPERTY(extrashell::udisks2::ManagerNVMe* nvme READ default NOTIFY nvmeChanged BINDABLE bindableNVMe)
    Q_PROPERTY(QStringList supportedEncryptionTypes READ default NOTIFY supportedEncryptionTypesChanged BINDABLE bindableSupportedEncryptionTypes)
    Q_PROPERTY(QStringList supportedFilesystems READ default NOTIFY supportedFilesystemsChanged BINDABLE bindableSupportedFilesystems)
    Q_PROPERTY(QString defaultEncryptionType READ default NOTIFY defaultEncryptionTypeChanged BINDABLE bindableDefaultEncryptionType)
    Q_PROPERTY(QString version READ default NOTIFY versionChanged BINDABLE bindableVersion)

  public:
    explicit Manager(const QVariantHash& initials, QObject *parent = nullptr);

    [[nodiscard]] QBindable<ManagerNVMe*> bindableNVMe() const { return &b_nvme; }
    [[nodiscard]] QBindable<QStringList> bindableSupportedEncryptionTypes() const { return &b_supportedEncryptionTypes; }
    [[nodiscard]] QBindable<QStringList> bindableSupportedFilesystems() const { return &b_supportedFilesystems; }
    [[nodiscard]] QBindable<QString> bindableDefaultEncryptionType() const { return &b_defaultEncryptionType; }
    [[nodiscard]] QBindable<QString> bindableVersion() const { return &b_version; }
    
    Q_INVOKABLE dbus::AsyncCall* canCheck(const QString& type);
    Q_INVOKABLE dbus::AsyncCall* canFormat(const QString& type);
    Q_INVOKABLE dbus::AsyncCall* canRepair(const QString& type);
    Q_INVOKABLE dbus::AsyncCall* canResize(const QString& type);
    
    Q_INVOKABLE void enableModule(const QString& name, bool enable = true);

    void setNVMe(ManagerNVMe* nvme) { b_nvme = nvme; };
    
  signals:
    void nvmeChanged();
    void supportedEncryptionTypesChanged();
    void supportedFilesystemsChanged();
    void defaultEncryptionTypeChanged();
    void versionChanged();

  private:
    UDisks2ManagerInterface* m_iface = nullptr;

    Q_OBJECT_BINDABLE_PROPERTY(Manager, ManagerNVMe*, b_nvme, &Manager::nvmeChanged)
    
    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    ES_DBUS_BINDABLE_PROPERTY(Manager, QStringList, b_supportedEncryptionTypes, supportedEncryptionTypesChanged, "SupportedEncryptionTypes", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Manager, QStringList, b_supportedFilesystems, supportedFilesystemsChanged, "SupportedFilesystems", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Manager, QString, b_defaultEncryptionType, defaultEncryptionTypeChanged, "DefaultEncryptionType", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Manager, QString, b_version, versionChanged, "Version", propertiesGroup)
};

} // namespace extrashell::udisks2