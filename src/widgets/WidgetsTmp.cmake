#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2014 Tadej Novak <tadej@tano.si>
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

###################
# List of sources #
###################
# Define the UI source files used by Tano Widgets
SET(Tano_Widgets_UIs
    widgets/MainWindow.ui
    widgets/common/OsdWidget.ui
    widgets/dialogs/AboutDialog.ui
    widgets/editor/PlaylistEditor.ui
    widgets/editor/PlaylistEditorHelp.ui
    widgets/editor/PlaylistEditorScan.ui
    widgets/editor/PlaylistExportTvheadend.ui
    widgets/editor/PlaylistImportCSV.ui
    widgets/editor/PrintDialog.ui
    widgets/main/ShowInfoTab.ui
    widgets/recorder/RecorderInfoWidget.ui
    widgets/recorder/RecorderNewDialog.ui
    widgets/recorder/RecorderTimersEditor.ui
    widgets/settings/SettingsBackend.ui
    widgets/settings/SettingsDialog.ui
    widgets/settings/SettingsGeneral.ui
    widgets/settings/SettingsPlaylist.ui
    widgets/settings/SettingsRecorder.ui
    widgets/settings/SettingsSchedule.ui
    widgets/settings/SettingsDialogShortcuts.ui
    widgets/settings/SettingsUi.ui
)

# Define the C++ source files used by Tano Widgets
SET(Tano_Widgets_Application_Srcs
    widgets/application/Notifications.cpp
    widgets/application/SingleApplication.cpp
    widgets/application/TanoApplication.cpp
    widgets/application/Updates.cpp
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(Tano_Widgets_Application_Srcs
        ${Tano_Widgets_Application_Srcs}

        widgets/application/NotificationsOSX.mm
        widgets/application/UpdatesOSX.mm
    )
ENDIF()
SET(Tano_Widgets_Common_Srcs
    widgets/common/Backend.cpp
    widgets/common/BrowseWidget.cpp
    widgets/common/ChannelSelect.cpp
    widgets/common/DesktopShortcuts.cpp
    widgets/common/FileDialogs.cpp
    widgets/common/InfoBarWidget.cpp
    widgets/common/OsdFloat.cpp
    widgets/common/OsdWidget.cpp
    widgets/common/PlaylistDisplayWidget.cpp
    widgets/common/QtKeySequenceEdit.cpp
    widgets/common/ScrollingWidget.cpp
    widgets/common/TrayIcon.cpp
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(Tano_Widgets_Common_Srcs
        ${Tano_Widgets_Common_Srcs}

        widgets/common/FreedesktopImage.cpp
    )
ENDIF()
SET(Tano_Widgets_Dialogs_Srcs
    widgets/dialogs/AboutDialog.cpp
)
SET(Tano_Widgets_Editor_Srcs
    widgets/editor/PlaylistEditor.cpp
    widgets/editor/PlaylistEditorHelp.cpp
    widgets/editor/PlaylistEditorScan.cpp
    widgets/editor/PlaylistExportTvheadend.cpp
    widgets/editor/PlaylistImportCSV.cpp
    widgets/editor/Print.cpp
    widgets/editor/PrintDialog.cpp
)
SET(Tano_Widgets_Main_Srcs
    widgets/main/MediaPlayer.cpp
    widgets/main/PlaylistTab.cpp
    widgets/main/ScheduleTab.cpp
    widgets/main/ShowInfoTab.cpp
)
SET(Tano_Widgets_Menu_Srcs
    widgets/menu/MenuAspectRatio.cpp
    widgets/menu/MenuCore.cpp
    widgets/menu/MenuCropRatio.cpp
    widgets/menu/MenuDeinterlacing.cpp
    widgets/menu/MenuScale.cpp
    widgets/menu/MenuTrackAudio.cpp
    widgets/menu/MenuTrackSubtitles.cpp
    widgets/menu/MenuTrackVideo.cpp
)
SET(Tano_Widgets_Recorder_Srcs
    widgets/recorder/Recorder.cpp
    widgets/recorder/RecorderCore.cpp
    widgets/recorder/RecorderInfoWidget.cpp
    widgets/recorder/RecorderNewDialog.cpp
    widgets/recorder/RecorderTimersEditor.cpp
    widgets/recorder/RecorderTimersWidget.cpp
)
SET(Tano_Widgets_Settings_Srcs
    widgets/settings/SettingsBackend.cpp
    widgets/settings/SettingsDialog.cpp
    widgets/settings/SettingsDialogShortcuts.cpp
    widgets/settings/SettingsGeneral.cpp
    widgets/settings/SettingsPlaylist.cpp
    widgets/settings/SettingsRecorder.cpp
    widgets/settings/SettingsSchedule.cpp
    widgets/settings/SettingsUi.cpp
)
SET(Tano_Widgets_Style_Srcs
    widgets/style/Common.cpp
    widgets/style/FancyLineEdit.cpp
    widgets/style/FancyTabWidget.cpp
    widgets/style/IconButton.cpp
    widgets/style/ManhattanStyle.cpp
    widgets/style/MiniSplitter.cpp
    widgets/style/StyleAnimator.cpp
    widgets/style/StyleHelper.cpp
    widgets/style/StyledBar.cpp
    widgets/style/VolumeSlider.cpp
)


SET(Tano_Widgets_Srcs
    widgets/MainWindow.cpp

    ${Tano_Widgets_Application_Srcs}
    ${Tano_Widgets_Common_Srcs}
    ${Tano_Widgets_Dialogs_Srcs}
    ${Tano_Widgets_Editor_Srcs}
    ${Tano_Widgets_Main_Srcs}
    ${Tano_Widgets_Menu_Srcs}
    ${Tano_Widgets_Recorder_Srcs}
    ${Tano_Widgets_Settings_Srcs}
    ${Tano_Widgets_Style_Srcs}
)


###############
# Qt settings #
###############
# This script runs the user interface compiler uic
QT5_WRAP_UI(Tano_Widgets_UIC_Srcs ${Tano_Widgets_UIs})


#####################
# Compiler settings #
#####################
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    INCLUDE_DIRECTORIES(${SPARKLE_INCLUDE_DIR})
    SET_SOURCE_FILES_PROPERTIES(widgets/application/NotificationsOSX.mm PROPERTIES COMPILE_FLAGS "-x objective-c++")
    SET_SOURCE_FILES_PROPERTIES(widgets/application/UpdatesOSX.mm PROPERTIES COMPILE_FLAGS "-x objective-c++")
ENDIF()
