#pragma once

#include <qcontainerfwd.h>
#include <qdbusextratypes.h>
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

Q_MOC_INCLUDE("partition_table.h")

Q_DECLARE_LOGGING_CATEGORY(logUDisks2Partition)

class UDisks2PartitionInterface;

namespace extrashell::udisks2 {

class PartitionTable;

class Partition : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2Partition)
    QML_UNCREATABLE("Partition must be acquired from the UDisks2Block.")

    Q_PROPERTY(extrashell::udisks2::PartitionTable* table READ default NOTIFY tableChanged BINDABLE bindableTable)
    Q_PROPERTY(QString name READ default NOTIFY nameChanged BINDABLE bindableName)
    Q_PROPERTY(QString type READ default NOTIFY typeChanged BINDABLE bindableType)
    Q_PROPERTY(QString uuid READ default NOTIFY uuidChanged BINDABLE bindableUUID)
    Q_PROPERTY(qulonglong flags READ default NOTIFY flagsChanged BINDABLE bindableFlags)
    Q_PROPERTY(qulonglong offset READ default NOTIFY offsetChanged BINDABLE bindableOffset)
    Q_PROPERTY(qulonglong size READ default NOTIFY sizeChanged BINDABLE bindableSize)
    Q_PROPERTY(uint number READ default NOTIFY numberChanged BINDABLE bindableNumber)
    Q_PROPERTY(bool isContained READ default NOTIFY isContainedChanged BINDABLE bindableIsContained)
    Q_PROPERTY(bool isContainer READ default NOTIFY isContainerChanged BINDABLE bindableIsContainer)

  public:
    explicit Partition(const QString& path, const QVariantHash& initials, QObject* parent = nullptr);

    [[nodiscard]] QBindable<PartitionTable*> bindableTable() const { return &b_table; }
    [[nodiscard]] QBindable<QDBusObjectPath> bindablePartitionTable() const { return &b_partitionTable; }
    [[nodiscard]] QBindable<QString> bindableName() const { return &b_name; }
    [[nodiscard]] QBindable<QString> bindableType() const { return &b_type; }
    [[nodiscard]] QBindable<QString> bindableUUID() const { return &b_uuid; }
    [[nodiscard]] QBindable<qulonglong> bindableFlags() const { return &b_flags; }
    [[nodiscard]] QBindable<qulonglong> bindableOffset() const { return &b_offset; }
    [[nodiscard]] QBindable<qulonglong> bindableSize() const { return &b_size; }
    [[nodiscard]] QBindable<uint> bindableNumber() const { return &b_number; }
    [[nodiscard]] QBindable<bool> bindableIsContained() const { return &b_isContained; }
    [[nodiscard]] QBindable<bool> bindableIsContainer() const { return &b_isContainer; }

    Q_INVOKABLE void deletePartition(const QVariantMap& options = {});
    Q_INVOKABLE void setName(const QString& name, const QVariantMap& options = {});
    Q_INVOKABLE void setType(const QString& type, const QVariantMap& options = {});
    Q_INVOKABLE void setUUID(const QString& uuid, const QVariantMap& options = {});
    Q_INVOKABLE void setFlags(qulonglong flags, const QVariantMap& options = {});
    Q_INVOKABLE void resize(qulonglong size, const QVariantMap& options = {});

  signals:
    void tableChanged();
    void nameChanged();
    void typeChanged();
    void uuidChanged();
    void flagsChanged();
    void offsetChanged();
    void sizeChanged();
    void numberChanged();
    void isContainedChanged();
    void isContainerChanged();

  private:
    UDisks2PartitionInterface* m_iface = nullptr;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    Q_OBJECT_BINDABLE_PROPERTY(Partition, PartitionTable*, b_table, &Partition::tableChanged)

    ES_DBUS_BINDABLE_PROPERTY(Partition, QDBusObjectPath, b_partitionTable, "Table", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Partition, QString, b_name, nameChanged, "Name", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Partition, QString, b_type, typeChanged, "Type", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Partition, QString, b_uuid, uuidChanged, "UUID", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Partition, qulonglong, b_flags, flagsChanged, "Flags", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Partition, qulonglong, b_offset, offsetChanged, "Offset", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Partition, qulonglong, b_size, sizeChanged, "Size", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Partition, uint, b_number, numberChanged, "Number", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Partition, bool, b_isContained, isContainedChanged, "IsContained", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Partition, bool, b_isContainer, isContainerChanged, "IsContainer", propertiesGroup)
};

} // namespace extrashell::udisks2