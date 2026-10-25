#pragma once

#include <qlatin1stringview.h>
#include <qstring.h>
#include <qstringfwd.h>

namespace extrashell::udisks2 {
    
using namespace Qt::StringLiterals;

inline constexpr QLatin1StringView UDISKS2_SERVICE = "org.freedesktop.UDisks2"_L1;

inline constexpr QLatin1StringView UDISKS2_OBJECT = "/org/freedesktop/UDisks2"_L1;
inline constexpr QLatin1StringView UDISKS2_MANAGER_OBJECT = "/org/freedesktop/UDisks2/Manager"_L1;

inline constexpr QLatin1StringView UDISKS2_MANAGER_INTERFACE = "org.freedesktop.UDisks2.Manager"_L1;
inline constexpr QLatin1StringView UDISKS2_MANAGER_BTRFS_INTERFACE = "org.freedesktop.UDisks2.Manager.BTRFS"_L1;
inline constexpr QLatin1StringView UDISKS2_MANAGER_NVME_INTERFACE = "org.freedesktop.UDisks2.Manager.NVMe"_L1;
inline constexpr QLatin1StringView UDISKS2_BLOCK_INTERFACE = "org.freedesktop.UDisks2.Block"_L1;
inline constexpr QLatin1StringView UDISKS2_FILESYSTEM_INTERFACE = "org.freedesktop.UDisks2.Filesystem"_L1;
inline constexpr QLatin1StringView UDISKS2_FILESYSTEM_BTRFS_INTERFACE = "org.freedesktop.UDisks2.Filesystem.BTRFS"_L1;
inline constexpr QLatin1StringView UDISKS2_PARTITION_INTERFACE = "org.freedesktop.UDisks2.Partition"_L1;
inline constexpr QLatin1StringView UDISKS2_PARTITION_TABLE_INTERFACE = "org.freedesktop.UDisks2.PartitionTable"_L1;
inline constexpr QLatin1StringView UDISKS2_SWAPSPACE_INTERFACE = "org.freedesktop.UDisks2.Swapspace"_L1;
inline constexpr QLatin1StringView UDISKS2_DRIVE_INTERFACE = "org.freedesktop.UDisks2.Drive"_L1;
inline constexpr QLatin1StringView UDISKS2_DRIVE_ATA_INTERFACE = "org.freedesktop.UDisks2.Drive.Ata"_L1;

} // namespace extrashell::udisks2