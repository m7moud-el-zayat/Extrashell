set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.login1.Manager.xml
    PROPERTIES
        CLASSNAME Login1ManagerInterface
        INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.login1.Manager.xml
    login1_manager_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.login1.Session.xml
    PROPERTIES
        CLASSNAME Login1SessionInterface
        INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.login1.Session.xml
    login1_session_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.login1.User.xml
    PROPERTIES
        CLASSNAME Login1UserInterface
        INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.login1.User.xml
    login1_user_interface
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.login1.Seat.xml
    PROPERTIES
        CLASSNAME Login1SeatInterface
        INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/types.h
)

qt6_add_dbus_interface(DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.login1.Seat.xml
    login1_seat_interface
)

qt6_install_qml_module(extrashell-login1 Extrashell.Login1
    VERSION 1.0

    SOURCES
        qml.cpp
        session.cpp
        user.cpp
        seat.cpp
        types.cpp
        ${DBUS_INTERFACES}

    DEPENDENCIES
        QtQml
)

target_include_directories(extrashell-login1 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(extrashell-login1 PRIVATE Qt6::Core Qt6::Qml Qt6::DBus extrashell-dbus)