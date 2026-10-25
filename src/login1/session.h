#pragma once

#include <qloggingcategory.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qtypes.h>

#include "types.h"

#include "../dbus/properties.h"

Q_MOC_INCLUDE("user.h")
Q_MOC_INCLUDE("seat.h")

Q_DECLARE_LOGGING_CATEGORY(logLogin1Session)

class Login1SessionInterface;

namespace extrashell::login1 {

class User;
class Seat;

class Session : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(Login1Session)
    QML_UNCREATABLE("Session must be acquired from the Login1.")

    Q_PROPERTY(extrashell::login1::User* user READ default NOTIFY userChanged BINDABLE bindableUser)
    Q_PROPERTY(extrashell::login1::Seat* seat READ default NOTIFY seatChanged BINDABLE bindableSeat)
    Q_PROPERTY(QString desktop READ default NOTIFY desktopChanged BINDABLE bindableDesktop)
    Q_PROPERTY(QString display READ default NOTIFY displayChanged BINDABLE bindableDisplay)
    Q_PROPERTY(QString id READ default NOTIFY idChanged BINDABLE bindableId)
    Q_PROPERTY(QString name READ default NOTIFY nameChanged BINDABLE bindableName)
    Q_PROPERTY(QString remoteHost READ default NOTIFY remoteHostChanged BINDABLE bindableRemoteHost)
    Q_PROPERTY(QString remoteUser READ default NOTIFY remoteUserChanged BINDABLE bindableRemoteUser)
    Q_PROPERTY(QString scope READ default NOTIFY scopeChanged BINDABLE bindableScope)
    Q_PROPERTY(QString service READ default NOTIFY serviceChanged BINDABLE bindableService)
    Q_PROPERTY(QString class READ default NOTIFY classChanged BINDABLE bindableClass)
    Q_PROPERTY(QString state READ default NOTIFY stateChanged BINDABLE bindableState)
    Q_PROPERTY(QString tty READ default NOTIFY ttyChanged BINDABLE bindableTTY)
    Q_PROPERTY(QString type READ default NOTIFY typeChanged BINDABLE bindableType)
    Q_PROPERTY(qulonglong idleSinceHint READ default NOTIFY idleSinceHintChanged BINDABLE bindableIdleSinceHint)
    Q_PROPERTY(qulonglong idleSinceHintMonotonic READ default NOTIFY idleSinceHintMonotonicChanged BINDABLE bindableIdleSinceHintMonotonic)
    Q_PROPERTY(qulonglong leaderPIDFDId READ default NOTIFY leaderPIDFDIdChanged BINDABLE bindableLeaderPIDFDId)
    Q_PROPERTY(qulonglong timestamp READ default NOTIFY timestampChanged BINDABLE bindableTimestamp)
    Q_PROPERTY(qulonglong timestampMonotonic READ default NOTIFY timestampMonotonicChanged BINDABLE bindableTimestampMonotonic)
    Q_PROPERTY(uint audit READ default NOTIFY auditChanged BINDABLE bindableAudit)
    Q_PROPERTY(uint leader READ default NOTIFY leaderChanged BINDABLE bindableLeader)
    Q_PROPERTY(uint vtnr READ default NOTIFY vtnrChanged BINDABLE bindableVtnr)
    Q_PROPERTY(bool active READ default NOTIFY activeChanged BINDABLE bindableActive)
    Q_PROPERTY(bool canIdle READ default NOTIFY canIdleChanged BINDABLE bindableCanIdle)
    Q_PROPERTY(bool canLock READ default NOTIFY canLockChanged BINDABLE bindableCanLock)
    Q_PROPERTY(bool idleHint READ default NOTIFY idleHintChanged BINDABLE bindableIdleHint)
    Q_PROPERTY(bool lockedHint READ default NOTIFY lockedHintChanged BINDABLE bindableLockedHint)
    Q_PROPERTY(bool remote READ default NOTIFY remoteChanged BINDABLE bindableRemote)

  public:
    explicit Session(const QString& path, QObject* parent = nullptr);

    [[nodiscard]] QBindable<User*> bindableUser() const { return &b_user; }
    [[nodiscard]] QBindable<Seat*> bindableSeat() const { return &b_seat; }

    [[nodiscard]] QBindable<session::User> bindableSessionUser() const { return &b_sessionUser; }
    [[nodiscard]] QBindable<session::Seat> bindableSessionSeat() const { return &b_sessionSeat; }
    [[nodiscard]] QBindable<QString> bindableDesktop() const { return &b_desktop; }
    [[nodiscard]] QBindable<QString> bindableDisplay() const { return &b_display; }
    [[nodiscard]] QBindable<QString> bindableId() const { return &b_id; }
    [[nodiscard]] QBindable<QString> bindableName() const { return &b_name; }
    [[nodiscard]] QBindable<QString> bindableRemoteHost() const { return &b_remoteHost; }
    [[nodiscard]] QBindable<QString> bindableRemoteUser() const { return &b_remoteUser; }
    [[nodiscard]] QBindable<QString> bindableScope() const { return &b_scope; }
    [[nodiscard]] QBindable<QString> bindableService() const { return &b_service; }
    [[nodiscard]] QBindable<QString> bindableClass() const { return &b_class; }
    [[nodiscard]] QBindable<QString> bindableState() const { return &b_state; }
    [[nodiscard]] QBindable<QString> bindableTTY() const { return &b_tty; }
    [[nodiscard]] QBindable<QString> bindableType() const { return &b_type; }
    [[nodiscard]] QBindable<qulonglong> bindableIdleSinceHint() const { return &b_idleSinceHint; }
    [[nodiscard]] QBindable<qulonglong> bindableIdleSinceHintMonotonic() const { return &b_idleSinceHintMonotonic; }
    [[nodiscard]] QBindable<qulonglong> bindableLeaderPIDFDId() const { return &b_leaderPIDFDId; }
    [[nodiscard]] QBindable<qulonglong> bindableTimestamp() const { return &b_timestamp; }
    [[nodiscard]] QBindable<qulonglong> bindableTimestampMonotonic() const { return &b_timestampMonotonic; }
    [[nodiscard]] QBindable<uint> bindableAudit() const { return &b_audit; }
    [[nodiscard]] QBindable<uint> bindableLeader() const { return &b_leader; }
    [[nodiscard]] QBindable<uint> bindableVtnr() const { return &b_vtnr; }
    [[nodiscard]] QBindable<bool> bindableActive() const { return &b_active; }
    [[nodiscard]] QBindable<bool> bindableCanIdle() const { return &b_canIdle; }
    [[nodiscard]] QBindable<bool> bindableCanLock() const { return &b_canLock; }
    [[nodiscard]] QBindable<bool> bindableIdleHint() const { return &b_idleHint; }
    [[nodiscard]] QBindable<bool> bindableLockedHint() const { return &b_lockedHint; }
    [[nodiscard]] QBindable<bool> bindableRemote() const { return &b_remote; }

    Q_INVOKABLE void activate();
    Q_INVOKABLE void kill(const QString& whom, int signal_number);
    Q_INVOKABLE void lock();
    Q_INVOKABLE void releaseControl();
    Q_INVOKABLE void setBrightness(const QString& subsystem, const QString& name, uint brightness);
    Q_INVOKABLE void setDisplay(const QString& display);
    Q_INVOKABLE void setIdleHint(bool idle);
    Q_INVOKABLE void setLockedHint(bool locked);
    Q_INVOKABLE void setClass(const QString& session_class);
    Q_INVOKABLE void setTTY(int tty_fd);
    Q_INVOKABLE void setType(const QString &type);
    Q_INVOKABLE void takeControl(bool force);
    Q_INVOKABLE void terminate();
    Q_INVOKABLE void unlock();

  signals:
    void lockRequested();
    void unlockRequested();

    void userChanged();
    void seatChanged();
    void desktopChanged();
    void displayChanged();
    void idChanged();
    void nameChanged();
    void remoteHostChanged();
    void remoteUserChanged();
    void scopeChanged();
    void serviceChanged();
    void classChanged();
    void stateChanged();
    void ttyChanged();
    void typeChanged();
    void idleSinceHintChanged();
    void idleSinceHintMonotonicChanged();
    void leaderPIDFDIdChanged();
    void timestampChanged();
    void timestampMonotonicChanged();
    void auditChanged();
    void leaderChanged();
    void vtnrChanged();
    void activeChanged();
    void canIdleChanged();
    void canLockChanged();
    void idleHintChanged();
    void lockedHintChanged();
    void remoteChanged();

  private:
    Login1SessionInterface* m_iface = nullptr;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    Q_OBJECT_BINDABLE_PROPERTY(Session, User*, b_user)
    Q_OBJECT_BINDABLE_PROPERTY(Session, Seat*, b_seat)

    ES_DBUS_BINDABLE_PROPERTY(Session, session::User, b_sessionUser, userChanged, "User", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, session::Seat, b_sessionSeat, seatChanged, "Seat", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_desktop, desktopChanged, "Desktop", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_display, displayChanged, "Display", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_id, idChanged, "Id", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_name, nameChanged, "Name", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_remoteHost, remoteHostChanged, "RemoteHost", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_remoteUser, remoteUserChanged, "RemoteUser", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_scope, scopeChanged, "Scope", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_service, serviceChanged, "Service", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_class, classChanged, "Class", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_state, stateChanged, "State", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_tty, ttyChanged, "TTY", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, QString, b_type, typeChanged, "Type", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, qulonglong, b_idleSinceHint, idleSinceHintChanged, "IdleSinceHint", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, qulonglong, b_idleSinceHintMonotonic, idleSinceHintMonotonicChanged, "IdleSinceHintMonotonic", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, qulonglong, b_leaderPIDFDId, leaderPIDFDIdChanged, "LeaderPIDFDId", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, qulonglong, b_timestamp, timestampChanged, "Timestamp", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, qulonglong, b_timestampMonotonic, timestampMonotonicChanged, "TimestampMonotonic", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, uint, b_audit, auditChanged, "Audit", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, uint, b_leader, leaderChanged, "Leader", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, uint, b_vtnr, vtnrChanged, "VTNr", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, bool, b_active, activeChanged, "Active", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, bool, b_canIdle, canIdleChanged, "CanIdle", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, bool, b_canLock, canLockChanged, "CanLock", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, bool, b_idleHint, idleHintChanged, "IdleHint", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, bool, b_lockedHint, lockedHintChanged, "LockedHint", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Session, bool, b_remote, remoteChanged, "Remote", propertiesGroup)
};

} // namespace extrashell::login1