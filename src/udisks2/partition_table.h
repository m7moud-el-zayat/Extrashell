#pragma once

#include <qcontainerfwd.h>
#include <qdbusextratypes.h>
#include <qlist.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qtmetamacros.h>

#include "../dbus/properties.h"

Q_DECLARE_LOGGING_CATEGORY(logUDisks2PartitionTable)

class UDisks2PartitionTableInterface;

namespace extrashell::udisks2 {

class PartitionTable : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2PartitionTable)
    QML_UNCREATABLE("ParitionTable must be acquired from the UDisks2Partition.")

    Q_PROPERTY(QString type READ default NOTIFY typeChanged BINDABLE bindableType)

  public:
    explicit PartitionTable(const QString& path, const QVariantHash& initials, QObject* parent = nullptr);

    [[nodiscard]] QBindable<QString> bindableType() const { return &b_type; }

    Q_INVOKABLE void CreatePartition(
        qulonglong offset, qulonglong size, const QString& type,
        const QString& name, const QVariantMap& options = {}
    );

    Q_INVOKABLE void CreatePartitionAndFormat(
        qulonglong offset, qulonglong size, const QString& type,
        const QString& name, const QVariantMap& options,
        const QString& format_type, const QVariantMap& format_options = {}
    );

  signals:
    void typeChanged();

  private:
    UDisks2PartitionTableInterface* m_iface = nullptr;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    ES_DBUS_BINDABLE_PROPERTY(PartitionTable, QString, b_type, typeChanged, "Type", propertiesGroup)
};

} // namespace extrashell::udisks2