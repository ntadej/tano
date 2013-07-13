#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2013 Tadej Novak <tadej@tano.si>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>./
#############################################################################

PROJECT(${TANO_EDITOR})

IF(MINGW OR MSVC)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/src/editor/tano-editor.rc.in
        ${CMAKE_BINARY_DIR}/src/editor/tano-editor.rc
    )
ENDIF(MINGW OR MSVC)

# Define the UI source files used by Tano Widgets
SET(Tano_Editor_UIs
    ui/PlaylistEditor.ui
    ui/PlaylistEditorHelp.ui
    ui/PlaylistEditorScan.ui
    ui/PlaylistExportTvheadend.ui
    ui/PlaylistImportCSV.ui
    ui/PrintDialog.ui
)

SET(Tano_Editor_Srcs
    main.cpp

    ui/PlaylistEditor.cpp
    ui/PlaylistEditorHelp.cpp
    ui/PlaylistEditorScan.cpp
    ui/PlaylistExportTvheadend.cpp
    ui/PlaylistImportCSV.cpp
    ui/Print.cpp
    ui/PrintDialog.cpp
)


###############
# Qt settings #
###############
# This script runs the user interface compiler uic
IF(QT5)
    QT5_WRAP_UI(Tano_Editor_UIC_Srcs ${Tano_Editor_UIs})
ELSE(QT5)
    QT4_WRAP_UI(Tano_Editor_UIC_Srcs ${Tano_Editor_UIs})
ENDIF(QT5)


#####################
# Compiler settings #
#####################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/editor
    ${CMAKE_SOURCE_DIR}/src/widgets
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_BINARY_DIR}/src/editor
)

# Include additional libraries
INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})

# Compile resources with windres
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/editor/tano-editor.obj
        COMMAND ${CMAKE_RC_COMPILER}
        -i ${CMAKE_BINARY_DIR}/src/editor/tano-editor.rc
        -o ${CMAKE_BINARY_DIR}/src/editor/tano-editor.obj
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/editor"
    )
    SET(Tano_Editor_Srcs ${Tano_Editor_Srcs} ${CMAKE_BINARY_DIR}/src/editor/tano-editor.obj)
ENDIF(MINGW)

IF(MSVC)
    SET(Tano_Editor_Srcs ${Tano_Editor_Srcs} ${CMAKE_BINARY_DIR}/src/editor/tano-editor.rc)
ENDIF(MSVC)

# This line tells cmake to create the Tano program.
IF(MINGW OR MSVC)
    ADD_EXECUTABLE(${TANO_EDITOR} WIN32 ${Tano_Editor_Srcs} ${Tano_Editor_UIC_Srcs})
ELSE(MINGW OR MSVC)
    ADD_EXECUTABLE(${TANO_EDITOR} ${Tano_Editor_Srcs} ${Tano_Editor_UIC_Srcs})
ENDIF(MINGW OR MSVC)
ADD_DEPENDENCIES(${TANO_EDITOR} ${TANO_CORE} ${TANO_WIDGETS})

# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET_TARGET_PROPERTIES(${TANO_EDITOR} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ".")
    SET_TARGET_PROPERTIES(${TANO_EDITOR} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET_TARGET_PROPERTIES(${TANO_EDITOR} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET_TARGET_PROPERTIES(${TANO_EDITOR} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET_TARGET_PROPERTIES(${TANO_EDITOR} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET_TARGET_PROPERTIES(${TANO_EDITOR} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF(MSVC)

# Here the needed Libraries are added to the link command.
IF(STATIC)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})
ENDIF(STATIC)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBVLCQT_LIBRARY} ${LIBVLCQT_WIDGETS_LIBRARY})

IF(QT5)
    QT5_USE_MODULES(${TANO_EDITOR} Network PrintSupport Sql Widgets Xml)
    TARGET_LINK_LIBRARIES(${TANO_EDITOR} ${TANO_CORE_LIB} ${TANO_WIDGETS_LIB})
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        QT5_USE_MODULES(${PROJECT_NAME} MacExtras)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${COREFOUNDATION_LIBRARY} ${IOKIT_LIBRARY})
    ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
ELSE(QT5)
    TARGET_LINK_LIBRARIES(${TANO_EDITOR} ${TANO_CORE_LIB} ${TANO_WIDGETS_LIB} ${QT_LIBRARIES})
ENDIF(QT5)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT STATIC)
    ADD_CUSTOM_COMMAND(TARGET ${TANO_EDITOR} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change ${TANO_CORE_LIB} "@loader_path/lib/${LIBSTART}${TANO_CORE}.${LIBEND}" $<TARGET_FILE:${TANO_EDITOR}>)
    ADD_CUSTOM_COMMAND(TARGET ${TANO_EDITOR} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change ${TANO_WIDGETS_LIB} "@loader_path/lib/${LIBSTART}${TANO_WIDGETS}.${LIBEND}" $<TARGET_FILE:${TANO_EDITOR}>)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT STATIC)


################
# Installation #
################
# Install tano exe
INSTALL(TARGETS ${TANO_EDITOR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
    BUNDLE DESTINATION .
)
