#pragma once

#include <qloggingcategory.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qqmllist.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qtypes.h>

#include "types.h"

#include "../dbus/properties.h"

Q_MOC_INCLUDE("session.h")

Q_DECLARE_LOGGING_CATEGORY(logLogin1User)

template <typename T>
class QList;

class Login1UserInterface;

namespace extrashell::login1 {

class Session;

class User : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(Login1User)
    QML_UNCREATABLE("User must be acquired from the Login1.")

    Q_PROPERTY(extrashell::login1::Session* display READ default NOTIFY displayChanged BINDABLE bindableDisplay)
    Q_PROPERTY(QQmlListProperty<extrashell::login1::Session> sessions READ sessions NOTIFY sessionsChanged)
    Q_PROPERTY(QString name READ default NOTIFY nameChanged BINDABLE bindableName)
    Q_PROPERTY(QString runtimePath READ default NOTIFY runtimePathChanged BINDABLE bindableRuntimePath)
    Q_PROPERTY(QString service READ default NOTIFY serviceChanged BINDABLE bindableService)
    Q_PROPERTY(QString slice READ default NOTIFY sliceChanged BINDABLE bindableSlice)
    Q_PROPERTY(QString state READ default NOTIFY stateChanged BINDABLE bindableState)
    Q_PROPERTY(qulonglong idleSinceHint READ default NOTIFY idleSinceHintChanged BINDABLE bindableIdleSinceHint)
    Q_PROPERTY(qulonglong idleSinceHintMonotonic READ default NOTIFY idleSinceHintMonotonicChanged BINDABLE bindableIdleSinceHintMonotonic)
    Q_PROPERTY(qulonglong timestamp READ default NOTIFY timestampChanged BINDABLE bindableTimestamp)
    Q_PROPERTY(qulonglong timestampMonotonic READ default NOTIFY timestampMonotonicChanged BINDABLE bindableTimestampMonotonic)
    Q_PROPERTY(uint gid READ default NOTIFY gidChanged BINDABLE bindableGID)
    Q_PROPERTY(uint uid READ default NOTIFY uidChanged BINDABLE bindableUID)
    Q_PROPERTY(bool idleHint READ default NOTIFY idleHintChanged BINDABLE bindableIdleHint)
    Q_PROPERTY(bool linger READ default NOTIFY lingerChanged BINDABLE bindableLinger)

  public:
    explicit User(const QString& path, QObject* parent = nullptr);

    [[nodiscard]] QBindable<Session*> bindableDisplay() const { return &b_display; }
    [[nodiscard]] QBindable<user::Session> bindableUserDisplay() const { return &b_userDisplay; }
    [[nodiscard]] QBindable<user::Sessions> bindableUserSessions() const { return &b_userSessions; }
    [[nodiscard]] QBindable<QString> bindableName() const { return &b_name; }
    [[nodiscard]] QBindable<QString> bindableRuntimePath() const { return &b_runtimePath; }
    [[nodiscard]] QBindable<QString> bindableService() const { return &b_service; }
    [[nodiscard]] QBindable<QString> bindableSlice() const { return &b_slice; }
    [[nodiscard]] QBindable<QString> bindableState() const { return &b_state; }
    [[nodiscard]] QBindable<qulonglong> bindableIdleSinceHint() const { return &b_idleSinceHint; }
    [[nodiscard]] QBindable<qulonglong> bindableIdleSinceHintMonotonic() const { return &b_idleSinceHintMonotonic; }
    [[nodiscard]] QBindable<qulonglong> bindableTimestamp() const { return &b_timestamp; }
    [[nodiscard]] QBindable<qulonglong> bindableTimestampMonotonic() const { return &b_timestampMonotonic; }
    [[nodiscard]] QBindable<uint> bindableGID() const { return &b_gid; }
    [[nodiscard]] QBindable<uint> bindableUID() const { return &b_uid; }
    [[nodiscard]] QBindable<bool> bindableIdleHint() const { return &b_idleHint; }
    [[nodiscard]] QBindable<bool> bindableLinger() const { return &b_linger; }

    QQmlListProperty<Session> sessions();

    Q_INVOKABLE void kill(int signal_number);
    Q_INVOKABLE void terminate();

  signals:
    void displayChanged();
    void sessionsChanged();
    void nameChanged();
    void runtimePathChanged();
    void serviceChanged();
    void sliceChanged();
    void stateChanged();
    void idleSinceHintChanged();
    void idleSinceHintMonotonicChanged();
    void timestampChanged();
    void timestampMonotonicChanged();
    void gidChanged();
    void uidChanged();
    void idleHintChanged();
    void lingerChanged();

  private:
    Login1UserInterface* m_iface = nullptr;

    static qsizetype sessionsCount(QQmlListProperty<Session>*);
    static Session* session(QQmlListProperty<Session>*, qsizetype);

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    Q_OBJECT_BINDABLE_PROPERTY(User, Session*, b_display, &User::displayChanged)
    Q_OBJECT_BINDABLE_PROPERTY(User, QList<Session*>, b_sessions, &User::sessionsChanged)

    ES_DBUS_BINDABLE_PROPERTY(User, user::Session, b_userDisplay, "Display", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, user::Sessions, b_userSessions, "Sessions", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, QString, b_name, nameChanged, "Name", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, QString, b_runtimePath, runtimePathChanged, "RuntimePath", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, QString, b_service, serviceChanged, "Service", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, QString, b_slice, sliceChanged, "Slice", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, QString, b_state, stateChanged, "State", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, qulonglong, b_idleSinceHint, idleSinceHintChanged, "IdleSinceHint", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, qulonglong, b_idleSinceHintMonotonic, idleSinceHintMonotonicChanged, "IdleSinceHintMonotonic", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, qulonglong, b_timestamp, timestampChanged, "Timestamp", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, qulonglong, b_timestampMonotonic, timestampMonotonicChanged, "TimestampMonotonic", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, uint, b_gid, gidChanged, "GID", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, uint, b_uid, uidChanged, "UID", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, bool, b_idleHint, idleHintChanged, "IdleHint", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(User, bool, b_linger, lingerChanged, "Linger", propertiesGroup)
};

} // namespace extrashell::login1