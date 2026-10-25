#pragma once

#include <qcontainerfwd.h>
#include <qlist.h>
#include <qloggingcategory.h>
#include <qmap.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qqmllist.h>
#include <qstring.h>
#include <qtclasshelpermacros.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <qvariant.h>

#include "types.h"

#include "../dbus/properties.h"

Q_MOC_INCLUDE("session.h")
Q_MOC_INCLUDE("user.h")
Q_MOC_INCLUDE("seat.h")

Q_DECLARE_LOGGING_CATEGORY(logLogin1)

class QDBusObjectPath;
class QQmlEngine;
class QJSEngine;
class Login1ManagerInterface;

namespace extrashell::dbus {

class AsyncCall;

} // namespace extrashell::dbus

namespace extrashell::login1 {

class Session;
class User;
class Seat;

class Login1 : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(extrashell::login1::Session* currentSession READ currentSession NOTIFY currentSessionChanged)
    Q_PROPERTY(extrashell::login1::User* currentUser READ currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(extrashell::login1::Seat* currentSeat READ currentSeat NOTIFY currentSeatChanged)
    Q_PROPERTY(QQmlListProperty<extrashell::login1::Session> sessions READ sessions NOTIFY sessionsChanged)
    Q_PROPERTY(QQmlListProperty<extrashell::login1::User> users READ users NOTIFY usersChanged)
    Q_PROPERTY(QQmlListProperty<extrashell::login1::Seat> seats READ seats NOTIFY seatsChanged)
    Q_PROPERTY(extrashell::login1::manager::ShutdownOperation scheduledShutdown READ default NOTIFY scheduledShutdownChanged BINDABLE bindableScheduledShutdown)
    Q_PROPERTY(QVariantMap preparingForShutdownWithMetadata READ default NOTIFY preparingForShutdownWithMetadataChanged BINDABLE bindablePreparingForShutdownWithMetadata)
    Q_PROPERTY(QStringList bootLoaderEntries READ default NOTIFY bootLoaderEntriesChanged BINDABLE bindableBootLoaderEntries)
    Q_PROPERTY(QStringList killExcludeUsers READ default NOTIFY killExcludeUsersChanged BINDABLE bindableKillExcludeUsers)
    Q_PROPERTY(QStringList killOnlyUsers READ default NOTIFY killOnlyUsersChanged BINDABLE bindableKillOnlyUsers)
    Q_PROPERTY(QStringList sleepOperation READ default NOTIFY sleepOperationChanged BINDABLE bindableSleepOperation)
    Q_PROPERTY(QString blockInhibited READ default NOTIFY blockInhibitedChanged BINDABLE bindableBlockInhibited)
    Q_PROPERTY(QString blockWeakInhibited READ default NOTIFY blockWeakInhibitedChanged BINDABLE bindableBlockWeakInhibited)
    Q_PROPERTY(QString delayInhibited READ default NOTIFY delayInhibitedChanged BINDABLE bindableDelayInhibited)
    Q_PROPERTY(QString designatedMaintenanceTime READ default NOTIFY designatedMaintenanceTimeChanged BINDABLE bindableDesignatedMaintenanceTime)
    Q_PROPERTY(QString handleHibernateKey READ default NOTIFY handleHibernateKeyChanged BINDABLE bindableHandleHibernateKey)
    Q_PROPERTY(QString handleHibernateKeyLongPress READ default NOTIFY handleHibernateKeyLongPressChanged BINDABLE bindableHandleHibernateKeyLongPress)
    Q_PROPERTY(QString handleLidSwitch READ default NOTIFY handleLidSwitchChanged BINDABLE bindableHandleLidSwitch)
    Q_PROPERTY(QString handleLidSwitchDocked READ default NOTIFY handleLidSwitchDockedChanged BINDABLE bindableHandleLidSwitchDocked)
    Q_PROPERTY(QString handleLidSwitchExternalPower READ default NOTIFY handleLidSwitchExternalPowerChanged BINDABLE bindableHandleLidSwitchExternalPower)
    Q_PROPERTY(QString handlePowerKey READ default NOTIFY handlePowerKeyChanged BINDABLE bindableHandlePowerKey)
    Q_PROPERTY(QString handlePowerKeyLongPress READ default NOTIFY handlePowerKeyLongPressChanged BINDABLE bindableHandlePowerKeyLongPress)
    Q_PROPERTY(QString handleRebootKey READ default NOTIFY handleRebootKeyChanged BINDABLE bindableHandleRebootKey)
    Q_PROPERTY(QString handleRebootKeyLongPress READ default NOTIFY handleRebootKeyLongPressChanged BINDABLE bindableHandleRebootKeyLongPress)
    Q_PROPERTY(QString handleSecureAttentionKey READ default NOTIFY handleSecureAttentionKeyChanged BINDABLE bindableHandleSecureAttentionKey)
    Q_PROPERTY(QString handleSuspendKey READ default NOTIFY handleSuspendKeyChanged BINDABLE bindableHandleSuspendKey)
    Q_PROPERTY(QString handleSuspendKeyLongPress READ default NOTIFY handleSuspendKeyLongPressChanged BINDABLE bindableHandleSuspendKeyLongPress)
    Q_PROPERTY(QString idleAction READ default NOTIFY idleActionChanged BINDABLE bindableIdleAction)
    Q_PROPERTY(QString rebootParameter READ default NOTIFY rebootParameterChanged BINDABLE bindableRebootParameter)
    Q_PROPERTY(QString rebootToBootLoaderEntry READ default NOTIFY rebootToBootLoaderEntryChanged BINDABLE bindableRebootToBootLoaderEntry)
    Q_PROPERTY(QString wallMessage READ default WRITE setWallMessage NOTIFY wallMessageChanged BINDABLE bindableWallMessage)
    Q_PROPERTY(qulonglong holdoffTimeoutUSec READ default NOTIFY holdoffTimeoutUSecChanged BINDABLE bindableHoldoffTimeoutUSec)
    Q_PROPERTY(qulonglong idleActionUSec READ default NOTIFY idleActionUSecChanged BINDABLE bindableIdleActionUSec)
    Q_PROPERTY(qulonglong idleSinceHint READ default NOTIFY idleSinceHintChanged BINDABLE bindableIdleSinceHint)
    Q_PROPERTY(qulonglong idleSinceHintMonotonic READ default NOTIFY idleSinceHintMonotonicChanged BINDABLE bindableIdleSinceHintMonotonic)
    Q_PROPERTY(qulonglong inhibitDelayMaxUSec READ default NOTIFY inhibitDelayMaxUSecChanged BINDABLE bindableInhibitDelayMaxUSec)
    Q_PROPERTY(qulonglong inhibitorsMax READ default NOTIFY inhibitorsMaxChanged BINDABLE bindableInhibitorsMax)
    Q_PROPERTY(qulonglong rebootToBootLoaderMenu READ default NOTIFY rebootToBootLoaderMenuChanged BINDABLE bindableRebootToBootLoaderMenu)
    Q_PROPERTY(qulonglong runtimeDirectoryInodesMax READ default NOTIFY runtimeDirectoryInodesMaxChanged BINDABLE bindableRuntimeDirectoryInodesMax)
    Q_PROPERTY(qulonglong runtimeDirectorySize READ default NOTIFY runtimeDirectorySizeChanged BINDABLE bindableRuntimeDirectorySize)
    Q_PROPERTY(qulonglong sessionsMax READ default NOTIFY sessionsMaxChanged BINDABLE bindableSessionsMax)
    Q_PROPERTY(qulonglong stopIdleSessionUSec READ default NOTIFY stopIdleSessionUSecChanged BINDABLE bindableStopIdleSessionUSec)
    Q_PROPERTY(qulonglong userStopDelayUSec READ default NOTIFY userStopDelayUSecChanged BINDABLE bindableUserStopDelayUSec)
    Q_PROPERTY(uint nAutoVTs READ default NOTIFY nAutoVTsChanged BINDABLE bindableNAutoVTs)
    Q_PROPERTY(bool docked READ default NOTIFY dockedChanged BINDABLE bindableDocked)
    Q_PROPERTY(bool enableWallMessages READ default WRITE setEnableWallMessages NOTIFY enableWallMessagesChanged BINDABLE bindableEnableWallMessages)
    Q_PROPERTY(bool idleHint READ default NOTIFY idleHintChanged BINDABLE bindableIdleHint)
    Q_PROPERTY(bool killUserProcesses READ default NOTIFY killUserProcessesChanged BINDABLE bindableKillUserProcesses)
    Q_PROPERTY(bool lidClosed READ default NOTIFY lidClosedChanged BINDABLE bindableLidClosed)
    Q_PROPERTY(bool onExternalPower READ default NOTIFY onExternalPowerChanged BINDABLE bindableOnExternalPower)
    Q_PROPERTY(bool preparingForShutdown READ default NOTIFY preparingForShutdownChanged BINDABLE bindablePreparingForShutdown)
    Q_PROPERTY(bool preparingForSleep READ default NOTIFY preparingForSleepChanged BINDABLE bindablePreparingForSleep)
    Q_PROPERTY(bool rebootToFirmwareSetup READ default NOTIFY rebootToFirmwareSetupChanged BINDABLE bindableRebootToFirmwareSetup)
    Q_PROPERTY(bool removeIPC READ default NOTIFY removeIPCChanged BINDABLE bindableRemoveIPC)

    using SessionsMap = QMap<QString, Session*>;
    using UsersMap = QMap<QString, User*>;
    using SeatsMap = QMap<QString, Seat*>;

  public:
    static Login1* instance();
    static Login1* create(QQmlEngine*, QJSEngine*);

    [[nodiscard]] Session* currentSession() const { return b_currentSession; };
    [[nodiscard]] User* currentUser() const { return b_currentUser; };
    [[nodiscard]] Seat* currentSeat() const { return b_currentSeat; };
    [[nodiscard]] QBindable<SessionsMap> bindableSessionsMap() const { return &b_sessions; }
    [[nodiscard]] QBindable<UsersMap> bindableUsersMap() const { return &b_users; }
    [[nodiscard]] QBindable<SeatsMap> bindableSeatsMap() const { return &b_seats; }
    [[nodiscard]] QBindable<manager::ShutdownOperation> bindableScheduledShutdown() const { return &b_scheduledShutdown; }
    [[nodiscard]] QBindable<QVariantMap> bindablePreparingForShutdownWithMetadata() const { return &b_preparingForShutdownWithMetadata; }
    [[nodiscard]] QBindable<QStringList> bindableBootLoaderEntries() const { return &b_bootLoaderEntries; }
    [[nodiscard]] QBindable<QStringList> bindableKillExcludeUsers() const { return &b_killExcludeUsers; }
    [[nodiscard]] QBindable<QStringList> bindableKillOnlyUsers() const { return &b_killOnlyUsers; }
    [[nodiscard]] QBindable<QStringList> bindableSleepOperation() const { return &b_sleepOperation; }
    [[nodiscard]] QBindable<QString> bindableBlockInhibited() const { return &b_blockInhibited; }
    [[nodiscard]] QBindable<QString> bindableBlockWeakInhibited() const { return &b_blockWeakInhibited; }
    [[nodiscard]] QBindable<QString> bindableDelayInhibited() const { return &b_delayInhibited; }
    [[nodiscard]] QBindable<QString> bindableDesignatedMaintenanceTime() const { return &b_designatedMaintenanceTime; }
    [[nodiscard]] QBindable<QString> bindableHandleHibernateKey() const { return &b_handleHibernateKey; }
    [[nodiscard]] QBindable<QString> bindableHandleHibernateKeyLongPress() const { return &b_handleHibernateKeyLongPress; }
    [[nodiscard]] QBindable<QString> bindableHandleLidSwitch() const { return &b_handleLidSwitch; }
    [[nodiscard]] QBindable<QString> bindableHandleLidSwitchDocked() const { return &b_handleLidSwitchDocked; }
    [[nodiscard]] QBindable<QString> bindableHandleLidSwitchExternalPower() const { return &b_handleLidSwitchExternalPower; }
    [[nodiscard]] QBindable<QString> bindableHandlePowerKey() const { return &b_handlePowerKey; }
    [[nodiscard]] QBindable<QString> bindableHandlePowerKeyLongPress() const { return &b_handlePowerKeyLongPress; }
    [[nodiscard]] QBindable<QString> bindableHandleRebootKey() const { return &b_handleRebootKey; }
    [[nodiscard]] QBindable<QString> bindableHandleRebootKeyLongPress() const { return &b_handleRebootKeyLongPress; }
    [[nodiscard]] QBindable<QString> bindableHandleSecureAttentionKey() const { return &b_handleSecureAttentionKey; }
    [[nodiscard]] QBindable<QString> bindableHandleSuspendKey() const { return &b_handleSuspendKey; }
    [[nodiscard]] QBindable<QString> bindableHandleSuspendKeyLongPress() const { return &b_handleSuspendKeyLongPress; }
    [[nodiscard]] QBindable<QString> bindableIdleAction() const { return &b_idleAction; }
    [[nodiscard]] QBindable<QString> bindableRebootParameter() const { return &b_rebootParameter; }
    [[nodiscard]] QBindable<QString> bindableRebootToBootLoaderEntry() const { return &b_rebootToBootLoaderEntry; }
    [[nodiscard]] QBindable<QString> bindableWallMessage() const { return &b_wallMessage; }
    [[nodiscard]] QBindable<qulonglong> bindableHoldoffTimeoutUSec() const { return &b_holdoffTimeoutUSec; }
    [[nodiscard]] QBindable<qulonglong> bindableIdleActionUSec() const { return &b_idleActionUSec; }
    [[nodiscard]] QBindable<qulonglong> bindableIdleSinceHint() const { return &b_idleSinceHint; }
    [[nodiscard]] QBindable<qulonglong> bindableIdleSinceHintMonotonic() const { return &b_idleSinceHintMonotonic; }
    [[nodiscard]] QBindable<qulonglong> bindableInhibitDelayMaxUSec() const { return &b_inhibitDelayMaxUSec; }
    [[nodiscard]] QBindable<qulonglong> bindableInhibitorsMax() const { return &b_inhibitorsMax; }
    [[nodiscard]] QBindable<qulonglong> bindableRebootToBootLoaderMenu() const { return &b_rebootToBootLoaderMenu; }
    [[nodiscard]] QBindable<qulonglong> bindableRuntimeDirectoryInodesMax() const { return &b_runtimeDirectoryInodesMax; }
    [[nodiscard]] QBindable<qulonglong> bindableRuntimeDirectorySize() const { return &b_runtimeDirectorySize; }
    [[nodiscard]] QBindable<qulonglong> bindableSessionsMax() const { return &b_sessionsMax; }
    [[nodiscard]] QBindable<qulonglong> bindableStopIdleSessionUSec() const { return &b_stopIdleSessionUSec; }
    [[nodiscard]] QBindable<qulonglong> bindableUserStopDelayUSec() const { return &b_userStopDelayUSec; }
    [[nodiscard]] QBindable<uint> bindableNAutoVTs() const { return &b_nAutoVTs; }
    [[nodiscard]] QBindable<bool> bindableDocked() const { return &b_docked; }
    [[nodiscard]] QBindable<bool> bindableEnableWallMessages() const { return &b_enableWallMessages; }
    [[nodiscard]] QBindable<bool> bindableIdleHint() const { return &b_idleHint; }
    [[nodiscard]] QBindable<bool> bindableKillUserProcesses() const { return &b_killUserProcesses; }
    [[nodiscard]] QBindable<bool> bindableLidClosed() const { return &b_lidClosed; }
    [[nodiscard]] QBindable<bool> bindableOnExternalPower() const { return &b_onExternalPower; }
    [[nodiscard]] QBindable<bool> bindablePreparingForShutdown() const { return &b_preparingForShutdown; }
    [[nodiscard]] QBindable<bool> bindablePreparingForSleep() const { return &b_preparingForSleep; }
    [[nodiscard]] QBindable<bool> bindableRebootToFirmwareSetup() const { return &b_rebootToFirmwareSetup; }
    [[nodiscard]] QBindable<bool> bindableRemoveIPC() const { return &b_removeIPC; }

    QQmlListProperty<Session> sessions();
    QQmlListProperty<User> users();
    QQmlListProperty<Seat> seats();

    void setWallMessage(const QString& wallMessage);
    void setEnableWallMessages(bool enableWallMessages);

    Q_INVOKABLE void attachDevice(const QString& seat_id, const QString& sysfs_path, bool interactive = false);
    Q_INVOKABLE dbus::AsyncCall* canHalt();
    Q_INVOKABLE dbus::AsyncCall* canHibernate();
    Q_INVOKABLE dbus::AsyncCall* canHybridSleep();
    Q_INVOKABLE dbus::AsyncCall* canPowerOff();
    Q_INVOKABLE dbus::AsyncCall* canReboot();
    Q_INVOKABLE dbus::AsyncCall* canRebootParameter();
    Q_INVOKABLE dbus::AsyncCall* canRebootToBootLoaderEntry();
    Q_INVOKABLE dbus::AsyncCall* canRebootToBootLoaderMenu();
    Q_INVOKABLE dbus::AsyncCall* canRebootToFirmwareSetup();
    Q_INVOKABLE dbus::AsyncCall* canSleep();
    Q_INVOKABLE dbus::AsyncCall* canSuspend();
    Q_INVOKABLE dbus::AsyncCall* canSuspendThenHibernate();
    Q_INVOKABLE dbus::AsyncCall* cancelScheduledShutdown();
    Q_INVOKABLE void flushDevices(bool interactive = false);
    Q_INVOKABLE void halt(bool interactive = false);
    Q_INVOKABLE void haltWithFlags(manager::PowerActionFlags::Flags flags);
    Q_INVOKABLE void hibernate(bool interactive = false);
    Q_INVOKABLE void hibernateWithFlags(manager::PowerActionFlags::Flags flags);
    Q_INVOKABLE void hybridSleep(bool interactive = false);
    Q_INVOKABLE void hybridSleepWithFlags(manager::PowerActionFlags::Flags flags);
    Q_INVOKABLE dbus::AsyncCall* listInhibitors();
    Q_INVOKABLE void lockSessions();
    Q_INVOKABLE void powerOff(bool interactive = false);
    Q_INVOKABLE void powerOffWithFlags(manager::PowerActionFlags::Flags flags);
    Q_INVOKABLE void reboot(bool interactive = false);
    Q_INVOKABLE void rebootWithFlags(manager::PowerActionFlags::Flags flags);
    Q_INVOKABLE void scheduleShutdown(const QString& type, qulonglong usec);
    Q_INVOKABLE void setRebootParameter(const QString& parameter);
    Q_INVOKABLE void setRebootToBootLoaderEntry(const QString& boot_loader_entry);
    Q_INVOKABLE void setRebootToBootLoaderMenu(qulonglong timeout);
    Q_INVOKABLE void setRebootToFirmwareSetup(bool enable);
    Q_INVOKABLE void setUserLinger(uint uid, bool enable, bool interactive = false);
    Q_INVOKABLE void sleep(qulonglong flags);
    Q_INVOKABLE void suspend(bool interactive = false);
    Q_INVOKABLE void suspendThenHibernate(bool interactive = false);
    Q_INVOKABLE void suspendThenHibernateWithFlags(manager::PowerActionFlags::Flags flags);
    Q_INVOKABLE void suspendWithFlags(manager::PowerActionFlags::Flags flags);
    Q_INVOKABLE void unlockSessions();

  signals:
    void prepareForShutdown(bool start);
    void prepareForShutdownWithMetadata(bool start, const QVariantMap& metadata);
    void prepareForSleep(bool start);
    void secureAttentionKey(const QString& seat_id, Seat* seat);

    void currentSessionChanged();
    void currentUserChanged();
    void currentSeatChanged();
    void sessionsChanged();
    void usersChanged();
    void seatsChanged();
    void scheduledShutdownChanged();
    void preparingForShutdownWithMetadataChanged();
    void bootLoaderEntriesChanged();
    void killExcludeUsersChanged();
    void killOnlyUsersChanged();
    void sleepOperationChanged();
    void blockInhibitedChanged();
    void blockWeakInhibitedChanged();
    void delayInhibitedChanged();
    void designatedMaintenanceTimeChanged();
    void handleHibernateKeyChanged();
    void handleHibernateKeyLongPressChanged();
    void handleLidSwitchChanged();
    void handleLidSwitchDockedChanged();
    void handleLidSwitchExternalPowerChanged();
    void handlePowerKeyChanged();
    void handlePowerKeyLongPressChanged();
    void handleRebootKeyChanged();
    void handleRebootKeyLongPressChanged();
    void handleSecureAttentionKeyChanged();
    void handleSuspendKeyChanged();
    void handleSuspendKeyLongPressChanged();
    void idleActionChanged();
    void rebootParameterChanged();
    void rebootToBootLoaderEntryChanged();
    void wallMessageChanged();
    void holdoffTimeoutUSecChanged();
    void idleActionUSecChanged();
    void idleSinceHintChanged();
    void idleSinceHintMonotonicChanged();
    void inhibitDelayMaxUSecChanged();
    void inhibitorsMaxChanged();
    void rebootToBootLoaderMenuChanged();
    void runtimeDirectoryInodesMaxChanged();
    void runtimeDirectorySizeChanged();
    void sessionsMaxChanged();
    void stopIdleSessionUSecChanged();
    void userStopDelayUSecChanged();
    void nAutoVTsChanged();
    void dockedChanged();
    void enableWallMessagesChanged();
    void idleHintChanged();
    void killUserProcessesChanged();
    void lidClosedChanged();
    void onExternalPowerChanged();
    void preparingForShutdownChanged();
    void preparingForSleepChanged();
    void rebootToFirmwareSetupChanged();
    void removeIPCChanged();

  private slots:
    void addSession(const QString&, const QDBusObjectPath& object_path);
    void addUser(int, const QDBusObjectPath& object_path);
    void addSeat(const QString&, const QDBusObjectPath& object_path);
    void removeSession(const QString&, const QDBusObjectPath& object_path);
    void removeUser(int, const QDBusObjectPath& object_path);
    void removeSeat(const QString&, const QDBusObjectPath& object_path);
    void emitSecureAttentionKey(const QString& seat_id, const QDBusObjectPath& object_path);

  private:
    explicit Login1();
    Q_DISABLE_COPY(Login1)

    Login1ManagerInterface* m_iface = nullptr;

    QList<Session*> m_sessions;
    QList<User*> m_users;
    QList<Seat*> m_seats;

    static qsizetype sessionsCount(QQmlListProperty<Session>*);
    static qsizetype usersCount(QQmlListProperty<User>*);
    static qsizetype seatsCount(QQmlListProperty<Seat>*);
    static Session* session(QQmlListProperty<Session>*, qsizetype);
    static User* user(QQmlListProperty<User>*, qsizetype);
    static Seat* seat(QQmlListProperty<Seat>*, qsizetype);

    void initializeSessions();
    void initializeUsers();
    void initializeSeats();

    void initializeCurrentObjects();

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    Q_OBJECT_BINDABLE_PROPERTY(Login1, Session*, b_currentSession, &Login1::currentSessionChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Login1, User*, b_currentUser, &Login1::currentUserChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Login1, Seat*, b_currentSeat, &Login1::currentSeatChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Login1, SessionsMap, b_sessions, &Login1::sessionsChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Login1, UsersMap, b_users, &Login1::usersChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Login1, SeatsMap, b_seats, &Login1::seatsChanged)

    ES_DBUS_BINDABLE_PROPERTY(Login1, manager::ShutdownOperation, b_scheduledShutdown, scheduledShutdownChanged, "ScheduledShutdown", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QVariantMap, b_preparingForShutdownWithMetadata, preparingForShutdownWithMetadataChanged, "PreparingForShutdownWithMetadata", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QStringList, b_bootLoaderEntries, bootLoaderEntriesChanged, "BootLoaderEntries", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QStringList, b_killExcludeUsers, killExcludeUsersChanged, "KillExcludeUsers", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QStringList, b_killOnlyUsers, killOnlyUsersChanged, "KillOnlyUsers", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QStringList, b_sleepOperation, sleepOperationChanged, "SleepOperation", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_blockInhibited, blockInhibitedChanged, "BlockInhibited", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_blockWeakInhibited, blockWeakInhibitedChanged, "BlockWeakInhibited", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_delayInhibited, delayInhibitedChanged, "DelayInhibited", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_designatedMaintenanceTime, designatedMaintenanceTimeChanged, "DesignatedMaintenanceTime", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleHibernateKey, handleHibernateKeyChanged, "HandleHibernateKey", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleHibernateKeyLongPress, handleHibernateKeyLongPressChanged, "HandleHibernateKeyLongPress", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleLidSwitch, handleLidSwitchChanged, "HandleLidSwitch", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleLidSwitchDocked, handleLidSwitchDockedChanged, "HandleLidSwitchDocked", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleLidSwitchExternalPower, handleLidSwitchExternalPowerChanged, "HandleLidSwitchExternalPower", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handlePowerKey, handlePowerKeyChanged, "HandlePowerKey", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handlePowerKeyLongPress, handlePowerKeyLongPressChanged, "HandlePowerKeyLongPress", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleRebootKey, handleRebootKeyChanged, "HandleRebootKey", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleRebootKeyLongPress, handleRebootKeyLongPressChanged, "HandleRebootKeyLongPress", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleSecureAttentionKey, handleSecureAttentionKeyChanged, "HandleSecureAttentionKey", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleSuspendKey, handleSuspendKeyChanged, "HandleSuspendKey", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_handleSuspendKeyLongPress, handleSuspendKeyLongPressChanged, "HandleSuspendKeyLongPress", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_idleAction, idleActionChanged, "IdleAction", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_rebootParameter, rebootParameterChanged, "RebootParameter", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_rebootToBootLoaderEntry, rebootToBootLoaderEntryChanged, "RebootToBootLoaderEntry", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, QString, b_wallMessage, wallMessageChanged, "WallMessage", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_holdoffTimeoutUSec, holdoffTimeoutUSecChanged, "HoldoffTimeoutUSec", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_idleActionUSec, idleActionUSecChanged, "IdleActionUSec", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_idleSinceHint, idleSinceHintChanged, "IdleSinceHint", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_idleSinceHintMonotonic, idleSinceHintMonotonicChanged, "IdleSinceHintMonotonic", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_inhibitDelayMaxUSec, inhibitDelayMaxUSecChanged, "InhibitDelayMaxUSec", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_inhibitorsMax, inhibitorsMaxChanged, "InhibitorsMax", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_rebootToBootLoaderMenu, rebootToBootLoaderMenuChanged, "RebootToBootLoaderMenu", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_runtimeDirectoryInodesMax, runtimeDirectoryInodesMaxChanged, "RuntimeDirectoryInodesMax", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_runtimeDirectorySize, runtimeDirectorySizeChanged, "RuntimeDirectorySize", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_sessionsMax, sessionsMaxChanged, "SessionsMax", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_stopIdleSessionUSec, stopIdleSessionUSecChanged, "StopIdleSessionUSec", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, qulonglong, b_userStopDelayUSec, userStopDelayUSecChanged, "UserStopDelayUSec", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, uint, b_nAutoVTs, nAutoVTsChanged, "NAutoVTs", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_docked, dockedChanged, "Docked", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_enableWallMessages, enableWallMessagesChanged, "EnableWallMessages", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_idleHint, idleHintChanged, "IdleHint", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_killUserProcesses, killUserProcessesChanged, "KillUserProcesses", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_lidClosed, lidClosedChanged, "LidClosed", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_onExternalPower, onExternalPowerChanged, "OnExternalPower", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_preparingForShutdown, preparingForShutdownChanged, "PreparingForShutdown", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_preparingForSleep, preparingForSleepChanged, "PreparingForSleep", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_rebootToFirmwareSetup, rebootToFirmwareSetupChanged, "RebootToFirmwareSetup", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Login1, bool, b_removeIPC, removeIPCChanged, "RemoveIPC", propertiesGroup)
};

} // namespace extrashell::login1