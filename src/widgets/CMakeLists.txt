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

PROJECT(${TANO_WIDGETS})

# Add definition for exporting symbols
ADD_DEFINITIONS(-DTANO_WIDGETS_LIBRARY)

# Add definitions for shared library
ADD_DEFINITIONS(-DQT_SHARED -D_REENTRANT)

IF(MSVC)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/src/widgets/tano-widgets.rc.in
        ${CMAKE_BINARY_DIR}/src/widgets/tano-widgets.rc
    )
ENDIF()


###################
# List of sources #
###################
# Define the UI source files used by Tano Widgets
SET(Tano_Widgets_UIs
    MainWindow.ui
    common/OsdWidget.ui
    dialogs/AboutDialog.ui
    dialogs/PasswordDialog.ui
    dialogs/UpdateDialog.ui
    editor/PlaylistEditor.ui
    editor/PlaylistEditorHelp.ui
    editor/PlaylistEditorScan.ui
    editor/PlaylistExportTvheadend.ui
    editor/PlaylistImportCSV.ui
    editor/PrintDialog.ui
    main/ShowInfoTab.ui
    recorder/RecorderInfoWidget.ui
    recorder/RecorderNewDialog.ui
    recorder/RecorderTimersEditor.ui
    settings/SettingsBackend.ui
    settings/SettingsDialog.ui
    settings/SettingsGeneral.ui
    settings/SettingsPlaylist.ui
    settings/SettingsRecorder.ui
    settings/SettingsSchedule.ui
    settings/SettingsDialogShortcuts.ui
    settings/SettingsUi.ui
)

# Define the C++ source files used by Tano Widgets
SET(Tano_Widgets_Application_Srcs
    application/Notifications.cpp
    application/SingleApplication.cpp
    application/TanoApplication.cpp
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(Tano_Widgets_Application_Srcs
        ${Tano_Widgets_Application_Srcs}

        application/NotificationsOSX.mm
    )
ENDIF()
SET(Tano_Widgets_Common_Srcs
    common/Backend.cpp
    common/BrowseWidget.cpp
    common/ChannelSelect.cpp
    common/DesktopShortcuts.cpp
    common/FileDialogs.cpp
    common/InfoBarWidget.cpp
    common/OsdFloat.cpp
    common/OsdWidget.cpp
    common/PlaylistDisplayWidget.cpp
    common/QtKeySequenceEdit.cpp
    common/ScrollingWidget.cpp
    common/TrayIcon.cpp
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(Tano_Widgets_Common_Srcs
        ${Tano_Widgets_Common_Srcs}

        common/FreedesktopImage.cpp
    )
ENDIF()
SET(Tano_Widgets_Dialogs_Srcs
    dialogs/AboutDialog.cpp
    dialogs/PasswordDialog.cpp
    dialogs/UpdateDialog.cpp
)
SET(Tano_Widgets_Editor_Srcs
    editor/PlaylistEditor.cpp
    editor/PlaylistEditorHelp.cpp
    editor/PlaylistEditorScan.cpp
    editor/PlaylistExportTvheadend.cpp
    editor/PlaylistImportCSV.cpp
    editor/Print.cpp
    editor/PrintDialog.cpp
)
SET(Tano_Widgets_Main_Srcs
    main/MediaPlayer.cpp
    main/PlaylistTab.cpp
    main/ScheduleTab.cpp
    main/ShowInfoTab.cpp
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
SET(Tano_Widgets_Recorder_Srcs
    recorder/Recorder.cpp
    recorder/RecorderCore.cpp
    recorder/RecorderInfoWidget.cpp
    recorder/RecorderNewDialog.cpp
    recorder/RecorderTimersEditor.cpp
    recorder/RecorderTimersWidget.cpp
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
SET(Tano_Widgets_Style_Srcs
    style/Common.cpp
    style/FancyLineEdit.cpp
    style/FancyTabWidget.cpp
    style/IconButton.cpp
    style/ManhattanStyle.cpp
    style/MiniSplitter.cpp
    style/StyleAnimator.cpp
    style/StyleHelper.cpp
    style/StyledBar.cpp
    style/VolumeSlider.cpp
)


SET(Tano_Widgets_Srcs
    MainWindow.cpp

    ../../resources/style/style.qrc

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
    FIND_PATH(APPLICATIONSERVICES_INCLUDE_DIR ApplicationServices/ApplicationServices.h)
    SET(APPLICATIONSERVICES_LIBRARY "-weak_framework ApplicationServices")
    FIND_PATH(COCOA_INCLUDE_DIR Cocoa/Cocoa.h)
    SET(COCOA_LIBRARY "-weak_framework Cocoa")

    INCLUDE_DIRECTORIES(${APPLICATIONSERVICES_INCLUDE_DIR}/Headers)
    SET_SOURCE_FILES_PROPERTIES(application/NotificationsOSX.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
ENDIF()

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

IF(MSVC)
    SET(Tano_Widgets_Srcs ${Tano_Widgets_Srcs} ${CMAKE_BINARY_DIR}/src/widgets/tano-widgets.rc)
ENDIF()

# This line tells cmake to create the Tano Widgets library.
ADD_LIBRARY(${TANO_WIDGETS} SHARED ${Tano_Widgets_Srcs} ${Tano_Widgets_UIC_Srcs})
ADD_DEPENDENCIES(${TANO_WIDGETS} ${TANO_CORE})

# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET_TARGET_PROPERTIES(${TANO_WIDGETS} PROPERTIES VERSION ${TANO_VERSION_MAJOR}
                                          SOVERSION "${TANO_VERSION_MAJOR}.${TANO_VERSION_MINOR}"
    )
ENDIF()

# Here the needed Libraries are added to the link command.
TARGET_LINK_LIBRARIES(${TANO_WIDGETS} Qt5::Network Qt5::PrintSupport Qt5::Sql Qt5::Widgets Qt5::Xml)
TARGET_LINK_LIBRARIES(${TANO_WIDGETS} VLCQt::Widgets)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    TARGET_LINK_LIBRARIES(${TANO_WIDGETS} DBus)
ENDIF()
TARGET_LINK_LIBRARIES(${TANO_WIDGETS} ${TANO_CORE_LIB})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    TARGET_LINK_LIBRARIES(${TANO_WIDGETS} ${APPLICATIONSERVICES_LIBRARY} ${COCOA_LIBRARY})
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    ADD_CUSTOM_COMMAND(TARGET ${TANO_WIDGETS} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change ${TANO_CORE_LIB} "@loader_path/../lib/${LIBSTART}${TANO_CORE}.${LIBEND}" $<TARGET_FILE:${TANO_WIDGETS}>)
    ADD_CUSTOM_COMMAND(TARGET ${TANO_WIDGETS} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change "libvlc-qt.dylib" "@loader_path/../lib/libvlc-qt.dylib" $<TARGET_FILE:${TANO_WIDGETS}>)
    ADD_CUSTOM_COMMAND(TARGET ${TANO_WIDGETS} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change "libvlc-qt-widgets.dylib" "@loader_path/../lib/libvlc-qt-widgets.dylib" $<TARGET_FILE:${TANO_WIDGETS}>)
ENDIF()

################
# Installation #
################
INSTALL(TARGETS ${TANO_WIDGETS} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_LIB_DIR}
)
