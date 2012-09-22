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

PROJECT(${TANO_WIDGETS})

# Add definitions for shared library
ADD_DEFINITIONS(-DQT_SHARED -D_REENTRANT)

###################
# List of sources #
###################
# Define the UI source files used by Tano Widgets
SET(Tano_Widgets_UIs
    MainWindow.ui
    common/OsdWidget.ui
    dialogs/AboutDialog.ui
    dialogs/DonationDialog.ui
    dialogs/PrintDialog.ui
    dialogs/UpdateDialog.ui
    epg/EpgShow.ui
    epg/EpgScheduleChannel.ui
    epg/EpgScheduleFull.ui
    playlist/PlaylistDisplayWidget.ui
    playlist/PlaylistEditor.ui
    playlist/PlaylistEditorHelp.ui
    playlist/PlaylistEditorScan.ui
    playlist/PlaylistExportTvheadend.ui
    playlist/PlaylistFilterWidget.ui
    playlist/PlaylistImportCSV.ui
    recorder/Recorder.ui
    recorder/RecorderInfoWidget.ui
    recorder/RecorderNewDialog.ui
    settings/SettingsBackend.ui
    settings/SettingsDialog.ui
    settings/SettingsGeneral.ui
    settings/SettingsPlaylist.ui
    settings/SettingsRecorder.ui
    settings/SettingsSchedule.ui
    settings/SettingsDialogShortcuts.ui
    settings/SettingsUi.ui
    timers/TimersDisplayWidget.ui
    timers/TimersEditorWidget.ui
)

# Define the C++ source files used by Tano Widgets
SET(Tano_Widgets_Srcs
    MainWindow.cpp
    common/Backend.cpp
    common/BrowseWidget.cpp
    common/ChannelSelect.cpp
    common/DesktopShortcuts.cpp
    common/FileDialogs.cpp
    common/InfoBarWidget.cpp
    common/LicenseWidget.cpp
    common/OsdFloat.cpp
    common/OsdWidget.cpp
    common/Print.cpp
    common/QtKeySequenceEdit.cpp
    common/ScrollingWidget.cpp
    common/TrayIcon.cpp
    dialogs/AboutDialog.cpp
    dialogs/DonationDialog.cpp
    dialogs/PrintDialog.cpp
    dialogs/UpdateDialog.cpp
    epg/EpgScheduleChannel.cpp
    epg/EpgScheduleFull.cpp
    epg/EpgShow.cpp
    menu/MenuAspectRatio.cpp
    menu/MenuCore.cpp
    menu/MenuCropRatio.cpp
    menu/MenuDeinterlacing.cpp
    menu/MenuScale.cpp
    menu/MenuTrackAudio.cpp
    menu/MenuTrackSubtitles.cpp
    menu/MenuTrackVideo.cpp
    playlist/PlaylistDisplayWidget.cpp
    playlist/PlaylistEditor.cpp
    playlist/PlaylistEditorHelp.cpp
    playlist/PlaylistEditorScan.cpp
    playlist/PlaylistExportTvheadend.cpp
    playlist/PlaylistFilterWidget.cpp
    playlist/PlaylistImportCSV.cpp
    recorder/Recorder.cpp
    recorder/RecorderCore.cpp
    recorder/RecorderInfoWidget.cpp
    recorder/RecorderNewDialog.cpp
    settings/SettingsBackend.cpp
    settings/SettingsDialog.cpp
    settings/SettingsGeneral.cpp
    settings/SettingsPlaylist.cpp
    settings/SettingsRecorder.cpp
    settings/SettingsSchedule.cpp
    settings/SettingsDialogShortcuts.cpp
    settings/SettingsUi.cpp
    timers/TimersDisplayWidget.cpp
    timers/TimersEditorWidget.cpp
)


###############
# Qt settings #
###############
# This script runs the user interface compiler uic
IF(WITH_QT5)
    QT5_WRAP_UI(Tano_Widgets_UIC_Srcs ${Tano_Widgets_UIs})
ELSE(WITH_QT5)
    QT4_WRAP_UI(Tano_Widgets_UIC_Srcs ${Tano_Widgets_UIs})
ENDIF(WITH_QT5)


#####################
# Compiler settings #
#####################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/widgets
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_BINARY_DIR}/src/widgets
)

# Include additional libraries
INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})

# This line tells cmake to create the library.
ADD_LIBRARY(${TANO_WIDGETS} SHARED ${Tano_Widgets_Srcs} ${Tano_Widgets_UIC_Srcs})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES VERSION ${TANO_VERSION_MAJOR}
                                          SOVERSION "${TANO_VERSION_MAJOR}.${TANO_VERSION_MINOR}"
    )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Here the needed Libraries are added to the link command.
IF(WITH_QT5)
    QT5_USE_MODULES(${TANO_WIDGETS} PrintSupport Widgets)
    TARGET_LINK_LIBRARIES(tano ${LIBVLCQT_LIBRARY})
ELSE(WITH_QT5)
    TARGET_LINK_LIBRARIES(${TANO_WIDGETS} ${QT_LIBRARIES} ${LIBVLCQT_LIBRARY})
ENDIF(WITH_QT5)


################
# Installation #
################
INSTALL(TARGETS ${TANO_WIDGETS} RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
)
