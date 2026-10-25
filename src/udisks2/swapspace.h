#pragma once

#include <qcontainerfwd.h>
#include <qloggingcategory.h>
#include <qmap.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qvariant.h>

#include "../dbus/properties.h"

Q_DECLARE_LOGGING_CATEGORY(logUDisks2Swapspace)

class UDisks2SwapspaceInterface;

namespace extrashell::udisks2 {

class Swapspace : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2Swapspace)
    QML_UNCREATABLE("Swapspace must be acquired from the UDisks2Block.")

    Q_PROPERTY(bool active READ default NOTIFY activeChanged BINDABLE bindableActive)

  public:
    explicit Swapspace(const QString& path, const QVariantHash& initials, QObject* parent = nullptr);

    [[nodiscard]] QBindable<bool> bindableActive() const { return &b_active; }

    Q_INVOKABLE void start(const QVariantMap& options = {});
    Q_INVOKABLE void stop(const QVariantMap& options = {});
    Q_INVOKABLE void setLabel(const QString& label, const QVariantMap& options = {});
    Q_INVOKABLE void setUUID(const QString& uuid, const QVariantMap& options = {});

  signals:
    void activeChanged();

  private:
    UDisks2SwapspaceInterface* m_iface = nullptr;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    ES_DBUS_BINDABLE_PROPERTY(Swapspace, bool, b_active, activeChanged, "Active", propertiesGroup)
};

} // namespace extrashell::udisks2