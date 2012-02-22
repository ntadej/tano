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
#############################################################################

#################
# Configuration #
#################
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/src/Config.h.in"
    "${CMAKE_BINARY_DIR}/src/Config.h"
)


###################
# List of sources #
###################
# Define the UI source files used by Tano
SET(Tano_UIs
    ui/MainWindow.ui
    ui/dialogs/AboutDialog.ui
    ui/dialogs/DonationDialog.ui
    ui/dialogs/PrintDialog.ui
    ui/epg/EpgShow.ui
    ui/epg/EpgScheduleChannel.ui
    ui/epg/EpgScheduleFull.ui
    ui/playlist/PlaylistDisplayWidget.ui
    ui/playlist/PlaylistEditor.ui
    ui/playlist/PlaylistEditorHelp.ui
    ui/playlist/PlaylistExportTvheadend.ui
    ui/playlist/PlaylistImportCSV.ui
    ui/recorder/Recorder.ui
    ui/recorder/TimersDisplayWidget.ui
    ui/recorder/TimersEdit.ui
    ui/settings/SettingsBackend.ui
    ui/settings/SettingsEdit.ui
    ui/settings/SettingsGeneral.ui
    ui/settings/SettingsPlaylist.ui
    ui/settings/SettingsRecorder.ui
    ui/settings/SettingsSchedule.ui
    ui/settings/SettingsShortcuts.ui
    ui/settings/SettingsUi.ui
    ui/wizard/ConclusionPage.ui
    ui/wizard/IntroPage.ui
    ui/wizard/SettingsPage.ui
)

# Define the MOC source files used by Tano
SET(Tano_MOCs    # List of classes which Qt needs to produce MOCs for them (in order for events, signals, slots and other macros to work
    container/core/Channel.h
    container/core/Timer.h
    container/xmltv/XmltvChannel.h
    container/xmltv/XmltvCrewMember.h
    container/xmltv/XmltvList.h
    container/xmltv/XmltvProgramme.h
    core/ChannelSelect.h
    core/GetFile.h
    core/ListItem.h
    core/ListModel.h
    core/Print.h
    playlist/PlaylistFilterModel.h
    playlist/PlaylistModel.h
    recorder/RecorderCore.h
    recorder/RecorderTimeManager.h
    recorder/TimersFilterModel.h
    recorder/TimersModel.h
    ui/MainWindow.h
    ui/core/BrowseWidget.h
    ui/core/CurrentTimeWidget.h
    ui/core/InfoBarWidget.h
    ui/core/LicenseWidget.h
    ui/core/QtKeySequenceEdit.h
    ui/core/TeletextWidget.h
    ui/core/TrayIcon.h
    ui/dialogs/AboutDialog.h
    ui/dialogs/DonationDialog.h
    ui/dialogs/PrintDialog.h
    ui/epg/EpgList.h
    ui/epg/EpgScheduleChannel.h
    ui/epg/EpgScheduleFull.h
    ui/epg/EpgShow.h
    ui/menu/MenuAspectRatio.h
    ui/menu/MenuCore.h
    ui/menu/MenuCropRatio.h
    ui/menu/MenuDeinterlacing.h
    ui/menu/MenuTrackAudio.h
    ui/menu/MenuTrackSubtitles.h
    ui/menu/MenuTrackVideo.h
    ui/playlist/PlaylistDisplayWidget.h
    ui/playlist/PlaylistEditor.h
    ui/playlist/PlaylistEditorHelp.h
    ui/playlist/PlaylistExportTvheadend.h
    ui/playlist/PlaylistImportCSV.h
    ui/recorder/Recorder.h
    ui/recorder/TimersDisplayWidget.h
    ui/recorder/TimersEdit.h
    ui/settings/SettingsBackend.h
    ui/settings/SettingsEdit.h
    ui/settings/SettingsGeneral.h
    ui/settings/SettingsPlaylist.h
    ui/settings/SettingsRecorder.h
    ui/settings/SettingsSchedule.h
    ui/settings/SettingsShortcuts.h
    ui/settings/SettingsUi.h
    ui/wizard/ConclusionPage.h
    ui/wizard/FirstRunWizard.h
    ui/wizard/IntroPage.h
    ui/wizard/SettingsPage.h
    xmltv/XmltvChannelsFilterModel.h
    xmltv/XmltvChannelsModel.h
    xmltv/XmltvCrewFilterModel.h
    xmltv/XmltvCrewModel.h
    xmltv/XmltvManager.h
    xmltv/XmltvProgrammeFilterModel.h
    xmltv/XmltvProgrammeModel.h
    xmltv/XmltvSystem.h
)

SET(Tano_Resrcs
    ../info.qrc
    ../data/icons.qrc
    ../data/images.qrc
    ../data/logo.qrc
)

# Define the C++ source files used by Tano
SET(Tano_Container_Srcs
    container/core/Channel.cpp
    container/core/Timer.cpp
    container/xmltv/XmltvChannel.cpp
    container/xmltv/XmltvCrewMember.cpp
    container/xmltv/XmltvList.cpp
    container/xmltv/XmltvProgramme.cpp
)
SET(Tano_Core_Srcs
    core/Arguments.cpp
    core/ChannelSelect.cpp
    core/Common.cpp
    core/Enums.cpp
    core/GetFile.cpp
    core/ListItem.h
    core/ListModel.cpp
    core/LocaleManager.cpp
    core/Print.cpp
    core/Settings.cpp
    core/SettingsDefaults.cpp
    core/Shortcuts.cpp
    core/Udpxy.cpp
)
SET(Tano_Playlist_Srcs
    playlist/PlaylistFilterModel.cpp
    playlist/PlaylistModel.cpp
    playlist/PlaylistOpen.cpp
    playlist/PlaylistSave.cpp
    playlist/generators/CSVGenerator.cpp
    playlist/generators/JsGenerator.cpp
    playlist/generators/M3UGenerator.cpp
    playlist/generators/TvheadendGenerator.cpp
    playlist/generators/XmltvIdGenerator.cpp
    playlist/handlers/CSVHandler.cpp
    playlist/handlers/JsHandler.cpp
    playlist/handlers/M3UHandler.cpp
    playlist/handlers/TanoHandlerOld.cpp
)
SET(Tano_Recorder_Srcs
    recorder/RecorderCore.cpp
    recorder/RecorderTimeManager.cpp
    recorder/TimersGenerator.cpp
    recorder/TimersHandler.cpp
    recorder/TimersFilterModel.cpp
    recorder/TimersModel.cpp
)
SET(Tano_UI_Srcs
    ui/MainWindow.cpp
    ui/core/BrowseWidget.cpp
    ui/core/CurrentTimeWidget.cpp
    ui/core/FileDialogs.cpp
    ui/core/InfoBarWidget.cpp
    ui/core/LicenseWidget.cpp
    ui/core/QtKeySequenceEdit.cpp
    ui/core/TeletextWidget.cpp
    ui/core/TrayIcon.cpp
    ui/dialogs/AboutDialog.cpp
    ui/dialogs/DonationDialog.cpp
    ui/dialogs/PrintDialog.cpp
    ui/epg/EpgList.cpp
    ui/epg/EpgScheduleChannel.cpp
    ui/epg/EpgScheduleFull.cpp
    ui/epg/EpgShow.cpp
    ui/menu/MenuAspectRatio.cpp
    ui/menu/MenuCore.cpp
    ui/menu/MenuCropRatio.cpp
    ui/menu/MenuDeinterlacing.cpp
    ui/menu/MenuTrackAudio.cpp
    ui/menu/MenuTrackSubtitles.cpp
    ui/menu/MenuTrackVideo.cpp
    ui/playlist/PlaylistDisplayWidget.cpp
    ui/playlist/PlaylistEditor.cpp
    ui/playlist/PlaylistEditorHelp.cpp
    ui/playlist/PlaylistExportTvheadend.cpp
    ui/playlist/PlaylistImportCSV.cpp
    ui/recorder/Recorder.cpp
    ui/recorder/TimersDisplayWidget.cpp
    ui/recorder/TimersEdit.cpp
    ui/settings/SettingsBackend.cpp
    ui/settings/SettingsEdit.cpp
    ui/settings/SettingsGeneral.cpp
    ui/settings/SettingsPlaylist.cpp
    ui/settings/SettingsRecorder.cpp
    ui/settings/SettingsSchedule.cpp
    ui/settings/SettingsShortcuts.cpp
    ui/settings/SettingsUi.cpp
    ui/wizard/ConclusionPage.cpp
    ui/wizard/FirstRunWizard.cpp
    ui/wizard/IntroPage.cpp
    ui/wizard/SettingsPage.cpp
)
SET(Tano_Xmltv_Srcs
    xmltv/XmltvChannelsFilterModel.cpp
    xmltv/XmltvChannelsModel.cpp
    xmltv/XmltvCommon.cpp
    xmltv/XmltvCrewFilterModel.cpp
    xmltv/XmltvCrewModel.cpp
    xmltv/XmltvEnums.cpp
    xmltv/XmltvHandler.cpp
    xmltv/XmltvManager.cpp
    xmltv/XmltvProgrammeFilterModel.cpp
    xmltv/XmltvProgrammeModel.cpp
    xmltv/XmltvSystem.cpp
)

SET(Tano_Srcs
    main.cpp

    ${Tano_Container_Srcs}
    ${Tano_Core_Srcs}
    ${Tano_Playlist_Srcs}
    ${Tano_Recorder_Srcs}
    ${Tano_UI_Srcs}
    ${Tano_Xmltv_Srcs}
)

IF(UPDATE)
    SET(Tano_UIs
        ${Tano_UIs}

        ui/dialogs/UpdateDialog.ui
    )
    SET(Tano_MOCs
        ${Tano_MOCs}

        core/UpdateManager.h
        ui/dialogs/UpdateDialog.h
    )
    SET(Tano_Srcs
        ${Tano_Srcs}

        core/UpdateHandler.cpp
        core/UpdateManager.cpp
        ui/dialogs/UpdateDialog.cpp
    )
ENDIF(UPDATE)

################
# Qt4 settings #
################
# This line adds the QtXML and QtNETWORK libraries to the Qt libraries
SET(QT_USE_QTXML True)
SET(QT_USE_QTNETWORK True)

# This scripts runs the resource compiler rcc.
QT4_ADD_RESOURCES(Tano_Resrcs_Srcs ${Tano_Resrcs})

# This scripts runs the user interface compiler uic.
QT4_WRAP_UI(Tano_UIC_Srcs ${Tano_UIs})

# This script runs the meta object compiler moc.
QT4_WRAP_CPP(Tano_MOC_Srcs ${Tano_MOCs})


#########################
# Compiler Define flags #
#########################
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
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} "${CMAKE_BINARY_DIR}/src" "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/lib/qtsingleapplication/src")

# This line adds the include path of the Qt
# directory to the compiler include path
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})

IF(MINGW)
    # CMake currently doesn't support windres yet. Add command manually
    FIND_PROGRAM(
        WINDRES_PROGRAM windres.exe PATHS
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/bin"
        c:/MinGW/bin /MinGW/bin
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/windows.obj
        COMMAND ${WINDRES_PROGRAM}
        -i ${CMAKE_SOURCE_DIR}/src/windows.rc
        -o ${CMAKE_BINARY_DIR}/src/windows.obj
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src"
    )
    SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_BINARY_DIR}/src/windows.obj)
ENDIF(MINGW)

# This line tells cmake to create the Tano program.
ADD_EXECUTABLE(tano ${Tano_UIC_Srcs} ${Tano_Srcs} ${Tano_MOC_Srcs} ${Tano_Resrcs_Srcs})
ADD_DEPENDENCIES(tano qtsingleapplication)

# Here the needed Qt Libraries are added to the link command.
TARGET_LINK_LIBRARIES(tano ${QT_SINGLE_APPLICATION} ${QT_LIBRARIES} ${LIBVLCQT_LIBRARY})


################
# Installation #
################
# Install tano exe
INSTALL(TARGETS tano
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
)
