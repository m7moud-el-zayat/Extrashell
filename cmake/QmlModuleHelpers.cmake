find_package(Qt6 REQUIRED COMPONENTS BuildInternals)

function(qt6_install_qml_module ARG_TARGET MODULE_URI)
    string(REPLACE "." "/" MODULE_PATH "${MODULE_URI}")

    qt6_add_qml_module(${ARG_TARGET}
        URI ${MODULE_URI}
        PLUGIN_TARGET ${ARG_TARGET}
        OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${MODULE_PATH}
        ${ARGN}
    )

    qt6_query_qml_module(${ARG_TARGET}
        QMLDIR QMLDIR_FILE
        TYPEINFO QMLTYPES_FILE
    )

    install(TARGETS ${ARG_TARGET}
        LIBRARY DESTINATION ${INSTALL_QMLDIR}/${MODULE_PATH}
    )

    install(FILES
        ${QMLDIR_FILE}
        ${QMLTYPES_FILE}
        DESTINATION ${INSTALL_QMLDIR}/${MODULE_PATH}
    )
endfunction()