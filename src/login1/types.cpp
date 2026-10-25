#include "types.h"

namespace extrashell::login1::manager {

QDBusArgument& operator<<(QDBusArgument& argument, const ShutdownOperation& shutdownOperation) {
    argument.beginStructure();
    argument << shutdownOperation.type << shutdownOperation.usec;
    argument.endStructure();

    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, ShutdownOperation& shutdownOperation) {
    argument.beginStructure();
    argument >> shutdownOperation.type >> shutdownOperation.usec;
    argument.endStructure();

    return argument;
}

QDBusArgument& operator<<(QDBusArgument& argument, const Session& session) {
    argument.beginStructure();
    argument
        << session.id << session.userId << session.userName
        << session.seatId << session.path;
    argument.endStructure();

    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, Session& session) {
    argument.beginStructure();
    argument
        >> session.id >> session.userId >> session.userName
        >> session.seatId >> session.path;
    argument.endStructure();

    return argument;
}

QDBusArgument& operator<<(QDBusArgument& argument, const User& user) {
    argument.beginStructure();
    argument << user.id << user.name << user.path;
    argument.endStructure();

    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, User& user) {
    argument.beginStructure();
    argument >> user.id >> user.name >> user.path;
    argument.endStructure();

    return argument;
}

QDBusArgument& operator<<(QDBusArgument& argument, const Seat& seat) {
    argument.beginStructure();
    argument << seat.id << seat.path;
    argument.endStructure();

    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, Seat& seat) {
    argument.beginStructure();
    argument >> seat.id >> seat.path;
    argument.endStructure();

    return argument;
}

QDBusArgument& operator<<(QDBusArgument& argument, const Inhibitor& inhibitor) {
    argument.beginStructure();
    argument 
        << inhibitor.what << inhibitor.who << inhibitor.why
        << inhibitor.mode << inhibitor.uid << inhibitor.pid;
    argument.endStructure();

    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, Inhibitor& inhibitor) {
    argument.beginStructure();
    argument 
        >> inhibitor.what >> inhibitor.who >> inhibitor.why
        >> inhibitor.mode >> inhibitor.uid >> inhibitor.pid;
    argument.endStructure();

    return argument;
}

QDebug operator<<(QDebug debug, const ShutdownOperation& shutdownOperation) {
    QDebugStateSaver saver(debug);
    debug.nospace()
        << "ShutdownOperation(type=" << shutdownOperation.type
        << ", usec=" << shutdownOperation.usec << ')';

    return debug;
}

QDebug operator<<(QDebug debug, const Seat& seat) {
    QDebugStateSaver saver(debug);
    debug.nospace()
        << "Seat(id=" << seat.id
        << ", path=" << seat.path << ')';

    return debug;
}

} // namespace extrashell::login1::manager

namespace extrashell::login1::session {

QDBusArgument &operator<<(QDBusArgument& argument, const User& user) {
    argument.beginStructure();
    argument << user.uid << user.path;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument& argument, User& user) {
    argument.beginStructure();
    argument >> user.uid >> user.path;
    argument.endStructure();

    return argument;
}

QDebug operator<<(QDebug debug, const User& user) {
    QDebugStateSaver saver(debug);
    debug.nospace()
        << "Session(uid=" << user.uid
        << ", path=" << user.path << ')';

    return debug;
}

} // namespace extrashell::login1::session

namespace extrashell::login1::user {

QDBusArgument &operator<<(QDBusArgument& argument, const Session& session) {
    argument.beginStructure();
    argument << session.id << session.path;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument& argument, Session& session) {
    argument.beginStructure();
    argument >> session.id >> session.path;
    argument.endStructure();

    return argument;
}

QDebug operator<<(QDebug debug, const Session& session) {
    QDebugStateSaver saver(debug);
    debug.nospace()
        << "Session(id=" << session.id
        << ", path=" << session.path << ')';

    return debug;
}

} // namespace extrashell::login1::user

#include "moc_types.cpp"