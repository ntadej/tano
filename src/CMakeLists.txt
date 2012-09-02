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
    ${CMAKE_SOURCE_DIR}/src/Config.h.in
    ${CMAKE_BINARY_DIR}/src/Config.h
)


###################
# List of sources #
###################
# Define the UI source files used by Tano
SET(Tano_UIs
    ui/MainWindow.ui
    ui/core/OsdWidget.ui
    ui/dialogs/AboutDialog.ui
    ui/dialogs/DonationDialog.ui
    ui/dialogs/PrintDialog.ui
    ui/epg/EpgShow.ui
    ui/epg/EpgScheduleChannel.ui
    ui/epg/EpgScheduleFull.ui
    ui/playlist/PlaylistDisplayWidget.ui
    ui/playlist/PlaylistEditor.ui
    ui/playlist/PlaylistEditorHelp.ui
    ui/playlist/PlaylistEditorScan.ui
    ui/playlist/PlaylistExportTvheadend.ui
    ui/playlist/PlaylistFilterWidget.ui
    ui/playlist/PlaylistImportCSV.ui
    ui/recorder/Recorder.ui
    ui/recorder/RecorderInfoWidget.ui
    ui/recorder/RecorderNewDialog.ui
    ui/recorder/TimersDisplayWidget.ui
    ui/recorder/TimersEditorWidget.ui
    ui/settings/SettingsBackend.ui
    ui/settings/SettingsDialog.ui
    ui/settings/SettingsGeneral.ui
    ui/settings/SettingsPlaylist.ui
    ui/settings/SettingsRecorder.ui
    ui/settings/SettingsSchedule.ui
    ui/settings/SettingsDialogShortcuts.ui
    ui/settings/SettingsUi.ui
)

# Define the QRC source files used by Tano
SET(Tano_Resrcs
    ../info.qrc
    ../data/logo/logo.qrc
    ../data/misc/misc.qrc
)

# Define the C++ source files used by Tano
SET(Tano_Container_Srcs
    container/ListItem.h
    container/ListModel.cpp
    container/core/Channel.cpp
    container/core/Timer.cpp
    container/xmltv/XmltvChannel.cpp
    container/xmltv/XmltvCrewMember.cpp
    container/xmltv/XmltvList.cpp
    container/xmltv/XmltvProgramme.cpp
)
SET(Tano_Core_Srcs
    core/Arguments.cpp
    core/Backend.cpp
    core/ChannelSelect.cpp
    core/Common.cpp
    core/File.h
    core/Out.cpp
    core/LocaleManager.cpp
    core/Log.cpp
    core/Print.cpp
    core/Resources.cpp
    core/network/NetworkDownload.cpp
    core/network/NetworkRequest.cpp
    core/network/NetworkUdpxy.cpp
    core/settings/Settings.cpp
    core/settings/SettingsChannel.cpp
    core/settings/SettingsShortcuts.cpp
)
SET(Tano_Playlist_Srcs
    playlist/PlaylistFilterModel.cpp
    playlist/PlaylistModel.cpp
    playlist/PlaylistOpen.cpp
    playlist/PlaylistSave.cpp
    playlist/PlaylistUpdate.cpp
    playlist/generators/CSVGenerator.cpp
    playlist/generators/JsGenerator.cpp
    playlist/generators/M3UGenerator.cpp
    playlist/generators/TvheadendGenerator.cpp
    playlist/generators/XmltvIdGenerator.cpp
    playlist/handlers/CSVHandler.cpp
    playlist/handlers/JsHandler.cpp
    playlist/handlers/M3UHandler.cpp
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
    ui/core/FileDialogs.cpp
    ui/core/InfoBarWidget.cpp
    ui/core/LicenseWidget.cpp
    ui/core/OsdFloat.cpp
    ui/core/OsdWidget.cpp
    ui/core/QtKeySequenceEdit.cpp
    ui/core/ScrollingWidget.cpp
    ui/core/TrayIcon.cpp
    ui/dialogs/AboutDialog.cpp
    ui/dialogs/DonationDialog.cpp
    ui/dialogs/PrintDialog.cpp
    ui/epg/EpgScheduleChannel.cpp
    ui/epg/EpgScheduleFull.cpp
    ui/epg/EpgShow.cpp
    ui/menu/MenuAspectRatio.cpp
    ui/menu/MenuCore.cpp
    ui/menu/MenuCropRatio.cpp
    ui/menu/MenuDeinterlacing.cpp
    ui/menu/MenuScale.cpp
    ui/menu/MenuTrackAudio.cpp
    ui/menu/MenuTrackSubtitles.cpp
    ui/menu/MenuTrackVideo.cpp
    ui/playlist/PlaylistDisplayWidget.cpp
    ui/playlist/PlaylistEditor.cpp
    ui/playlist/PlaylistEditorHelp.cpp
    ui/playlist/PlaylistEditorScan.cpp
    ui/playlist/PlaylistExportTvheadend.cpp
    ui/playlist/PlaylistFilterWidget.cpp
    ui/playlist/PlaylistImportCSV.cpp
    ui/recorder/Recorder.cpp
    ui/recorder/RecorderInfoWidget.cpp
    ui/recorder/RecorderNewDialog.cpp
    ui/recorder/TimersDisplayWidget.cpp
    ui/recorder/TimersEditorWidget.cpp
    ui/settings/SettingsBackend.cpp
    ui/settings/SettingsDialog.cpp
    ui/settings/SettingsGeneral.cpp
    ui/settings/SettingsPlaylist.cpp
    ui/settings/SettingsRecorder.cpp
    ui/settings/SettingsSchedule.cpp
    ui/settings/SettingsDialogShortcuts.cpp
    ui/settings/SettingsUi.cpp
)
SET(Tano_Xmltv_Srcs
    xmltv/XmltvChannelsFilterModel.cpp
    xmltv/XmltvChannelsModel.cpp
    xmltv/XmltvCommon.cpp
    xmltv/XmltvCrewFilterModel.cpp
    xmltv/XmltvCrewModel.cpp
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

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(Tano_Srcs
        ${Tano_Srcs}

        platform/linux/Locations.cpp
    )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(MINGW)
    SET(Tano_Srcs
        ${Tano_Srcs}

        platform/windows/Console.cpp
    )
ENDIF(MINGW)

IF(UPDATE)
    SET(Tano_UIs
        ${Tano_UIs}

        update/UpdateDialog.ui
    )
    SET(Tano_Srcs
        ${Tano_Srcs}

        update/UpdateDialog.cpp
        update/UpdateHandler.cpp
    )
ENDIF(UPDATE)

IF(CUSTOM_SETTINGS)
    SET(Tano_Srcs
        ${Tano_Srcs}

        custom/SettingsDefaults.cpp
        custom/SettingsShortcutsDefaults.cpp
    )
ELSE(CUSTOM_SETTINGS)
    SET(Tano_Srcs
        ${Tano_Srcs}

        core/settings/SettingsDefaults.cpp
        core/settings/SettingsShortcutsDefaults.cpp
    )
ENDIF(CUSTOM_SETTINGS)


###############
# Qt settings #
###############
# This scripts run the resource compiler rcc and the user interface compiler uic
IF(WITH_QT5)
    QT5_ADD_RESOURCES(Tano_Resrcs_Srcs ${Tano_Resrcs})
    QT5_WRAP_UI(Tano_UIC_Srcs ${Tano_UIs})
ELSE(WITH_QT5)
    QT4_ADD_RESOURCES(Tano_Resrcs_Srcs ${Tano_Resrcs})
    QT4_WRAP_UI(Tano_UIC_Srcs ${Tano_UIs})
ENDIF(WITH_QT5)


#####################
# Compiler settings #
#####################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib/qtsingleapplication/src
    ${CMAKE_BINARY_DIR}/src
)

# Include additional libraries
INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})

# Compile resources with windres
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/windows.obj
        COMMAND ${CMAKE_RC_COMPILER}
        -i ${CMAKE_SOURCE_DIR}/src/windows.rc
        -o ${CMAKE_BINARY_DIR}/src/windows.obj
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src"
    )
    SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_BINARY_DIR}/src/windows.obj)
ENDIF(MINGW)

# This line tells cmake to create the Tano program.
ADD_EXECUTABLE(tano ${Tano_UIC_Srcs} ${Tano_Srcs} ${Tano_Resrcs_Srcs})
ADD_DEPENDENCIES(tano qtsingleapplication)

# Here the needed Libraries are added to the link command.
IF(WITH_QT5)
    QT5_USE_MODULES(tano Concurrent Network PrintSupport Widgets Xml)
    TARGET_LINK_LIBRARIES(tano ${QT_SINGLE_APPLICATION} ${LIBVLCQT_LIBRARY})
ELSE(WITH_QT5)
    TARGET_LINK_LIBRARIES(tano
                         ${QT_SINGLE_APPLICATION}
                         ${QT_LIBRARIES}
                         ${LIBVLCQT_LIBRARY}
    )
ENDIF(WITH_QT5)


################
# Installation #
################
# Install tano exe
INSTALL(TARGETS tano
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
)
