#include <qdbusabstractinterface.h>

#include "utils.h"

namespace extrashell::dbus {

bool isInterfaceValid(
    const QLoggingCategory& loggingCategory,
    QDBusAbstractInterface* iface,
    const QString& name
) {
    if (!iface) {
        const QString message = QStringLiteral("D-Bus interface is null.");

        qCWarning(loggingCategory).noquote()
            << "Failed to call function" << (name + ':')
            << message;

        return false;
    }

    if (!iface->isValid()) {
        const QString message = iface->lastError().message();

        qCWarning(loggingCategory).noquote()
            << "Failed to call function" << name << "for" << iface->path() << "in" << (iface->interface() + ':')
            << message;

        return false;
    }

    return true;
}

} // namespace extrashell::dbus

#include "moc_utils.cpp"