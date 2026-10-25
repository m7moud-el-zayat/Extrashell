#pragma once

#include <qcontainerfwd.h>
#include <qloggingcategory.h>
#include <qmap.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <qvariant.h>

#include "../dbus/properties.h"

Q_DECLARE_LOGGING_CATEGORY(logUDisks2FilesystemBtrfs)

class UDisks2FilesystemBTRFSInterface;

namespace extrashell::dbus {

class AsyncCall;

} // namespace extrashell::dbus

namespace extrashell::udisks2 {

class FilesystemBtrfs : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2FilesystemBtrfs)
    QML_UNCREATABLE("FilesystemBtrfs must be acquired from the UDisks2Filesystem.")

    Q_PROPERTY(QString label READ default NOTIFY labelChanged BINDABLE bindableLabel)
    Q_PROPERTY(QString uuid READ default NOTIFY uuidChanged BINDABLE bindableUuid)
    Q_PROPERTY(qulonglong num_devices READ default NOTIFY num_devicesChanged BINDABLE bindableNum_devices)
    Q_PROPERTY(qulonglong used READ default NOTIFY usedChanged BINDABLE bindableUsed)

  public:
    explicit FilesystemBtrfs(const QString& path, const QVariantHash& initials, QObject* parent = nullptr);

    [[nodiscard]] QBindable<QString> bindableLabel() const { return &b_label; }
    [[nodiscard]] QBindable<QString> bindableUuid() const { return &b_uuid; }
    [[nodiscard]] QBindable<qulonglong> bindableNum_devices() const { return &b_num_devices; }
    [[nodiscard]] QBindable<qulonglong> bindableUsed() const { return &b_used; }

    Q_INVOKABLE dbus::AsyncCall* getDefaultSubvolumeID(const QVariantMap& options = {});
    Q_INVOKABLE dbus::AsyncCall* getSubvolumes(bool snapshots_only, const QVariantMap& options = {});
    Q_INVOKABLE void createSnapshot(const QString& source, const QString& dest, bool ro, const QVariantMap& options = {});
    Q_INVOKABLE void createSubvolume(const QString& name, const QVariantMap& options = {});
    Q_INVOKABLE void removeSubvolume(const QString& name, const QVariantMap& options = {});
    Q_INVOKABLE void addDevice(const QString& device, const QVariantMap& options = {});
    Q_INVOKABLE void removeDevice(const QString& device, const QVariantMap& options = {});
    Q_INVOKABLE void setLabel(const QString& label, const QVariantMap& options = {});
    Q_INVOKABLE void resize(qulonglong size, const QVariantMap& options = {});
    Q_INVOKABLE void setDefaultSubvolumeID(uint id, const QVariantMap& options = {});
    Q_INVOKABLE void repair(const QVariantMap& options = {});

  signals:
    void labelChanged();
    void uuidChanged();
    void num_devicesChanged();
    void usedChanged();

  private:
    UDisks2FilesystemBTRFSInterface* m_iface = nullptr;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    ES_DBUS_BINDABLE_PROPERTY(FilesystemBtrfs, QString, b_label, labelChanged, "label", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(FilesystemBtrfs, QString, b_uuid, uuidChanged, "uuid", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(FilesystemBtrfs, qulonglong, b_num_devices, num_devicesChanged, "num_devices", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(FilesystemBtrfs, qulonglong, b_used, usedChanged, "used", propertiesGroup)
};

} // namespace extrashell::udisks2