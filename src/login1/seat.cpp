#include <qdbusconnection.h>
#include <qlogging.h>

#include "dbus_interfaces.h"
#include "qml.h"
#include "seat.h"
#include "session.h"
#include "user.h" // IWYU pragma: keep

#include "../core/utils.h"
#include "../dbus/utils.h"

#include "login1_seat_interface.h"

Q_LOGGING_CATEGORY(logLogin1Seat, "extrashell.login1.seat", QtWarningMsg)

namespace extrashell::login1 {

Seat::Seat(const QString& path, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logLogin1Seat).noquote()
            << "Failed to initialize Seat for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new Login1SeatInterface(LOGIN1_SERVICE, path, systemBus, this);

    b_activeSession.setBinding([this]() {
        const auto& sessions = Login1::instance()->bindableSessionsMap().value();

        return sessions.value(b_seatActiveSession.value().path.path());
    });

    b_sessions.setBinding([this]() {
        const auto& sessions = Login1::instance()->bindableSessionsMap().value();
        QList<Session*> sessionsList;

        for (const auto& session : b_seatSessions.value())
            if (auto it = sessions.constFind(session.path.path()); it != sessions.cend())
                sessionsList.push_back(*it);

        return sessionsList;
    });

    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllViaGetAll();
}

QQmlListProperty<Session> Seat::sessions() {
    return {
        this, nullptr,
        &Seat::sessionsCount,
        &Seat::session,
    };
}

void Seat::activateSession(const QString& session_id) {
    dbus::callAsync(
        logLogin1Seat,
        m_iface,
        QStringLiteral("activateSession"),
        &Login1SeatInterface::ActivateSession,
        session_id
    );
}

void Seat::switchTo(uint vtnr) {
    dbus::callAsync(
        logLogin1Seat,
        m_iface,
        QStringLiteral("switchTo"),
        &Login1SeatInterface::SwitchTo,
        vtnr
    );
}

void Seat::switchToNext() {
    dbus::callAsync(
        logLogin1Seat,
        m_iface,
        QStringLiteral("switchToNext"),
        &Login1SeatInterface::SwitchToNext
    );
}

void Seat::switchToPrevious() {
    dbus::callAsync(
        logLogin1Seat,
        m_iface,
        QStringLiteral("switchToPrevious"),
        &Login1SeatInterface::SwitchToPrevious
    );
}

void Seat::terminate() {
    dbus::callAsync(
        logLogin1Seat,
        m_iface,
        QStringLiteral("terminate"),
        &Login1SeatInterface::Terminate
    );
}

qsizetype Seat::sessionsCount(QQmlListProperty<Session>* list) { return utils::objectCount(list, &Seat::b_sessions); }
Session* Seat::session(QQmlListProperty<Session>* list, qsizetype index) { return utils::objectAt(list, index, &Seat::b_sessions); }

} // namespace extrashell::login1

#include "moc_seat.cpp"