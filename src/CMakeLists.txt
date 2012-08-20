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

# Define the MOC source files used by Tano
SET(Tano_Container_MOCs
    container/core/Channel.h
    container/core/Timer.h
    container/xmltv/XmltvChannel.h
    container/xmltv/XmltvCrewMember.h
    container/xmltv/XmltvList.h
    container/xmltv/XmltvProgramme.h
)

SET(Tano_Core_MOCs
    core/ChannelSelect.h
    core/NetworkDownload.h
    core/NetworkRequest.h
    core/ListItem.h
    core/ListModel.h
    core/Print.h
)

SET(Tano_Playlist_MOCs
    playlist/PlaylistFilterModel.h
    playlist/PlaylistModel.h
    playlist/PlaylistUpdate.h
)

SET(Tano_Recorder_MOCs
    recorder/RecorderCore.h
    recorder/RecorderTimeManager.h
    recorder/TimersFilterModel.h
    recorder/TimersModel.h
)

SET(Tano_UI_MOCs
    ui/MainWindow.h
    ui/core/BrowseWidget.h
    ui/core/InfoBarWidget.h
    ui/core/LicenseWidget.h
    ui/core/OsdFloat.h
    ui/core/OsdWidget.h
    ui/core/QtKeySequenceEdit.h
    ui/core/ScrollingWidget.h
    ui/core/TrayIcon.h
    ui/dialogs/AboutDialog.h
    ui/dialogs/DonationDialog.h
    ui/dialogs/PrintDialog.h
    ui/epg/EpgScheduleChannel.h
    ui/epg/EpgScheduleFull.h
    ui/epg/EpgShow.h
    ui/menu/MenuAspectRatio.h
    ui/menu/MenuCore.h
    ui/menu/MenuCropRatio.h
    ui/menu/MenuDeinterlacing.h
    ui/menu/MenuScale.h
    ui/menu/MenuTrackAudio.h
    ui/menu/MenuTrackSubtitles.h
    ui/menu/MenuTrackVideo.h
    ui/playlist/PlaylistDisplayWidget.h
    ui/playlist/PlaylistEditor.h
    ui/playlist/PlaylistEditorHelp.h
    ui/playlist/PlaylistEditorScan.h
    ui/playlist/PlaylistExportTvheadend.h
    ui/playlist/PlaylistFilterWidget.h
    ui/playlist/PlaylistImportCSV.h
    ui/recorder/Recorder.h
    ui/recorder/RecorderInfoWidget.h
    ui/recorder/RecorderNewDialog.h
    ui/recorder/TimersDisplayWidget.h
    ui/recorder/TimersEditorWidget.h
    ui/settings/SettingsBackend.h
    ui/settings/SettingsDialog.h
    ui/settings/SettingsGeneral.h
    ui/settings/SettingsPlaylist.h
    ui/settings/SettingsRecorder.h
    ui/settings/SettingsSchedule.h
    ui/settings/SettingsDialogShortcuts.h
    ui/settings/SettingsUi.h
)

SET(Tano_Xmltv_MOCs
    xmltv/XmltvChannelsFilterModel.h
    xmltv/XmltvChannelsModel.h
    xmltv/XmltvCrewFilterModel.h
    xmltv/XmltvCrewModel.h
    xmltv/XmltvManager.h
    xmltv/XmltvProgrammeFilterModel.h
    xmltv/XmltvProgrammeModel.h
    xmltv/XmltvSystem.h
)

SET(Tano_MOCs
    ${Tano_Container_MOCs}
    ${Tano_Core_MOCs}
    ${Tano_Playlist_MOCs}
    ${Tano_Recorder_MOCs}
    ${Tano_UI_MOCs}
    ${Tano_Xmltv_MOCs}
)

# Define the QRC source files used by Tano
SET(Tano_Resrcs
    ../info.qrc
    ../data/icons.qrc
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
    core/Backend.cpp
    core/ChannelSelect.cpp
    core/Common.cpp
    core/File.h
    core/Out.cpp
    core/NetworkDownload.cpp
    core/NetworkRequest.cpp
    core/ListItem.h
    core/ListModel.cpp
    core/LocaleManager.cpp
    core/Log.cpp
    core/Print.cpp
    core/Resources.cpp
    core/Settings.cpp
    core/Shortcuts.cpp
    core/Udpxy.cpp
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
    SET(Tano_MOCs
        ${Tano_MOCs}

        update/UpdateDialog.h
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
    )
ELSE(CUSTOM_SETTINGS)
    SET(Tano_Srcs
        ${Tano_Srcs}

        core/SettingsDefaults.cpp
    )
ENDIF(CUSTOM_SETTINGS)


###############
# Qt settings #
###############
# This scripts run the resource compiler rcc, the user interface compiler uic
# and the meta object compiler moc.
IF(WITH_QT5)
    QT5_ADD_RESOURCES(Tano_Resrcs_Srcs ${Tano_Resrcs})
    QT5_WRAP_UI(Tano_UIC_Srcs ${Tano_UIs})
    QT5_WRAP_CPP(Tano_MOC_Srcs ${Tano_MOCs})
ELSE(WITH_QT5)
    QT4_ADD_RESOURCES(Tano_Resrcs_Srcs ${Tano_Resrcs})
    QT4_WRAP_UI(Tano_UIC_Srcs ${Tano_UIs})
    QT4_WRAP_CPP(Tano_MOC_Srcs ${Tano_MOCs})
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
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib/qtsingleapplication/src
    ${CMAKE_BINARY_DIR}/src
)

# Include additional libraries
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

# Here the needed Libraries are added to the link command.
IF(WITH_QT5)
    TARGET_LINK_LIBRARIES(tano
                         ${QT_SINGLE_APPLICATION}
                         ${Qt5Concurrent_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Xml_LIBRARIES}
                         ${LIBVLCQT_LIBRARY}
        )
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
