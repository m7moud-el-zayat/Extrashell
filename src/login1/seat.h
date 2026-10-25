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

Q_DECLARE_LOGGING_CATEGORY(logLogin1Seat)

template <typename T>
class QList;

class Login1SeatInterface;

namespace extrashell::login1 {

class Session;

class Seat : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(Login1Seat)
    QML_UNCREATABLE("Seat must be acquired from the Login1.")

    Q_PROPERTY(extrashell::login1::Session* activeSession READ default NOTIFY activeSessionChanged BINDABLE bindableActiveSession)
    Q_PROPERTY(QQmlListProperty<extrashell::login1::Session> sessions READ sessions NOTIFY sessionsChanged)
    Q_PROPERTY(QString id READ default NOTIFY idChanged BINDABLE bindableId)
    Q_PROPERTY(qulonglong idleSinceHint READ default NOTIFY idleSinceHintChanged BINDABLE bindableIdleSinceHint)
    Q_PROPERTY(qulonglong idleSinceHintMonotonic READ default NOTIFY idleSinceHintMonotonicChanged BINDABLE bindableIdleSinceHintMonotonic)
    Q_PROPERTY(bool canGraphical READ default NOTIFY canGraphicalChanged BINDABLE bindableCanGraphical)
    Q_PROPERTY(bool canTTY READ default NOTIFY canTTYChanged BINDABLE bindableCanTTY)
    Q_PROPERTY(bool idleHint READ default NOTIFY idleHintChanged BINDABLE bindableIdleHint)

  public:
    explicit Seat(const QString& path, QObject* parent = nullptr);
    
    [[nodiscard]] QBindable<Session*> bindableActiveSession() const { return &b_activeSession; }
    [[nodiscard]] QBindable<seat::Session> bindableSeatActiveSession() const { return &b_seatActiveSession; }
    [[nodiscard]] QBindable<seat::Sessions> bindableSeatSessions() const { return &b_seatSessions; }
    [[nodiscard]] QBindable<QString> bindableId() const { return &b_id; }
    [[nodiscard]] QBindable<qulonglong> bindableIdleSinceHint() const { return &b_idleSinceHint; }
    [[nodiscard]] QBindable<qulonglong> bindableIdleSinceHintMonotonic() const { return &b_idleSinceHintMonotonic; }
    [[nodiscard]] QBindable<bool> bindableCanGraphical() const { return &b_canGraphical; }
    [[nodiscard]] QBindable<bool> bindableCanTTY() const { return &b_canTTY; }
    [[nodiscard]] QBindable<bool> bindableIdleHint() const { return &b_idleHint; }

    QQmlListProperty<Session> sessions();

    Q_INVOKABLE void activateSession(const QString& session_id);
    Q_INVOKABLE void switchTo(uint vtnr);
    Q_INVOKABLE void switchToNext();
    Q_INVOKABLE void switchToPrevious();
    Q_INVOKABLE void terminate();

  signals:
    void activeSessionChanged();
    void sessionsChanged();
    void idChanged();
    void idleSinceHintChanged();
    void idleSinceHintMonotonicChanged();
    void canGraphicalChanged();
    void canTTYChanged();
    void idleHintChanged();

  private:
    Login1SeatInterface* m_iface = nullptr;

    static qsizetype sessionsCount(QQmlListProperty<Session>*);
    static Session* session(QQmlListProperty<Session>*, qsizetype);

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    Q_OBJECT_BINDABLE_PROPERTY(Seat, Session*, b_activeSession, &Seat::activeSessionChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Seat, QList<Session*>, b_sessions, &Seat::sessionsChanged)

    ES_DBUS_BINDABLE_PROPERTY(Seat, seat::Session, b_seatActiveSession, "ActiveSession", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Seat, seat::Sessions, b_seatSessions, "Sessions", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Seat, QString, b_id, idChanged, "Id", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Seat, qulonglong, b_idleSinceHint, idleSinceHintChanged, "IdleSinceHint", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Seat, qulonglong, b_idleSinceHintMonotonic, idleSinceHintMonotonicChanged, "IdleSinceHintMonotonic", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Seat, bool, b_canGraphical, canGraphicalChanged, "CanGraphical", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Seat, bool, b_canTTY, canTTYChanged, "CanTTY", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Seat, bool, b_idleHint, idleHintChanged, "IdleHint", propertiesGroup)
};

} // namespace extrashell::login1