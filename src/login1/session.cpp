#include <qdbusconnection.h>
#include <qdbusunixfiledescriptor.h>
#include <qlogging.h>
#include <qobjectdefs.h>

#include "dbus_interfaces.h"
#include "qml.h"
#include "session.h"

#include "login1_session_interface.h"

#include "../dbus/utils.h"

Q_LOGGING_CATEGORY(logLogin1Session, "extrashell.login1.session", QtWarningMsg)

namespace extrashell::login1 {

Session::Session(const QString& path, QObject* parent) : QObject(parent) {
    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logLogin1Session).noquote()
            << "Failed to initialize Session for" << (path + ':')
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new Login1SessionInterface(LOGIN1_SERVICE, path, systemBus, this);

    b_user.setBinding([this]() {
        const auto& users = Login1::instance()->bindableUsersMap().value();

        return users.value(b_sessionUser.value().path.path());
    });

    b_seat.setBinding([this]() {
        const auto& seats = Login1::instance()->bindableSeatsMap().value();

        return seats.value(b_sessionSeat.value().path.path());
    });

    const QString staticInterfaceName = m_iface->staticInterfaceName();

    systemBus.connect(LOGIN1_SERVICE, path, staticInterfaceName, "Lock", this, SIGNAL(lockRequested()));
    systemBus.connect(LOGIN1_SERVICE, path, staticInterfaceName, "Unlock", this, SIGNAL(unlockRequested()));

    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllViaGetAll();
}

void Session::activate() {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("activate"),
        &Login1SessionInterface::Activate
    );
}

void Session::kill(const QString& whom, int signal_number) {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("kill"),
        &Login1SessionInterface::Kill,
        whom, signal_number
    );
}

void Session::lock() {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("lock"),
        &Login1SessionInterface::Lock
    );
}

void Session::releaseControl() {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("releaseControl"),
        &Login1SessionInterface::ReleaseControl
    );
}

void Session::setBrightness(const QString& subsystem, const QString& name, uint brightness) {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("setBrightness"),
        &Login1SessionInterface::SetBrightness,
        subsystem, name, brightness
    );
}

void Session::setDisplay(const QString& display) {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("setDisplay"),
        &Login1SessionInterface::SetDisplay,
        display
    );
}

void Session::setIdleHint(bool idle) {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("setIdleHint"),
        &Login1SessionInterface::SetIdleHint,
        idle
    );
}

void Session::setLockedHint(bool locked) {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("setLockedHint"),
        &Login1SessionInterface::SetLockedHint,
        locked
    );
}

void Session::setClass(const QString& session_class) {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("setClass"),
        &Login1SessionInterface::SetClass,
        session_class
    );
}

void Session::setTTY(int tty_fd) {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("setTTY"),
        &Login1SessionInterface::SetTTY,
        QDBusUnixFileDescriptor(tty_fd)
    );
}

void Session::setType(const QString &type) {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("setType"),
        &Login1SessionInterface::SetType,
        type
    );
}

void Session::takeControl(bool force) {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("takeControl"),
        &Login1SessionInterface::TakeControl,
        force
    );
}

void Session::terminate() {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("terminate"),
        &Login1SessionInterface::Terminate
    );
}

void Session::unlock() {
    dbus::callAsync(
        logLogin1Session,
        m_iface,
        QStringLiteral("unlock"),
        &Login1SessionInterface::Unlock
    );
}

} // namespace extrashell::login1

#include "moc_session.cpp"