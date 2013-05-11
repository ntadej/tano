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

PROJECT(${TANO_WIDGETS})

# Add definition for exporting symbols
ADD_DEFINITIONS(-DTANO_WIDGETS_LIBRARY)

# Add definitions for static/shared library
IF(STATIC)
    ADD_DEFINITIONS(-DQT_STATIC -D_REENTRANT)
ELSE(STATIC)
    ADD_DEFINITIONS(-DQT_SHARED -D_REENTRANT)
ENDIF(STATIC)


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
    settings/SettingsBackend.ui
    settings/SettingsDialog.ui
    settings/SettingsGeneral.ui
    settings/SettingsPlaylist.ui
    settings/SettingsRecorder.ui
    settings/SettingsSchedule.ui
    settings/SettingsDialogShortcuts.ui
    settings/SettingsUi.ui
)
SET(Tano_Widgets_Recorder_UIs
    recorder/Recorder.ui
    recorder/RecorderInfoWidget.ui
    recorder/RecorderNewDialog.ui
    timers/TimersDisplayWidget.ui
    timers/TimersEditorWidget.ui
)
IF(FEATURE_RECORDER)
    SET(Tano_Widgets_UIs
        ${Tano_Widgets_UIs}
        ${Tano_Widgets_Recorder_UIs}
    )
ENDIF(FEATURE_RECORDER)

# Define the C++ source files used by Tano Widgets
SET(Tano_Widgets_Common_Srcs
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
)
SET(Tano_Widgets_Dialogs_Srcs
    dialogs/AboutDialog.cpp
    dialogs/DonationDialog.cpp
    dialogs/PrintDialog.cpp
    dialogs/UpdateDialog.cpp
)
SET(Tano_Widgets_Epg_Srcs
    epg/EpgScheduleChannel.cpp
    epg/EpgScheduleFull.cpp
    epg/EpgShow.cpp
)
SET(Tano_Widgets_Menu_Srcs
    menu/MenuAspectRatio.cpp
    menu/MenuCore.cpp
    menu/MenuCropRatio.cpp
    menu/MenuDeinterlacing.cpp
    menu/MenuScale.cpp
    menu/MenuTrackAudio.cpp
    menu/MenuTrackSubtitles.cpp
    menu/MenuTrackVideo.cpp
)
SET(Tano_Widgets_Playlist_Srcs
    playlist/PlaylistDisplayWidget.cpp
    playlist/PlaylistEditor.cpp
    playlist/PlaylistEditorHelp.cpp
    playlist/PlaylistEditorScan.cpp
    playlist/PlaylistExportTvheadend.cpp
    playlist/PlaylistFilterWidget.cpp
    playlist/PlaylistImportCSV.cpp
)
SET(Tano_Widgets_Recorder_Srcs
    recorder/Recorder.cpp
    recorder/RecorderCore.cpp
    recorder/RecorderInfoWidget.cpp
    recorder/RecorderNewDialog.cpp
)
SET(Tano_Widgets_Settings_Srcs
    settings/SettingsBackend.cpp
    settings/SettingsDialog.cpp
    settings/SettingsDialogShortcuts.cpp
    settings/SettingsGeneral.cpp
    settings/SettingsPlaylist.cpp
    settings/SettingsRecorder.cpp
    settings/SettingsSchedule.cpp
    settings/SettingsUi.cpp
)
SET(Tano_Widgets_Timers_Srcs
    timers/TimersDisplayWidget.cpp
    timers/TimersEditorWidget.cpp
)

SET(Tano_Widgets_Srcs
    MainWindow.cpp

    ${Tano_Widgets_Common_Srcs}
    ${Tano_Widgets_Dialogs_Srcs}
    ${Tano_Widgets_Epg_Srcs}
    ${Tano_Widgets_Menu_Srcs}
    ${Tano_Widgets_Playlist_Srcs}
    ${Tano_Widgets_Settings_Srcs}
)

IF(FEATURE_RECORDER)
    SET(Tano_Widgets_Srcs
        ${Tano_Widgets_Srcs}
        ${Tano_Widgets_Recorder_Srcs}
        ${Tano_Widgets_Timers_Srcs}
    )
ENDIF(FEATURE_RECORDER)

IF(BRANDING)
    SET(Tano_Widgets_Srcs
        ${Tano_Widgets_Srcs}

        ../../branding/Branding.cpp
    )
ENDIF(BRANDING)


###############
# Qt settings #
###############
# This script runs the user interface compiler uic
IF(QT5)
    QT5_WRAP_UI(Tano_Widgets_UIC_Srcs ${Tano_Widgets_UIs})
ELSE(QT5)
    QT4_WRAP_UI(Tano_Widgets_UIC_Srcs ${Tano_Widgets_UIs})
ENDIF(QT5)


#####################
# Compiler settings #
#####################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/widgets
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_BINARY_DIR}/src/widgets
)

# Include additional libraries
INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})

# Compile resources with windres
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/platform/windows/tano-widgets.obj
        COMMAND ${CMAKE_RC_COMPILER}
        -i ${CMAKE_SOURCE_DIR}/src/platform/windows/tano-widgets.rc
        -o ${CMAKE_BINARY_DIR}/src/platform/windows/tano-widgets.obj
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/platform/windows"
    )
    SET(Tano_Widgets_Srcs ${Tano_Widgets_Srcs} ${CMAKE_BINARY_DIR}/src/platform/windows/tano-widgets.obj)
ENDIF(MINGW)

IF(MSVC)
    SET(Tano_Widgets_Srcs ${Tano_Widgets_Srcs} ${CMAKE_SOURCE_DIR}/src/platform/windows/tano-widgets.rc)
ENDIF(MSVC)

# This line tells cmake to create the Tano Widgets library.
IF(STATIC)
    ADD_LIBRARY(${TANO_WIDGETS} STATIC ${Tano_Widgets_Srcs} ${Tano_Widgets_UIC_Srcs})
ELSE(STATIC)
    ADD_LIBRARY(${TANO_WIDGETS} SHARED ${Tano_Widgets_Srcs} ${Tano_Widgets_UIC_Srcs})
ENDIF(STATIC)

# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF(MSVC)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES VERSION ${TANO_VERSION_MAJOR}
                                          SOVERSION "${TANO_VERSION_MAJOR}.${TANO_VERSION_MINOR}"
    )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Here the needed Libraries are added to the link command.
IF(QT5)
    QT5_USE_MODULES(${TANO_WIDGETS} Network PrintSupport Widgets Xml)
    TARGET_LINK_LIBRARIES(${TANO_WIDGETS} ${TANO_CORE_LIB} ${LIBVLCQT_LIBRARY} ${LIBVLCQT_WIDGETS_LIBRARY})
ELSE(QT5)
    TARGET_LINK_LIBRARIES(${TANO_WIDGETS} ${QT_LIBRARIES} ${TANO_CORE_LIB} ${LIBVLCQT_LIBRARY} ${LIBVLCQT_WIDGETS_LIBRARY})
ENDIF(QT5)


################
# Installation #
################
INSTALL(TARGETS ${TANO_WIDGETS} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
)
