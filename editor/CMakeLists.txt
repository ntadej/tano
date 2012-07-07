#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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
###############################################################################

PROJECT(tano-editor)
cmake_minimum_required(VERSION 2.6)

#################
# Configuration #
#################
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/src/Config.h.in"
    "${CMAKE_BINARY_DIR}/editor/Config.h"
)

###################
# List of sources #
###################
# Define the UI source files used by Tano Editor
SET(Tano_Editor_UIs
    ../src/ui/dialogs/AboutDialog.ui
    ../src/ui/dialogs/PrintDialog.ui
    ../src/ui/playlist/PlaylistDisplayWidget.ui
    ../src/ui/playlist/PlaylistEditor.ui
    ../src/ui/playlist/PlaylistEditorHelp.ui
    ../src/ui/playlist/PlaylistExportTvheadend.ui
    ../src/ui/playlist/PlaylistFilterWidget.ui
    ../src/ui/playlist/PlaylistImportCSV.ui
    ../src/ui/settings-editor/SettingsEditorCommon.ui
    ../src/ui/settings-editor/SettingsEditorDialog.ui
    ../src/ui/settings-editor/SettingsEditorGeneral.ui
)

# Define the MOC source files used by Tano Editor
SET(Tano_Editor_MOCs
    ../src/container/core/Channel.h
    ../src/core/ListItem.h
    ../src/core/ListModel.h
    ../src/core/NetworkRequest.h
    ../src/core/Print.h
    ../src/playlist/PlaylistFilterModel.h
    ../src/playlist/PlaylistModel.h
    ../src/ui/core/BrowseWidget.h
    ../src/ui/core/LicenseWidget.h
    ../src/ui/core/QtKeySequenceEdit.h
    ../src/ui/dialogs/AboutDialog.h
    ../src/ui/dialogs/PrintDialog.h
    ../src/ui/playlist/PlaylistDisplayWidget.h
    ../src/ui/playlist/PlaylistEditor.h
    ../src/ui/playlist/PlaylistEditorHelp.h
    ../src/ui/playlist/PlaylistExportTvheadend.h
    ../src/ui/playlist/PlaylistFilterWidget.h
    ../src/ui/playlist/PlaylistImportCSV.h
    ../src/ui/settings-editor/SettingsEditorCommon.h
    ../src/ui/settings-editor/SettingsEditorDialog.h
    ../src/ui/settings-editor/SettingsEditorGeneral.h
)

# Define the QRC source files used by Tano Editor
SET(Tano_Editor_Resrcs
    ../info.qrc
    ../data/icons.qrc
    ../data/images.qrc
    ../data/logo.qrc
)

# Define the C++ source files used by Tano
SET(Tano_Editor_Srcs
    main.cpp

    ../src/container/core/Channel.cpp

    ../src/core/Common.cpp
    ../src/core/Enums.cpp
    ../src/core/ListModel.cpp
    ../src/core/LocaleManager.cpp
    ../src/core/Log.cpp
    ../src/core/NetworkRequest.cpp
    ../src/core/Print.cpp
    ../src/core/Settings.cpp
    ../src/core/Udpxy.cpp

    ../src/playlist/PlaylistFilterModel.cpp
    ../src/playlist/PlaylistModel.cpp
    ../src/playlist/PlaylistOpen.cpp
    ../src/playlist/PlaylistSave.cpp
    ../src/playlist/generators/CSVGenerator.cpp
    ../src/playlist/generators/JsGenerator.cpp
    ../src/playlist/generators/M3UGenerator.cpp
    ../src/playlist/generators/TvheadendGenerator.cpp
    ../src/playlist/generators/XmltvIdGenerator.cpp
    ../src/playlist/handlers/CSVHandler.cpp
    ../src/playlist/handlers/JsHandler.cpp
    ../src/playlist/handlers/M3UHandler.cpp

    ../src/ui/core/BrowseWidget.cpp
    ../src/ui/core/FileDialogs.cpp
    ../src/ui/core/LicenseWidget.cpp
    ../src/ui/core/QtKeySequenceEdit.cpp
    ../src/ui/dialogs/AboutDialog.cpp
    ../src/ui/dialogs/PrintDialog.cpp
    ../src/ui/playlist/PlaylistDisplayWidget.cpp
    ../src/ui/playlist/PlaylistEditor.cpp
    ../src/ui/playlist/PlaylistEditorHelp.cpp
    ../src/ui/playlist/PlaylistExportTvheadend.cpp
    ../src/ui/playlist/PlaylistFilterWidget.cpp
    ../src/ui/playlist/PlaylistImportCSV.cpp
    ../src/ui/settings-editor/SettingsEditorCommon.cpp
    ../src/ui/settings-editor/SettingsEditorDialog.cpp
    ../src/ui/settings-editor/SettingsEditorGeneral.cpp
)

IF(EDITOR_VLCQT)
    SET(Tano_Editor_UIs
        ${Tano_Editor_UIs}

        ../src/ui/playlist/PlaylistEditorScan.ui
    )
    SET(Tano_Editor_MOCs
        ${Tano_Editor_MOCs}

        ../src/ui/playlist/PlaylistEditorScan.h
    )
    SET(Tano_Editor_Srcs
        ${Tano_Editor_Srcs}

        ../src/ui/playlist/PlaylistEditorScan.cpp
    )
ENDIF(EDITOR_VLCQT)

IF(UPDATE)
    SET(Tano_Editor_UIs
        ${Tano_Editor_UIs}

        ../src/update/UpdateDialog.ui
    )
    SET(Tano_Editor_MOCs
        ${Tano_Editor_MOCs}

        ../src/update/UpdateDialog.h
    )
    SET(Tano_Editor_Srcs
        ${Tano_Editor_Srcs}

        ../src/update/UpdateDialog.cpp
        ../src/update/UpdateHandler.cpp
    )
ENDIF(UPDATE)

IF(CUSTOM_SETTINGS)
    SET(Tano_Editor_Srcs
        ${Tano_Editor_Srcs}

        ../src/custom/SettingsDefaults.cpp
    )
ELSE(CUSTOM_SETTINGS)
    SET(Tano_Editor_Srcs
        ${Tano_Editor_Srcs}

        ../src/core/SettingsDefaults.cpp
    )
ENDIF(CUSTOM_SETTINGS)


################
# Qt4 settings #
################
# This scripts run the resource compiler rcc, the user interface compiler uic
# and the meta object compiler moc.
IF(WITH_QT5)
    QT5_ADD_RESOURCES(Tano_Editor_Resrcs_Srcs ${Tano_Editor_Resrcs})
    QT5_WRAP_UI(Tano_Editor_UIC_Srcs ${Tano_Editor_UIs})
    QT5_WRAP_CPP(Tano_Editor_MOC_Srcs ${Tano_Editor_MOCs})
ELSE(WITH_QT5)
    QT4_ADD_RESOURCES(Tano_Editor_Resrcs_Srcs ${Tano_Editor_Resrcs})
    QT4_WRAP_UI(Tano_Editor_UIC_Srcs ${Tano_Editor_UIs})
    QT4_WRAP_CPP(Tano_Editor_MOC_Srcs ${Tano_Editor_MOCs})
ENDIF(WITH_QT5)

# This line adds the include path of the Qt
# directory to the compiler include path
IF(WITH_QT5)
    INCLUDE_DIRECTORIES(
        ${Qt5Concurrent_INCLUDE_DIRS}
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS}
        ${Qt5PrintSupport_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Xml_INCLUDE_DIRS}
    )
ELSE(WITH_QT5)
    # This line adds the QtNETWORK and QtXML libraries to the Qt4 libraries
    SET(QT_USE_QTNETWORK True)
    SET(QT_USE_QTXML True)

    INCLUDE(${QT_USE_FILE})
ENDIF(WITH_QT5)


#####################
# Compiler settings #
#####################
# Define we are compiling editor
ADD_DEFINITIONS(-DEDITOR)

# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/editor
)

# Include additional libraries
IF(EDITOR_WITH_VLCQT)
    INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})
ENDIF(EDITOR_WITH_VLCQT)

IF(MINGW)
    # CMake currently doesn't support windres yet. Add command manually
    FIND_PROGRAM(
        WINDRES_PROGRAM windres.exe PATHS
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/bin"
        c:/MinGW/bin /MinGW/bin
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/editor/windows.obj
        COMMAND ${WINDRES_PROGRAM}
        -i ${CMAKE_SOURCE_DIR}/editor/windows.rc
        -o ${CMAKE_BINARY_DIR}/editor/windows.obj
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/editor"
    )
    SET(Tano_Editor_Srcs ${Tano_Editor_Srcs} ${CMAKE_BINARY_DIR}/editor/windows.obj)
ENDIF(MINGW)

# This line tells cmake to create the Tano Editor program.
ADD_EXECUTABLE(tano-editor ${Tano_Editor_UIC_Srcs} ${Tano_Editor_Srcs} ${Tano_Editor_MOC_Srcs} ${Tano_Editor_Resrcs_Srcs})

# Here the needed Libraries are added to the link command.
IF(WITH_QT5)
    IF(EDITOR_WITH_VLCQT)
        TARGET_LINK_LIBRARIES(tano-editor
                             ${Qt5Concurrent_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Xml_LIBRARIES}
                             ${LIBVLCQT_LIBRARY}
        )
     ELSE(EDITOR_WITH_VLCQT)
        TARGET_LINK_LIBRARIES(tano-editor
                             ${Qt5Concurrent_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Xml_LIBRARIES}
        )
     ENDIF(EDITOR_WITH_VLCQT)
ELSE(WITH_QT5)
    IF(EDITOR_WITH_VLCQT)
        TARGET_LINK_LIBRARIES(tano-editor
                             ${QT_LIBRARIES}
                             ${LIBVLCQT_LIBRARY}
        )
    ELSE(EDITOR_WITH_VLCQT)
        TARGET_LINK_LIBRARIES(tano-editor
                             ${QT_LIBRARIES}
        )
    ENDIF(EDITOR_WITH_VLCQT)
ENDIF(WITH_QT5)


################
# Installation #
################
# Install tano-editor exe
INSTALL(TARGETS tano-editor
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
)
