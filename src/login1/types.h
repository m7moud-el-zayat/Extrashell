#pragma once

#include <qdbusargument.h>
#include <qdbusextratypes.h>
#include <qdebug.h>
#include <qflags.h>
#include <qlist.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qtypes.h>

namespace extrashell::login1::manager {

class PowerActionFlags : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

  public:
    enum Flag : quint8 {
        RootCheckInhibitors,
        KexecReboot,
        SoftReboot,
        SoftRebootIfNextRootSetUp,
        SkipInhibitors
    };

    Q_ENUM(Flag)
    Q_DECLARE_FLAGS(Flags, Flag)
    Q_FLAG(Flags)
};

class ShutdownOperation {
    Q_GADGET

    Q_PROPERTY(QString type MEMBER type)
    Q_PROPERTY(quint64 usec MEMBER usec)

  public:
    QString type;
    quint64 usec;

    bool operator==(const ShutdownOperation&) const = default;
};

QDBusArgument &operator<<(QDBusArgument& argument, const ShutdownOperation& shutdownOperation);
const QDBusArgument &operator>>(const QDBusArgument& argument, ShutdownOperation& shutdownOperation);

struct Session {
    QString id;
    quint32 userId;
    QString userName;
    QString seatId;
    QDBusObjectPath path;
};

QDBusArgument &operator<<(QDBusArgument& argument, const Session& session);
const QDBusArgument &operator>>(const QDBusArgument& argument, Session& session);

struct User {
    quint32 id;
    QString name;
    QDBusObjectPath path;
};

QDBusArgument &operator<<(QDBusArgument& argument, const User& user);
const QDBusArgument &operator>>(const QDBusArgument& argument, User& user);

struct Seat {
    QString id;
    QDBusObjectPath path;

    bool operator==(const Seat&) const = default;
};

QDBusArgument &operator<<(QDBusArgument& argument, const Seat& seat);
const QDBusArgument &operator>>(const QDBusArgument& argument, Seat& seat);

class Inhibitor {
    Q_GADGET

    Q_PROPERTY(QString what MEMBER what)
    Q_PROPERTY(QString who MEMBER who)
    Q_PROPERTY(QString why MEMBER why)
    Q_PROPERTY(QString mode MEMBER mode)
    Q_PROPERTY(quint32 uid MEMBER uid)
    Q_PROPERTY(quint32 pid MEMBER pid)

  public:
    QString what;
    QString who;
    QString why;
    QString mode;
    quint32 uid;
    quint32 pid;
};

QDBusArgument &operator<<(QDBusArgument& argument, const Inhibitor& inhibitor);
const QDBusArgument &operator>>(const QDBusArgument& argument, Inhibitor& inhibitor);

using Sessions = QList<Session>;
using Users = QList<User>;
using Seats = QList<Seat>;
using Inhibitors = QList<Inhibitor>;

QDebug operator<<(QDebug debug, const ShutdownOperation& shutdownOperation);
QDebug operator<<(QDebug debug, const Seat& seat);

} // namespace extrashell::login1::manager

namespace extrashell::login1::session {

struct User {
    quint32 uid;
    QDBusObjectPath path;

    bool operator==(const User&) const = default;
};

QDBusArgument &operator<<(QDBusArgument& argument, const User& user);
const QDBusArgument &operator>>(const QDBusArgument& argument, User& user);

using Seat = manager::Seat;

QDebug operator<<(QDebug debug, const User& user);

} // namespace extrashell::login1::session

namespace extrashell::login1::user {

struct Session {
    QString id;
    QDBusObjectPath path;

    bool operator==(const Session&) const = default;
};

QDBusArgument &operator<<(QDBusArgument& argument, const Session& session);
const QDBusArgument &operator>>(const QDBusArgument& argument, Session& session);

using Sessions = QList<Session>;

QDebug operator<<(QDebug debug, const Session& session);

} // namespace extrashell::login1::user

namespace extrashell::login1::seat {

using Session = user::Session;
using Sessions = user::Sessions;

} // namespace extrashell::login1::user

Q_DECLARE_METATYPE(extrashell::login1::manager::ShutdownOperation)
Q_DECLARE_METATYPE(extrashell::login1::manager::Session)
Q_DECLARE_METATYPE(extrashell::login1::manager::Sessions)
Q_DECLARE_METATYPE(extrashell::login1::manager::User)
Q_DECLARE_METATYPE(extrashell::login1::manager::Users)
Q_DECLARE_METATYPE(extrashell::login1::manager::Seat)
Q_DECLARE_METATYPE(extrashell::login1::manager::Seats)
Q_DECLARE_METATYPE(extrashell::login1::manager::Inhibitor)
Q_DECLARE_METATYPE(extrashell::login1::manager::Inhibitors)

Q_DECLARE_METATYPE(extrashell::login1::session::User)

Q_DECLARE_METATYPE(extrashell::login1::user::Session)
Q_DECLARE_METATYPE(extrashell::login1::user::Sessions)