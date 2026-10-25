#include <qdbusconnection.h>
#include <qdbusextratypes.h>
#include <qdbusmetatype.h>
#include <qjsengine.h>
#include <qlogging.h>

#include "block.h"
#include "dbus_interfaces.h"
#include "drive.h"
#include "drive_ata.h"
#include "filesystem.h"
#include "filesystem_btrfs.h"
#include "manager.h"
#include "manager_nvme.h"
#include "partition.h"
#include "partition_table.h"
#include "qml.h"
#include "swapspace.h"
#include "types.h"

#include <utility>

#include "../dbus/object_manager.h"
#include "../dbus/types.h"

Q_LOGGING_CATEGORY(logUDisks2, "extrashell.udisks2", QtWarningMsg)

namespace extrashell::udisks2 {

UDisks2::UDisks2() {
    qDBusRegisterMetaType<QByteArrayList>();  // Apparently, it's not registered
    qDBusRegisterMetaType<ActionAvailability>();
    qDBusRegisterMetaType<ResizeAvailability>();
    qDBusRegisterMetaType<BlockConfigurationItem>();
    qDBusRegisterMetaType<BlockConfiguration>();
    qDBusRegisterMetaType<BtrfsSubvolume>();
    qDBusRegisterMetaType<BtrfsSubvolumes>();
    qDBusRegisterMetaType<SMARTAttribute>();
    qDBusRegisterMetaType<SMARTAttributes>();

    QDBusConnection systemBus = QDBusConnection::systemBus();

    if (!systemBus.isConnected()) {
        qCWarning(logUDisks2).noquote()
            << "Failed to initialize UDisks2:"
            << "D-Bus connection" << systemBus.name() << "is not connected.";

        return;
    }

    m_objectManager = new dbus::ObjectManager(UDISKS2_SERVICE, UDISKS2_OBJECT, systemBus, this);

    if (!isObjectManagerValid()) {
        qCWarning(logUDisks2).noquote()
            << "Failed to connect InterfacesAdded/Removed signals for" << (UDISKS2_SERVICE + ':')
            << "D-Bus ObjectManager interface is invalid.";

        return;
    }

    connect(m_objectManager, &dbus::ObjectManager::interfacesAdded, this, &UDisks2::addObject);
    connect(m_objectManager, &dbus::ObjectManager::interfacesRemoved, this, &UDisks2::removeObject);

    m_objectManager->getManagedObjects();
}

UDisks2* UDisks2::instance() {
    static auto* instance = new UDisks2;

    return instance;
}

UDisks2* UDisks2::create(QQmlEngine*, QJSEngine*) {
    auto* udisks2 = instance();
    QJSEngine::setObjectOwnership(udisks2, QJSEngine::CppOwnership);

    return udisks2;
}

QQmlListProperty<Block> UDisks2::blocks() {
    return {
        this, nullptr,
        UDisks2::blocksCount,
        UDisks2::block
    };
}

QQmlListProperty<Drive> UDisks2::drives() {
    return {
        this, nullptr,
        UDisks2::drivesCount,
        UDisks2::drive
    };
}

void UDisks2::addObject(const QDBusObjectPath& objectPath, const DBusObjectManagerInterfaces& ifaces) {
    const QString path = objectPath.path();

    addManager(ifaces);
    addBlock(path, ifaces);
    addDrive(path, ifaces);
}

void UDisks2::removeObject(const QDBusObjectPath& objectPath, const QStringList& ifaces) {
    const QString path = objectPath.path();

    removeManager(ifaces);
    removeBlock(path, ifaces);
    removeDrive(path, ifaces);
}

qsizetype UDisks2::blocksCount(QQmlListProperty<Block>* list)
{
    return static_cast<UDisks2*>(list->object)->m_blocks.size();
}

qsizetype UDisks2::drivesCount(QQmlListProperty<Drive>* list)
{
    return static_cast<UDisks2*>(list->object)->m_drives.size();
}

Block* UDisks2::block(QQmlListProperty<Block>* list, qsizetype index)
{
    return static_cast<UDisks2*>(list->object)->m_blocks.at(index);
}

Drive* UDisks2::drive(QQmlListProperty<Drive>* list, qsizetype index)
{
    return static_cast<UDisks2*>(list->object)->m_drives.at(index);
}

void UDisks2::addManager(const DBusObjectManagerInterfaces& ifaces) {
    if (auto it = ifaces.constFind(UDISKS2_MANAGER_INTERFACE); it != ifaces.cend()) {
        if (!m_manager)
            m_manager = new Manager(*it, this);
        
        emit managerChanged();
    }

    if (!m_manager)
        return;

    if (auto it = ifaces.constFind(UDISKS2_MANAGER_NVME_INTERFACE); it != ifaces.cend())
        if (!m_manager->bindableNVMe().value())
            m_manager->setNVMe(new ManagerNVMe(*it, m_manager));
}

void UDisks2::addBlock(const QString& path, const DBusObjectManagerInterfaces& ifaces) {
    Block* block = b_blocks.value().value(path, nullptr);

    if (auto it = ifaces.constFind(UDISKS2_BLOCK_INTERFACE); it != ifaces.cend()) {
        if (!block) {
            block = new Block(path, *it, this);

            auto blocks = b_blocks.value();
            blocks.insert(path, block);
            m_blocks = blocks.values();
            b_blocks = std::move(blocks);
        }
    }

    if (!block)
        return;

    if (auto it = ifaces.constFind(UDISKS2_FILESYSTEM_INTERFACE); it != ifaces.cend()) {
        if (!block->bindableFilesystem().value()) {
            auto* filesystem = new Filesystem(path, *it, block);
            block->setFilesystem(filesystem);
        }
    }

    if (auto* filesystem = block->bindableFilesystem().value())
        if (auto it = ifaces.constFind(UDISKS2_FILESYSTEM_BTRFS_INTERFACE); it != ifaces.cend())
            if (!filesystem->bindableBtrfs().value())
                filesystem->setBtrfs( new FilesystemBtrfs(path, *it, filesystem));

    if (auto it = ifaces.constFind(UDISKS2_PARTITION_INTERFACE); it != ifaces.cend())
        if (!block->bindablePartition().value())
            block->setPartition(new Partition(path, *it, block));

    if (auto it = ifaces.constFind(UDISKS2_PARTITION_TABLE_INTERFACE); it != ifaces.cend())
        if (!block->bindablePartitionTable().value())
            block->setPartitionTable(new PartitionTable(path, *it, block));

    if (auto it = ifaces.constFind(UDISKS2_SWAPSPACE_INTERFACE); it != ifaces.cend())
        if (!block->bindableSwapspace().value())
            block->setSwapspace(new Swapspace(path, *it, block));
}

void UDisks2::addDrive(const QString& path, const DBusObjectManagerInterfaces& ifaces) {
    Drive* drive = b_drives.value().value(path, nullptr);

    if (auto it = ifaces.constFind(UDISKS2_DRIVE_INTERFACE); it != ifaces.cend()) {
        if (!drive) {
            drive = new Drive(path, *it, this);

            auto drives = b_drives.value();
            drives.insert(path, drive);
            m_drives = drives.values();
            b_drives = std::move(drives);
        }
    }

    if (!drive)
        return;

    if (auto it = ifaces.constFind(UDISKS2_DRIVE_ATA_INTERFACE); it != ifaces.cend())
        if (!drive->bindableAta().value())
            drive->setAta(new DriveAta(path, *it, drive));
}

void UDisks2::removeManager(const QStringList& ifaces) {
    if (!m_manager)
        return;

    if (ifaces.contains(UDISKS2_MANAGER_INTERFACE)) {
        // Manager owns ManagerNVMe, so removing Manager also removes NVMe.
        Manager* manager = m_manager;
        m_manager = nullptr;
        manager->deleteLater();

        return;
    }

    if (ifaces.contains(UDISKS2_MANAGER_NVME_INTERFACE)) {
        if (auto* nvme = m_manager->bindableNVMe().value()) {
            m_manager->setNVMe(nullptr);
            nvme->deleteLater();
        }
    }
}

void UDisks2::removeBlock(const QString& path, const QStringList& ifaces) {
    Block* block = b_blocks.value().value(path, nullptr);

    if (!block)
        return;

    // If the Block interface itself disappears, the entire QObject
    // hierarchy disappears with it. There is no need to manually
    // delete Filesystem/Partition/etc.
    if (ifaces.contains(UDISKS2_BLOCK_INTERFACE)) {
        auto blocks = b_blocks.value();

        if (Block* removedBlock = blocks.take(path)) {
            m_blocks = blocks.values();
            b_blocks = std::move(blocks);
            removedBlock->deleteLater();
        }

        return;
    }

    // From here on, only child interfaces are being removed.
    // The Block itself remains alive.
    if (ifaces.contains(UDISKS2_FILESYSTEM_INTERFACE)) {
        if (auto* filesystem = block->bindableFilesystem().value()) {
            // Removing Filesystem also removes its Btrfs child
            // through QObject parenting.
            block->setFilesystem(nullptr);
            filesystem->deleteLater();
        }
    } else if (
        ifaces.contains(UDISKS2_MANAGER_BTRFS_INTERFACE) ||
        ifaces.contains(UDISKS2_FILESYSTEM_BTRFS_INTERFACE)
    ) {
        // Only Btrfs disappeared; Filesystem remains.
        if (auto* filesystem = block->bindableFilesystem().value()) {
            if (auto* btrfs = filesystem->bindableBtrfs().value()) {
                filesystem->setBtrfs(nullptr);
                btrfs->deleteLater();
            }
        }
    }

    if (ifaces.contains(UDISKS2_PARTITION_INTERFACE)) {
        if (auto* partition = block->bindablePartition().value()) {
            block->setPartition(nullptr);
            partition->deleteLater();
        }
    }

    if (ifaces.contains(UDISKS2_PARTITION_TABLE_INTERFACE)) {
        if (auto* partitionTable = block->bindablePartitionTable().value()) {
            block->setPartitionTable(nullptr);
            partitionTable->deleteLater();
        }
    }

    if (ifaces.contains(UDISKS2_SWAPSPACE_INTERFACE)) {
        if (auto* swapspace = block->bindableSwapspace().value()) {
            block->setSwapspace(nullptr);
            swapspace->deleteLater();
        }
    }
}

void UDisks2::removeDrive(const QString& path, const QStringList& ifaces) {
    Drive* drive = b_drives.value().value(path, nullptr);

    if (!drive)
        return;

    // Removing Drive removes DriveAta as well because Drive owns it.
    if (ifaces.contains(UDISKS2_DRIVE_INTERFACE)) {
        auto drives = b_drives.value();

        if (Drive* removedDrive = drives.take(path)) {
            m_drives = drives.values();
            b_drives = std::move(drives);
            removedDrive->deleteLater();
        }

        return;
    }

    // Only ATA disappeared; Drive remains.
    if (ifaces.contains(UDISKS2_DRIVE_ATA_INTERFACE)) {
        if (auto* ata = drive->bindableAta().value()) {
            drive->setAta(nullptr);
            ata->deleteLater();
        }
    }
}

bool UDisks2::isObjectManagerValid() const { return m_objectManager && m_objectManager->isValid(); }

} // namespace extrashell::udisks2

#include "moc_qml.cpp"