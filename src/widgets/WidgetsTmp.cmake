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
SET(Project_Widgets_UIs
    widgets/MainWindow.ui
    widgets/common/OsdWidget.ui
    widgets/main/ShowInfoTab.ui
    widgets/recorder/RecorderInfoWidget.ui
    widgets/recorder/RecorderNewDialog.ui
    widgets/recorder/RecorderTimersEditor.ui
)

# Define the C++ source files used by Tano Widgets
SET(Project_Widgets_Common_Srcs
    widgets/common/InfoBarWidget.cpp
    widgets/common/OsdFloat.cpp
    widgets/common/OsdWidget.cpp
    widgets/common/ScrollingWidget.cpp
    widgets/common/TrayIcon.cpp
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(Project_Widgets_Common_Srcs
        ${Project_Widgets_Common_Srcs}

        widgets/common/FreedesktopImage.cpp
    )
ENDIF()

SET(Project_Widgets_Main_Srcs
    widgets/main/MediaPlayer.cpp
    widgets/main/ChannelsTab.cpp
    widgets/main/ScheduleTab.cpp
    widgets/main/ShowInfoTab.cpp
)
SET(Project_Widgets_Menu_Srcs
    widgets/menu/MenuAspectRatio.cpp
    widgets/menu/MenuCore.cpp
    widgets/menu/MenuCropRatio.cpp
    widgets/menu/MenuDeinterlacing.cpp
    widgets/menu/MenuScale.cpp
    widgets/menu/MenuTrackAudio.cpp
    widgets/menu/MenuTrackSubtitles.cpp
    widgets/menu/MenuTrackVideo.cpp
)
SET(Project_Widgets_Recorder_Srcs
    widgets/recorder/Recorder.cpp
    widgets/recorder/RecorderCore.cpp
    widgets/recorder/RecorderInfoWidget.cpp
    widgets/recorder/RecorderNewDialog.cpp
    widgets/recorder/RecorderTimersEditor.cpp
    widgets/recorder/RecorderTimersWidget.cpp
)

SET(Project_Widgets_Tmp_Srcs
    widgets/MainWindow.cpp

    ${Project_Widgets_Application_Srcs}
    ${Project_Widgets_Common_Srcs}
    ${Project_Widgets_Main_Srcs}
    ${Project_Widgets_Menu_Srcs}
    ${Project_Widgets_Recorder_Srcs}

    ${Project_Widgets_UIs}
)


###############
# Qt settings #
###############
# This script runs the user interface compiler uic
QT5_WRAP_UI(Project_Widgets_UIC_Srcs ${Project_Widgets_UIs})
