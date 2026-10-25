#include <qdbusconnection.h>
#include <qlogging.h>

#include "dbus_interfaces.h"
#include "qml.h"
#include "seat.h" // IWYU pragma: keep
#include "session.h"
#include "user.h"

#include "../core/utils.h"
#include "../dbus/utils.h"

#include "login1_user_interface.h"

Q_LOGGING_CATEGORY(logLogin1User, "extrashell.login1.user", QtWarningMsg)

namespace extrashell::login1 {

User::User(const QString& path, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logLogin1User).noquote()
            << "Failed to initialize Session for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new Login1UserInterface(LOGIN1_SERVICE, path, systemBus, this);

    b_display.setBinding([this]() {
        const auto& sessions = Login1::instance()->bindableSessionsMap().value();

        return sessions.value(b_userDisplay.value().path.path());
    });

    b_sessions.setBinding([this]() {
        const auto& sessions = Login1::instance()->bindableSessionsMap().value();
        QList<Session*> sessionsList;

        for (const auto& session : b_userSessions.value())
            if (auto it = sessions.constFind(session.path.path()); it != sessions.cend())
                sessionsList.push_back(*it);

        return sessionsList;
    });

    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllViaGetAll();
}

QQmlListProperty<Session> User::sessions() {
    return {
        this, nullptr,
        &User::sessionsCount,
        &User::session,
    };
}

void User::kill(int signal_number) {
    dbus::callAsync(
        logLogin1User,
        m_iface,
        QStringLiteral("kill"),
        &Login1UserInterface::Kill,
        signal_number
    );
}

void User::terminate() {
    dbus::callAsync(
        logLogin1User,
        m_iface,
        QStringLiteral("terminate"),
        &Login1UserInterface::Terminate
    );
}

qsizetype User::sessionsCount(QQmlListProperty<Session>* list) { return utils::objectCount(list, &User::b_sessions); }
Session* User::session(QQmlListProperty<Session>* list, qsizetype index) { return utils::objectAt(list, index, &User::b_sessions); }

} // namespace extrashell::login1

#include "moc_user.cpp"