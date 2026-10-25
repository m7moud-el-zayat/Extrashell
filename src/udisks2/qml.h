#pragma once

#include <qcontainerfwd.h>
#include <qlist.h>
#include <qloggingcategory.h>
#include <qmap.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qqmllist.h>
#include <qstring.h>
#include <qtclasshelpermacros.h>
#include <qtmetamacros.h>

#include <qproperty.h>
#include <qtypes.h>

#include "../dbus/types.h"

Q_DECLARE_LOGGING_CATEGORY(logUDisks2)

class QDBusObjectPath;
class QQmlEngine;
class QJSEngine;

namespace extrashell::dbus {

class ObjectManager;

} // namespace extrashell::dbus

namespace extrashell::udisks2 {

class Manager;
class Block;
class Drive;

class UDisks2 : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(extrashell::udisks2::Manager* manager READ manager NOTIFY managerChanged)
    Q_PROPERTY(QQmlListProperty<extrashell::udisks2::Block> blocks READ blocks NOTIFY blocksChanged)
    Q_PROPERTY(QQmlListProperty<extrashell::udisks2::Drive> drives READ drives NOTIFY drivesChanged)

    using BlocksMap = QMap<QString, Block*>;
    using DrivesMap = QMap<QString, Drive*>;

  public:
    static UDisks2* instance();
    static UDisks2* create(QQmlEngine*, QJSEngine*);

    [[nodiscard]] Manager* manager() const { return m_manager; }
    [[nodiscard]] QBindable<BlocksMap> bindableBlocksMap() const { return &b_blocks; }
    [[nodiscard]] QBindable<DrivesMap> bindableDrivesMap() const { return &b_drives; }

    QQmlListProperty<Block> blocks();
    QQmlListProperty<Drive> drives();

  signals:
    void managerChanged();
    void blocksChanged();
    void drivesChanged();

  private slots:
    void addObject(const QDBusObjectPath& object_path, const DBusObjectManagerInterfaces& ifaces);
    void removeObject(const QDBusObjectPath& object_path, const QStringList& ifaces);

  private:
    explicit UDisks2();
    Q_DISABLE_COPY(UDisks2)

    dbus::ObjectManager* m_objectManager = nullptr;

    Manager* m_manager = nullptr;

    QList<Block*> m_blocks;
    QList<Drive*> m_drives;

    static qsizetype blocksCount(QQmlListProperty<Block>*);
    static qsizetype drivesCount(QQmlListProperty<Drive>*);
    static Block* block(QQmlListProperty<Block>*, qsizetype);
    static Drive* drive(QQmlListProperty<Drive>*, qsizetype);

    void addManager(const DBusObjectManagerInterfaces& ifaces);
    void addBlock(const QString& path, const DBusObjectManagerInterfaces& ifaces);
    void addDrive(const QString& path, const DBusObjectManagerInterfaces& ifaces);

    void removeManager(const QStringList& ifaces);
    void removeBlock(const QString& path, const QStringList& ifaces);
    void removeDrive(const QString& path, const QStringList& ifaces);

    bool isObjectManagerValid() const;

    Q_OBJECT_BINDABLE_PROPERTY(UDisks2, BlocksMap, b_blocks, &UDisks2::blocksChanged)
    Q_OBJECT_BINDABLE_PROPERTY(UDisks2, DrivesMap, b_drives, &UDisks2::drivesChanged)
};

} // namespace extrashell::udisks2