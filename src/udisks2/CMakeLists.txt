set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Manager.xml
    PROPERTIES
        CLASSNAME UDisks2ManagerInterface
        INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Manager.xml
    udisks2_manager_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Manager.NVMe.xml
    PROPERTIES
        CLASSNAME UDisks2ManagerNVMeInterface
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Manager.NVMe.xml
    udisks2_manager_nvme_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Block.xml
    PROPERTIES
        CLASSNAME UDisks2BlockInterface
        INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Block.xml
    udisks2_block_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Filesystem.xml
    PROPERTIES
        CLASSNAME UDisks2FilesystemInterface
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Filesystem.xml
    udisks2_filesystem_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Filesystem.BTRFS.xml
    PROPERTIES
        CLASSNAME UDisks2FilesystemBTRFSInterface
        INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Filesystem.BTRFS.xml
    udisks2_filesystem_btrfs_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Partition.xml
    PROPERTIES
        CLASSNAME UDisks2PartitionInterface
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Partition.xml
    udisks2_partition_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.PartitionTable.xml
    PROPERTIES
        CLASSNAME UDisks2PartitionTableInterface
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.PartitionTable.xml
    udisks2_partition_table_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Swapspace.xml
    PROPERTIES
        CLASSNAME UDisks2SwapspaceInterface
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Swapspace.xml
    udisks2_swapspace_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Drive.xml
    PROPERTIES
        CLASSNAME UDisks2DriveInterface
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Drive.xml
    udisks2_drive_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Drive.Ata.xml
    PROPERTIES
        CLASSNAME UDisks2DriveAtaInterface
        INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.UDisks2.Drive.Ata.xml
    udisks2_drive_ata_interface
)

qt6_install_qml_module(extrashell-udisks2 Extrashell.UDisks2
    VERSION 1.0

    SOURCES
        qml.cpp
        manager.cpp
        manager_nvme.cpp
        block.cpp
        filesystem.cpp
        filesystem_btrfs.cpp
        partition.cpp
        partition_table.cpp
        swapspace.cpp
        drive.cpp
        drive_ata.cpp
        types.cpp
        ${DBUS_INTERFACES}

    DEPENDENCIES
        QtQml
)

target_include_directories(extrashell-udisks2 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(extrashell-udisks2 PRIVATE Qt6::Core Qt6::Qml Qt6::DBus extrashell-dbus)