#pragma once

#include <qcontainerfwd.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qtypes.h>

#include <types.h>

#include "../dbus/properties.h"

Q_MOC_INCLUDE("drive.h")
Q_MOC_INCLUDE("filesystem.h")
Q_MOC_INCLUDE("partition.h")
Q_MOC_INCLUDE("partition_table.h")
Q_MOC_INCLUDE("swapspace.h")

Q_DECLARE_LOGGING_CATEGORY(logUDisks2Block)

class QByteArray;
class QDBusObjectPath;
class UDisks2BlockInterface;

namespace extrashell::dbus {

class AsyncCall;

} // namespace extrashell::dbus

namespace extrashell::udisks2 {

class Drive;
class Filesystem;
class Partition;
class PartitionTable;
class Swapspace;

class Block : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2Block)
    QML_UNCREATABLE("Block must be acquired from the UDisks2.")

    Q_PROPERTY(extrashell::udisks2::Drive* drive READ default NOTIFY driveChanged BINDABLE bindableDrive)
    Q_PROPERTY(extrashell::udisks2::Filesystem* filesystem READ default NOTIFY filesystemChanged BINDABLE bindableFilesystem)
    Q_PROPERTY(extrashell::udisks2::Partition* partition READ default NOTIFY partitionChanged BINDABLE bindablePartition)
    Q_PROPERTY(extrashell::udisks2::PartitionTable* partitionTable READ default NOTIFY partitionTableChanged BINDABLE bindablePartitionTable)
    Q_PROPERTY(extrashell::udisks2::Swapspace* swapspace READ default NOTIFY swapspaceChanged BINDABLE bindableSwapspace)
    Q_PROPERTY(BlockConfiguration configuration READ default NOTIFY configurationChanged BINDABLE bindableConfiguration)
    Q_PROPERTY(QByteArrayList symlinks READ default NOTIFY symlinksChanged BINDABLE bindableSymlinks)
    Q_PROPERTY(QByteArray device READ default NOTIFY deviceChanged BINDABLE bindableDevice)
    Q_PROPERTY(QByteArray preferredDevice READ default NOTIFY preferredDeviceChanged BINDABLE bindablePreferredDevice)
    Q_PROPERTY(QStringList userspaceMountOptions READ default NOTIFY userspaceMountOptionsChanged BINDABLE bindableUserspaceMountOptions)
    Q_PROPERTY(QString hintIconName READ default NOTIFY hintIconNameChanged BINDABLE bindableHintIconName)
    Q_PROPERTY(QString hintName READ default NOTIFY hintNameChanged BINDABLE bindableHintName)
    Q_PROPERTY(QString hintSymbolicIconName READ default NOTIFY hintSymbolicIconNameChanged BINDABLE bindableHintSymbolicIconName)
    Q_PROPERTY(QString id READ default NOTIFY idChanged BINDABLE bindableId)
    Q_PROPERTY(QString idLabel READ default NOTIFY idLabelChanged BINDABLE bindableIdLabel)
    Q_PROPERTY(QString idType READ default NOTIFY idTypeChanged BINDABLE bindableIdType)
    Q_PROPERTY(QString idUsage READ default NOTIFY idUsageChanged BINDABLE bindableIdUsage)
    Q_PROPERTY(QString idUUID READ default NOTIFY idUUIDChanged BINDABLE bindableIdUUID)
    Q_PROPERTY(QString idVersion READ default NOTIFY idVersionChanged BINDABLE bindableIdVersion)
    Q_PROPERTY(qulonglong deviceNumber READ default NOTIFY deviceNumberChanged BINDABLE bindableDeviceNumber)
    Q_PROPERTY(qulonglong size READ default NOTIFY sizeChanged BINDABLE bindableSize)
    Q_PROPERTY(bool hintAuto READ default NOTIFY hintAutoChanged BINDABLE bindableHintAuto)
    Q_PROPERTY(bool hintIgnore READ default NOTIFY hintIgnoreChanged BINDABLE bindableHintIgnore)
    Q_PROPERTY(bool hintPartitionable READ default NOTIFY hintPartitionableChanged BINDABLE bindableHintPartitionable)
    Q_PROPERTY(bool hintSystem READ default NOTIFY hintSystemChanged BINDABLE bindableHintSystem)
    Q_PROPERTY(bool readOnly READ default NOTIFY readOnlyChanged BINDABLE bindableReadOnly)

  public:
    explicit Block(const QString& path, const QVariantHash& initials, QObject* parent = nullptr);

    [[nodiscard]] QBindable<Drive*> bindableDrive() const { return &b_drive; }
    [[nodiscard]] QBindable<Filesystem*> bindableFilesystem() const { return &b_filesystem; }
    [[nodiscard]] QBindable<Partition*> bindablePartition() const { return &b_partition; }
    [[nodiscard]] QBindable<PartitionTable*> bindablePartitionTable() const { return &b_partitionTable; }
    [[nodiscard]] QBindable<Swapspace*> bindableSwapspace() const { return &b_swapspace; }
    [[nodiscard]] QBindable<BlockConfiguration> bindableConfiguration() const { return &b_configuration; }
    [[nodiscard]] QBindable<QByteArrayList> bindableSymlinks() const { return &b_symlinks; }
    [[nodiscard]] QBindable<QByteArray> bindableDevice() const { return &b_device; }
    [[nodiscard]] QBindable<QByteArray> bindablePreferredDevice() const { return &b_preferredDevice; }
    [[nodiscard]] QBindable<QDBusObjectPath> bindableBlockDrive() const { return &b_blockDrive; }
    [[nodiscard]] QBindable<QStringList> bindableUserspaceMountOptions() const { return &b_userspaceMountOptions; }
    [[nodiscard]] QBindable<QString> bindableHintIconName() const { return &b_hintIconName; }
    [[nodiscard]] QBindable<QString> bindableHintName() const { return &b_hintName; }
    [[nodiscard]] QBindable<QString> bindableHintSymbolicIconName() const { return &b_hintSymbolicIconName; }
    [[nodiscard]] QBindable<QString> bindableId() const { return &b_id; }
    [[nodiscard]] QBindable<QString> bindableIdLabel() const { return &b_idLabel; }
    [[nodiscard]] QBindable<QString> bindableIdType() const { return &b_idType; }
    [[nodiscard]] QBindable<QString> bindableIdUsage() const { return &b_idUsage; }
    [[nodiscard]] QBindable<QString> bindableIdUUID() const { return &b_idUUID; }
    [[nodiscard]] QBindable<QString> bindableIdVersion() const { return &b_idVersion; }
    [[nodiscard]] QBindable<qulonglong> bindableDeviceNumber() const { return &b_deviceNumber; }
    [[nodiscard]] QBindable<qulonglong> bindableSize() const { return &b_size; }
    [[nodiscard]] QBindable<bool> bindableHintAuto() const { return &b_hintAuto; }
    [[nodiscard]] QBindable<bool> bindableHintIgnore() const { return &b_hintIgnore; }
    [[nodiscard]] QBindable<bool> bindableHintPartitionable() const { return &b_hintPartitionable; }
    [[nodiscard]] QBindable<bool> bindableHintSystem() const { return &b_hintSystem; }
    [[nodiscard]] QBindable<bool> bindableReadOnly() const { return &b_readOnly; }

    Q_INVOKABLE void updateConfigurationItem(const BlockConfigurationItem& old_item, const BlockConfigurationItem& new_item, const QVariantMap& options = {});
    Q_INVOKABLE void addConfigurationItem(const BlockConfigurationItem& item, const QVariantMap& options = {});
    Q_INVOKABLE void removeConfigurationItem(const BlockConfigurationItem& item, const QVariantMap& options = {});
    Q_INVOKABLE void format(const QString& type, const QVariantMap& options = {});
    Q_INVOKABLE void restoreEncryptedHeader(const QString& backup_file, const QVariantMap& options = {});
    Q_INVOKABLE dbus::AsyncCall* getSecretConfiguration(const QVariantMap& options = {});
    Q_INVOKABLE void rescan(const QVariantMap& options = {});

    void setFilesystem(Filesystem* filesystem) { b_filesystem = filesystem; }
    void setPartition(Partition* partition) { b_partition = partition; }
    void setPartitionTable(PartitionTable* partitionTable) { b_partitionTable = partitionTable; }
    void setSwapspace(Swapspace* swapspace) { b_swapspace = swapspace; }

  signals:
    void driveChanged();
    void filesystemChanged();
    void partitionChanged();
    void partitionTableChanged();
    void swapspaceChanged();
    void configurationChanged();
    void symlinksChanged();
    void deviceChanged();
    void preferredDeviceChanged();
    void userspaceMountOptionsChanged();
    void hintIconNameChanged();
    void hintNameChanged();
    void hintSymbolicIconNameChanged();
    void idChanged();
    void idLabelChanged();
    void idTypeChanged();
    void idUsageChanged();
    void idUUIDChanged();
    void idVersionChanged();
    void deviceNumberChanged();
    void sizeChanged();
    void hintAutoChanged();
    void hintIgnoreChanged();
    void hintPartitionableChanged();
    void hintSystemChanged();
    void readOnlyChanged();

  private:
    UDisks2BlockInterface* m_iface = nullptr;
    
    bool isValid() const;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    Q_OBJECT_BINDABLE_PROPERTY(Block, Drive*, b_drive, &Block::driveChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Block, Filesystem*, b_filesystem, &Block::filesystemChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Block, Partition*, b_partition, &Block::partitionChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Block, PartitionTable*, b_partitionTable, &Block::partitionTableChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Block, Swapspace*, b_swapspace, &Block::swapspaceChanged)

    ES_DBUS_BINDABLE_PROPERTY(Block, BlockConfiguration, b_configuration, configurationChanged, "Configuration", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QByteArrayList, b_symlinks, symlinksChanged, "Symlinks", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QByteArray, b_device, deviceChanged, "Device", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QByteArray, b_preferredDevice, preferredDeviceChanged, "PreferredDevice", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QDBusObjectPath, b_blockDrive, "Drive", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QStringList, b_userspaceMountOptions, userspaceMountOptionsChanged, "UserspaceMountOptions", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QString, b_hintIconName, hintIconNameChanged, "HintIconName", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QString, b_hintName, hintNameChanged, "HintName", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QString, b_hintSymbolicIconName, hintSymbolicIconNameChanged, "HintSymbolicIconName", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QString, b_id, idChanged, "Id", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QString, b_idLabel, idLabelChanged, "IdLabel", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QString, b_idType, idTypeChanged, "IdType", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QString, b_idUsage, idUsageChanged, "IdUsage", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QString, b_idUUID, idUUIDChanged, "IdUUID", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, QString, b_idVersion, idVersionChanged, "IdVersion", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, bool, b_hintAuto, hintAutoChanged, "HintAuto", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, bool, b_hintIgnore, hintIgnoreChanged, "HintIgnore", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS(Block, bool, b_hintPartitionable, true, hintPartitionableChanged, "HintPartitionable", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, bool, b_hintSystem, hintSystemChanged, "HintSystem", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, bool, b_readOnly, readOnlyChanged, "ReadOnly", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, qulonglong, b_deviceNumber, deviceNumberChanged, "DeviceNumber", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Block, qulonglong, b_size, sizeChanged, "Size", propertiesGroup)
};

} // namespace extrashell::udisks2