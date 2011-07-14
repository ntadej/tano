#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

###################
# List of sources #
###################
# Define the UI source files used by Tano Editor
SET(Tano_Editor_UIs
    ../src/ui/dialogs/AboutDialog.ui
    ../src/ui/dialogs/PrintDialog.ui
    ../src/ui/dialogs/UpdateDialog.ui
    ../src/ui/playlist/PlaylistEdit.ui
    ../src/ui/playlist/PlaylistExportTvheadend.ui
    ../src/ui/playlist/PlaylistImportCSV.ui
    ../src/ui/playlist/PlaylistImportWeb.ui
    ../src/ui/playlist/PlaylistSelect.ui
    ../src/ui/settings/SettingsBackend.ui
    ../src/ui/settings/SettingsEdit.ui
    ../src/ui/settings/SettingsGeneral.ui
    ../src/ui/settings/SettingsPlaylist.ui
    ../src/ui/settings/SettingsRecorder.ui
    ../src/ui/settings/SettingsSchedule.ui
    ../src/ui/settings/SettingsShortcuts.ui
    ../src/ui/settings/SettingsUi.ui
)

# Define the MOC source files used by Tano Editor
SET(Tano_Editor_MOCs    # List of classes which Qt needs to produce MOCs for them (in order for events, signals, slots and other macros to work
    ../src/container/core/Channel.h
    ../src/core/GetFile.h
    ../src/core/ListItem.h
    ../src/core/ListModel.h
    ../src/core/Print.h
    ../src/core/UpdateManager.h
    ../src/playlist/PlaylistFilterModel.h
    ../src/playlist/PlaylistModel.h
    ../src/ui/core/BrowseWidget.h
    ../src/ui/core/LicenseWidget.h
    ../src/ui/core/QtKeySequenceEdit.h
    ../src/ui/dialogs/AboutDialog.h
    ../src/ui/dialogs/PrintDialog.h
    ../src/ui/dialogs/UpdateDialog.h
    ../src/ui/playlist/PlaylistDisplayWidget.h
    ../src/ui/playlist/PlaylistEdit.h
    ../src/ui/playlist/PlaylistExportTvheadend.h
    ../src/ui/playlist/PlaylistImportCSV.h
    ../src/ui/playlist/PlaylistImportWeb.h
    ../src/ui/playlist/PlaylistSelect.h
    ../src/ui/settings/SettingsBackend.h
    ../src/ui/settings/SettingsEdit.h
    ../src/ui/settings/SettingsGeneral.h
    ../src/ui/settings/SettingsPlaylist.h
    ../src/ui/settings/SettingsRecorder.h
    ../src/ui/settings/SettingsSchedule.h
    ../src/ui/settings/SettingsShortcuts.h
    ../src/ui/settings/SettingsUi.h
)

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
    ../src/container/core/File.cpp
    ../src/container/core/UpdateInfo.cpp
    ../src/container/playlist/CSVInfo.cpp
    ../src/container/playlist/PlaylistInfo.cpp

    ../src/core/Common.cpp
    ../src/core/ConsoleOutput.cpp
    ../src/core/Enums.cpp
    ../src/core/GetFile.cpp
    ../src/core/ListModel.cpp
    ../src/core/LocaleManager.cpp
    ../src/core/Print.cpp
    ../src/core/Settings.cpp
    ../src/core/Shortcuts.cpp
    ../src/core/Udpxy.cpp
    ../src/core/UpdateManager.cpp

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
    ../src/playlist/handlers/TanoHandlerOld.cpp

    ../src/ui/core/BrowseWidget.cpp
    ../src/ui/core/FileDialogs.cpp
    ../src/ui/core/LicenseWidget.cpp
    ../src/ui/core/QtKeySequenceEdit.cpp
    ../src/ui/dialogs/AboutDialog.cpp
    ../src/ui/dialogs/PrintDialog.cpp
    ../src/ui/dialogs/UpdateDialog.cpp
    ../src/ui/playlist/PlaylistDisplayWidget.cpp
    ../src/ui/playlist/PlaylistEdit.cpp
    ../src/ui/playlist/PlaylistExportTvheadend.cpp
    ../src/ui/playlist/PlaylistImportCSV.cpp
    ../src/ui/playlist/PlaylistImportWeb.cpp
    ../src/ui/playlist/PlaylistSelect.cpp
    ../src/ui/settings/SettingsBackend.cpp
    ../src/ui/settings/SettingsEdit.cpp
    ../src/ui/settings/SettingsGeneral.cpp
    ../src/ui/settings/SettingsPlaylist.cpp
    ../src/ui/settings/SettingsRecorder.cpp
    ../src/ui/settings/SettingsSchedule.cpp
    ../src/ui/settings/SettingsShortcuts.cpp
    ../src/ui/settings/SettingsUi.cpp

    ../src/xml/PlaylistListHandler.cpp
    ../src/xml/UpdateHandler.cpp
)

################
# Qt4 settings #
################
# This line adds the QtXML and QtNETWORK libraries to the Qt libraries
# loaded when linking Tano Editor
SET(QT_USE_QTXML True)
SET(QT_USE_QTNETWORK True)

# This scripts runs the resource compiler rcc.
QT4_ADD_RESOURCES(Tano_Editor_Resrcs_Srcs ${Tano_Editor_Resrcs})

# This scripts runs the user interface compiler uic.
QT4_WRAP_UI(Tano_Editor_UIC_Srcs ${Tano_Editor_UIs})

# This script runs the meta object compiler moc.
QT4_WRAP_CPP(Tano_Editor_MOC_Srcs ${Tano_Editor_MOCs})


#########################
# Compiler Define flags #
#########################
# Define we are compiling editor
ADD_DEFINITIONS(-DEDITOR=1)

# Define default data dir, if under POSIX systems.
IF(NOT MINGW)
    IF (NOT DEFAULT_DATA_DIR)
        ADD_DEFINITIONS(-DDEFAULT_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    ELSE (NOT DEFAULT_DATA_DIR)
        ADD_DEFINITIONS(-DDEFAULT_DATA_DIR="${DEFAULT_DATA_DIR}")
    ENDIF (NOT DEFAULT_DATA_DIR)
ENDIF(NOT MINGW)

# Define default plugin dir, if under POSIX systems.
IF(NOT MINGW)
    IF (NOT DEFAULT_PLUGIN_DIR)
        ADD_DEFINITIONS(-DDEFAULT_PLUGIN_DIR="${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR}")
    ELSE (NOT DEFAULT_PLUGIN_DIR)
        ADD_DEFINITIONS(-DDEFAULT_PLUGIN_DIR="${DEFAULT_PLUGIN_DIR}")
    ENDIF (NOT DEFAULT_PLUGIN_DIR)
ENDIF(NOT MINGW)


####################################
# Include paths and set up targets #
####################################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} "${CMAKE_BINARY_DIR}/src" "${CMAKE_SOURCE_DIR}/src")

# This line adds the include path of the Qt
# directory to the compiler include path
INCLUDE(${QT_USE_FILE})

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

# Here the needed Qt Libraries are added to the link command.
IF(EDITOR_WITH_VLCQT)
    TARGET_LINK_LIBRARIES(tano-editor ${QT_LIBRARIES} ${LIBVLCQT_LIBRARY})
ELSE(EDITOR_WITH_VLCQT)
    TARGET_LINK_LIBRARIES(tano-editor ${QT_LIBRARIES})
ENDIF(EDITOR_WITH_VLCQT)


################
# Installation #
################
# Install tano-editor exe
INSTALL(TARGETS tano-editor
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
)

