#include <qcoreapplication.h>
#include <qdbusconnection.h>
#include <qdbusextratypes.h>
#include <qdbusmetatype.h>
#include <qdbuspendingcall.h>
#include <qdbuspendingreply.h>
#include <qjsengine.h>
#include <qlogging.h>

#include "dbus_interfaces.h"
#include "qml.h"
#include "seat.h"
#include "session.h"
#include "user.h"

#include "../core/utils.h"
#include "../dbus/utils.h"

#include "login1_manager_interface.h"

#include <utility>

Q_LOGGING_CATEGORY(logLogin1, "extrashell.login1", QtWarningMsg)

namespace extrashell::login1 {

namespace {

template <auto Call, auto Signal>
void initializeObjects(Login1ManagerInterface* iface, const QString& name) {
    auto result = (iface->*Call)();
    auto* watcher = new QDBusPendingCallWatcher(result, static_cast<Login1*>(iface->parent()));

    using ReturnType = decltype(result);

    auto onFinished = [=](QDBusPendingCallWatcher* watcher) {
        ReturnType reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            qCWarning(logLogin1).noquote()
                << "Failed to list" << name << "for" << iface->path() << "in" << (iface->interface() + ':')
                << reply.error().message();

            return;
        }

        for (const auto& object : reply.value())
            emit (iface->*Signal)(object.id, object.path);
    };

    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, iface, onFinished);
}

template <auto BindableProperty, typename Class>
void addObject(const QDBusObjectPath& object_path, QList<Class*>& list) {
    auto* parent = Login1::instance();

    QString path = object_path.path();
    auto objects = (parent->*BindableProperty).value();

    if (auto it = objects.constFind(path); it != objects.cend())
        return;
    
    objects.insert(path, new Class(path, parent));
    list = objects.values();
    parent->*BindableProperty = std::move(objects);
}

template <auto BindableProperty, typename Class>
void removeObject(const QDBusObjectPath& object_path, QList<Class*>& list) {
    auto* parent = Login1::instance();

    QString path = object_path.path();
    auto objects = (parent->*BindableProperty).value();

    if (auto it = objects.constFind(path); it != objects.cend()) {
        (*it)->deleteLater();
        objects.erase(it);
        list = objects.values();
        parent->*BindableProperty = std::move(objects);
    }
}

} // namespace

Login1::Login1() {
    qDBusRegisterMetaType<manager::ShutdownOperation>();
    qDBusRegisterMetaType<manager::Session>();
    qDBusRegisterMetaType<manager::Sessions>();
    qDBusRegisterMetaType<manager::User>();
    qDBusRegisterMetaType<manager::Users>();
    qDBusRegisterMetaType<manager::Seat>();
    qDBusRegisterMetaType<manager::Seats>();
    qDBusRegisterMetaType<manager::Inhibitor>();
    qDBusRegisterMetaType<manager::Inhibitors>();
    qDBusRegisterMetaType<session::User>();
    qDBusRegisterMetaType<user::Session>();
    qDBusRegisterMetaType<user::Sessions>();

    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logLogin1).noquote()
            << "Failed to initialize Login1:"
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_iface = new Login1ManagerInterface(LOGIN1_SERVICE, LOGIN1_OBJECT, systemBus, this);

    connect(m_iface, &Login1ManagerInterface::SessionNew, this, &Login1::addSession);
    connect(m_iface, &Login1ManagerInterface::SessionRemoved, this, &Login1::removeSession);
    connect(m_iface, &Login1ManagerInterface::UserNew, this, &Login1::addUser);
    connect(m_iface, &Login1ManagerInterface::UserRemoved, this, &Login1::removeUser);
    connect(m_iface, &Login1ManagerInterface::SeatNew, this, &Login1::addSeat);
    connect(m_iface, &Login1ManagerInterface::SeatRemoved, this, &Login1::removeSeat);
    connect(m_iface, &Login1ManagerInterface::PrepareForShutdown, this, &Login1::prepareForShutdown);
    connect(m_iface, &Login1ManagerInterface::PrepareForShutdownWithMetadata, this, &Login1::prepareForShutdownWithMetadata);
    connect(m_iface, &Login1ManagerInterface::PrepareForSleep, this, &Login1::prepareForSleep);
    connect(m_iface, &Login1ManagerInterface::SecureAttentionKey, this, &Login1::emitSecureAttentionKey);

    initializeSessions();
    initializeUsers();
    initializeSeats();

    initializeCurrentObjects();
    
    propertiesGroup.initialize(m_iface);
    propertiesGroup.initAllViaGetAll();
}

Login1* Login1::instance() {
    static auto* instance = new Login1;

    return instance;
}

Login1* Login1::create(QQmlEngine*, QJSEngine*) {
    auto* login1 = instance();
    QJSEngine::setObjectOwnership(login1, QJSEngine::CppOwnership);

    return login1;
}

QQmlListProperty<Session> Login1::sessions() {
    return {
        this, nullptr,
        &Login1::sessionsCount,
        &Login1::session
    };
}

QQmlListProperty<User> Login1::users() {
    return {
        this, nullptr,
        &Login1::usersCount,
        &Login1::user
    };
}

QQmlListProperty<Seat> Login1::seats() {
    return {
        this, nullptr,
        &Login1::seatsCount,
        &Login1::seat
    };
}

void Login1::setWallMessage(const QString& wallMessage) {
    propertiesGroup.setPropertyViaSet(
        "WallMessage",
        wallMessage
    );
}

void Login1::setEnableWallMessages(bool enableWallMessages) {
    propertiesGroup.setPropertyViaSet(
        "EnableWallMessages",
        enableWallMessages
    );
}

void Login1::attachDevice(const QString& seat_id, const QString& sysfs_path, bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("attachDevice"),
        &Login1ManagerInterface::AttachDevice,
        seat_id, sysfs_path, interactive
    );
}

dbus::AsyncCall* Login1::canHalt() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canHalt"),
        &Login1ManagerInterface::CanHalt
    );
}

dbus::AsyncCall* Login1::canHibernate() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canHibernate"),
        &Login1ManagerInterface::CanHibernate
    );
}

dbus::AsyncCall* Login1::canHybridSleep() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canHybridSleep"),
        &Login1ManagerInterface::CanHybridSleep
    );
}

dbus::AsyncCall* Login1::canPowerOff() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canPowerOff"),
        &Login1ManagerInterface::CanPowerOff
    );
}

dbus::AsyncCall* Login1::canReboot() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canReboot"),
        &Login1ManagerInterface::CanReboot
    );
}

dbus::AsyncCall* Login1::canRebootParameter() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canRebootParameter"),
        &Login1ManagerInterface::CanRebootParameter
    );
}

dbus::AsyncCall* Login1::canRebootToBootLoaderEntry() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canRebootToBootLoaderEntry"),
        &Login1ManagerInterface::CanRebootToBootLoaderEntry
    );
}

dbus::AsyncCall* Login1::canRebootToBootLoaderMenu() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canRebootToBootLoaderMenu"),
        &Login1ManagerInterface::CanRebootToBootLoaderMenu
    );
}

dbus::AsyncCall* Login1::canRebootToFirmwareSetup() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canRebootToFirmwareSetup"),
        &Login1ManagerInterface::CanRebootToFirmwareSetup
    );
}

dbus::AsyncCall* Login1::canSleep() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canSleep"),
        &Login1ManagerInterface::CanSleep
    );
}

dbus::AsyncCall* Login1::canSuspend() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canSuspend"),
        &Login1ManagerInterface::CanSuspend
    );
}

dbus::AsyncCall* Login1::canSuspendThenHibernate() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("canSuspendThenHibernate"),
        &Login1ManagerInterface::CanSuspendThenHibernate
    );
}

dbus::AsyncCall* Login1::cancelScheduledShutdown() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("cancelScheduledShutdown"),
        &Login1ManagerInterface::CancelScheduledShutdown
    );
}

dbus::AsyncCall* Login1::listInhibitors() {
    return new dbus::AsyncCall(
        logLogin1,
        m_iface,
        QStringLiteral("listInhibitors"),
        &Login1ManagerInterface::ListInhibitors
    );
}

void Login1::flushDevices(bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("flushDevices"),
        &Login1ManagerInterface::FlushDevices,
        interactive
    );
}

void Login1::halt(bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("halt"),
        &Login1ManagerInterface::Halt,
        interactive
    );
}

void Login1::haltWithFlags(manager::PowerActionFlags::Flags flags) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("haltWithFlags"),
        &Login1ManagerInterface::HaltWithFlags,
        flags
    );
}

void Login1::hibernate(bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("hibernate"),
        &Login1ManagerInterface::Hibernate,
        interactive
    );
}

void Login1::hibernateWithFlags(manager::PowerActionFlags::Flags flags) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("hibernateWithFlags"),
        &Login1ManagerInterface::HibernateWithFlags,
        flags
    );
}

void Login1::hybridSleep(bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("hybridSleep"),
        &Login1ManagerInterface::HybridSleep,
        interactive
    );
}

void Login1::hybridSleepWithFlags(manager::PowerActionFlags::Flags flags) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("hybridSleepWithFlags"),
        &Login1ManagerInterface::HybridSleepWithFlags,
        flags
    );
}

void Login1::lockSessions() {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("lockSessions"),
        &Login1ManagerInterface::LockSessions
    );
}

void Login1::powerOff(bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("powerOff"),
        &Login1ManagerInterface::PowerOff,
        interactive
    );
}

void Login1::powerOffWithFlags(manager::PowerActionFlags::Flags flags) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("powerOffWithFlags"),
        &Login1ManagerInterface::PowerOffWithFlags,
        flags
    );
}

void Login1::reboot(bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("reboot"),
        &Login1ManagerInterface::Reboot,
        interactive
    );
}

void Login1::rebootWithFlags(manager::PowerActionFlags::Flags flags) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("rebootWithFlags"),
        &Login1ManagerInterface::RebootWithFlags,
        flags
    );
}

void Login1::scheduleShutdown(const QString& type, qulonglong usec) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("scheduleShutdown"),
        &Login1ManagerInterface::ScheduleShutdown,
        type, usec
    );
}

void Login1::setRebootParameter(const QString& parameter) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("setRebootParameter"),
        &Login1ManagerInterface::SetRebootParameter,
        parameter
    );
}

void Login1::setRebootToBootLoaderEntry(const QString& boot_loader_entry) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("setRebootToBootLoaderEntry"),
        &Login1ManagerInterface::SetRebootToBootLoaderEntry,
        boot_loader_entry
    );
}

void Login1::setRebootToBootLoaderMenu(qulonglong timeout) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("setRebootToBootLoaderMenu"),
        &Login1ManagerInterface::SetRebootToBootLoaderMenu,
        timeout
    );
}

void Login1::setRebootToFirmwareSetup(bool enable) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("setRebootToFirmwareSetup"),
        &Login1ManagerInterface::SetRebootToFirmwareSetup,
        enable
    );
}

void Login1::setUserLinger(uint uid, bool enable, bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("setUserLinger"),
        &Login1ManagerInterface::SetUserLinger,
        uid, enable, interactive
    );
}

void Login1::sleep(qulonglong flags) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("sleep"),
        &Login1ManagerInterface::Sleep,
        flags
    );
}

void Login1::suspend(bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("suspend"),
        &Login1ManagerInterface::Suspend,
        interactive
    );
}

void Login1::suspendThenHibernate(bool interactive) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("suspendThenHibernate"),
        &Login1ManagerInterface::SuspendThenHibernate,
        interactive
    );
}

void Login1::suspendThenHibernateWithFlags(manager::PowerActionFlags::Flags flags) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("suspendThenHibernateWithFlags"),
        &Login1ManagerInterface::SuspendThenHibernateWithFlags,
        flags
    );
}

void Login1::suspendWithFlags(manager::PowerActionFlags::Flags flags) {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("suspendWithFlags"),
        &Login1ManagerInterface::SuspendWithFlags,
        flags
    );
}

void Login1::unlockSessions() {
    dbus::callAsync(
        logLogin1,
        m_iface,
        QStringLiteral("unlockSessions"),
        &Login1ManagerInterface::UnlockSessions
    );
}

void Login1::addSession(const QString&, const QDBusObjectPath& object_path)
{
    addObject<&Login1::b_sessions, Session>(object_path, m_sessions);
}

void Login1::addUser(int, const QDBusObjectPath& object_path)
{
    addObject<&Login1::b_users, User>(object_path, m_users);
}

void Login1::addSeat(const QString&, const QDBusObjectPath& object_path)
{
    addObject<&Login1::b_seats, Seat>(object_path, m_seats);
}

void Login1::removeSession(const QString&, const QDBusObjectPath& object_path)
{
    removeObject<&Login1::b_sessions, Session>(object_path, m_sessions);
}

void Login1::removeUser(int, const QDBusObjectPath& object_path)
{
    removeObject<&Login1::b_users, User>(object_path, m_users);
}

void Login1::removeSeat(const QString&, const QDBusObjectPath& object_path)
{
    removeObject<&Login1::b_seats, Seat>(object_path, m_seats);
}

void Login1::emitSecureAttentionKey(const QString& seat_id, const QDBusObjectPath& object_path)
{
    emit secureAttentionKey(seat_id, b_seats.value().value(object_path.path()));
}

qsizetype Login1::sessionsCount(QQmlListProperty<Session>* list) { return utils::objectCount<Session>(list, &Login1::m_sessions); }
qsizetype Login1::usersCount(QQmlListProperty<User>* list) { return utils::objectCount<User>(list, &Login1::m_users); }
qsizetype Login1::seatsCount(QQmlListProperty<Seat>* list) { return utils::objectCount<Seat>(list, &Login1::m_seats); }

Session* Login1::session(QQmlListProperty<Session>* list, qsizetype index) { return utils::objectAt<Session>(list, index, &Login1::m_sessions); }
User* Login1::user(QQmlListProperty<User>* list, qsizetype index) { return utils::objectAt<User>(list, index, &Login1::m_users); }
Seat* Login1::seat(QQmlListProperty<Seat>* list, qsizetype index) { return utils::objectAt<Seat>(list, index, &Login1::m_seats); }

void Login1::initializeSessions() {
    initializeObjects<
        &Login1ManagerInterface::ListSessions,
        &Login1ManagerInterface::SessionNew
    >(m_iface, QStringLiteral("sessions"));
}

void Login1::initializeUsers() {
    initializeObjects<
        &Login1ManagerInterface::ListUsers,
        &Login1ManagerInterface::UserNew
    >(m_iface, QStringLiteral("users"));
}

void Login1::initializeSeats() {
    initializeObjects<
        &Login1ManagerInterface::ListSeats,
        &Login1ManagerInterface::SeatNew
    >(m_iface, QStringLiteral("seats"));
}

void Login1::initializeCurrentObjects() {
    auto* watcher = new QDBusPendingCallWatcher(
        m_iface->GetUserByPID(QCoreApplication::applicationPid()),
        this
    );

    auto onFinished = [this](QDBusPendingCallWatcher* watcher) {
        QDBusPendingReply<QDBusObjectPath> reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            qCWarning(logLogin1).noquote()
                << "Failed to to initialize current user for" << m_iface->path() << "in" << (m_iface->interface() + ':')
                << reply.error().message();

            return;
        }

        const auto path = reply.value().path();

        b_currentUser.setBinding([=, this]() {
            return b_users.value().value(path);
        });

        b_currentSession.setBinding([this]() -> Session* {
            if (auto* user = b_currentUser.value())
                return user->bindableDisplay().value();

            return nullptr;
        });

        b_currentSeat.setBinding([this]() -> Seat* {
            if (auto* user = b_currentUser.value())
                if (auto* session = user->bindableDisplay().value())
                    return session->bindableSeat().value();

            return nullptr;
        });
    };

    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, m_iface, onFinished);
}

} // namespace extrashell::login1

#include "moc_qml.cpp"