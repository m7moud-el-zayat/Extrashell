set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.ObjectManager.xml
    PROPERTIES
        CLASSNAME DBusObjectManagerInterface
        INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.ObjectManager.xml
    object_manager_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.Properties.xml
    PROPERTIES
        CLASSNAME DBusPropertiesInterface
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.Properties.xml
    properties_interface
)

qt6_add_library(extrashell-dbus STATIC
    object_manager.cpp
    properties.cpp
    utils.cpp
    ${DBUS_INTERFACES}
)

target_include_directories(extrashell-dbus PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(extrashell-dbus PRIVATE Qt6::Core Qt6::Qml Qt6::DBus)