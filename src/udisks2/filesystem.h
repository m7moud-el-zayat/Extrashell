#pragma once

#include <qbytearray.h>
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

Q_MOC_INCLUDE("filesystem_btrfs.h")

Q_DECLARE_LOGGING_CATEGORY(logUDisks2Filesystem)

class UDisks2FilesystemInterface;

namespace extrashell::dbus {

class AsyncCall;

} // namespace extrashell::dbus

namespace extrashell::udisks2 {

class FilesystemBtrfs;

class Filesystem : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2Filesystem)
    QML_UNCREATABLE("Filesystem must be acquired from the UDisks2Block.")

    Q_PROPERTY(extrashell::udisks2::FilesystemBtrfs* btrfs READ default NOTIFY btrfsChanged BINDABLE bindableBtrfs)
    Q_PROPERTY(QByteArrayList mountPoints READ default NOTIFY mountPointsChanged BINDABLE bindableMountPoints)
    Q_PROPERTY(qulonglong size READ default NOTIFY sizeChanged BINDABLE bindableSize)

  public:
    explicit Filesystem(const QString& path, const QVariantHash& initials, QObject* parent = nullptr);

    [[nodiscard]] QBindable<FilesystemBtrfs*> bindableBtrfs() const { return &b_btrfs; }
    [[nodiscard]] QBindable<QByteArrayList> bindableMountPoints() const { return &b_mountPoints; }
    [[nodiscard]] QBindable<qulonglong> bindableSize() const { return &b_size; }

    Q_INVOKABLE void updateSize();
    Q_INVOKABLE dbus::AsyncCall* mount(const QVariantMap& options = {});
    Q_INVOKABLE dbus::AsyncCall* check(const QVariantMap& options = {});
    Q_INVOKABLE dbus::AsyncCall* repair(const QVariantMap& options = {});
    Q_INVOKABLE void unmount(const QVariantMap& options = {});
    Q_INVOKABLE void takeOwnership(const QVariantMap& options = {});
    Q_INVOKABLE void setLabel(const QString& label, const QVariantMap& options = {});
    Q_INVOKABLE void setUUID(const QString& uuid, const QVariantMap& options = {});
    Q_INVOKABLE void resize(qulonglong size, const QVariantMap& options = {});

    void setBtrfs(FilesystemBtrfs* btrfs) { b_btrfs = btrfs; }

  signals:
    void btrfsChanged();
    void mountPointsChanged();
    void sizeChanged();

  private:
    UDisks2FilesystemInterface* m_iface = nullptr;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    Q_OBJECT_BINDABLE_PROPERTY(Filesystem, FilesystemBtrfs*, b_btrfs, &Filesystem::btrfsChanged)

    ES_DBUS_BINDABLE_PROPERTY(Filesystem, QByteArrayList, b_mountPoints, mountPointsChanged, "MountPoints", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Filesystem, qulonglong, b_size, sizeChanged, "Size", propertiesGroup)
};

} // namespace extrashell::udisks