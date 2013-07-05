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

# Add definitions for static/style library
IF(STATIC)
    ADD_DEFINITIONS(-DQT_STATIC -D_REENTRANT)
ELSE(STATIC)
    ADD_DEFINITIONS(-DQT_SHARED -D_REENTRANT)
ENDIF(STATIC)

IF(MINGW OR MSVC)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/src/widgets/tano-widgets.rc.in
        ${CMAKE_BINARY_DIR}/src/widgets/tano-widgets.rc
    )
ENDIF(MINGW OR MSVC)


###################
# List of sources #
###################
# Define the QRC source files used by Tano
IF(BRANDING)
    SET(Tano_Widgets_Resrcs
        ../../branding/logo/logo.qrc
        ../../data/misc/misc.qrc
        ../../data/style/style.qrc
    )
ELSE(BRANDING)
    SET(Tano_Widgets_Resrcs
        ../../data/logo/logo.qrc
        ../../data/misc/misc.qrc
        ../../data/style/style.qrc
    )
ENDIF(BRANDING)

# Define the UI source files used by Tano Widgets
SET(Tano_Widgets_UIs
    MainWindow.ui
    common/OsdWidget.ui
    dialogs/AboutDialog.ui
    dialogs/PasswordDialog.ui
    dialogs/UpdateDialog.ui
    main/ShowInfoTab.ui
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
    common/PlaylistDisplayWidget.cpp
    common/QtKeySequenceEdit.cpp
    common/ScrollingWidget.cpp
    common/TrayIcon.cpp
)
SET(Tano_Widgets_Dialogs_Srcs
    dialogs/AboutDialog.cpp
    dialogs/PasswordDialog.cpp
    dialogs/UpdateDialog.cpp
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
SET(Tano_Widgets_Timers_Srcs
    timers/TimersDisplayWidget.cpp
    timers/TimersEditorWidget.cpp
)

SET(Tano_Widgets_Srcs
    MainWindow.cpp

    ${Tano_Widgets_Common_Srcs}
    ${Tano_Widgets_Dialogs_Srcs}
    ${Tano_Widgets_Main_Srcs}
    ${Tano_Widgets_Menu_Srcs}
    ${Tano_Widgets_Settings_Srcs}
    ${Tano_Widgets_Style_Srcs}
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
    QT5_ADD_RESOURCES(Tano_Widgets_Resrcs_Srcs ${Tano_Widgets_Resrcs})
ELSE(QT5)
    QT4_WRAP_UI(Tano_Widgets_UIC_Srcs ${Tano_Widgets_UIs})
    QT4_ADD_RESOURCES(Tano_Widgets_Resrcs_Srcs ${Tano_Widgets_Resrcs})
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
        OUTPUT ${CMAKE_BINARY_DIR}/src/widgets/tano-widgets.obj
        COMMAND ${CMAKE_RC_COMPILER}
        -i ${CMAKE_BINARY_DIR}/src/widgets/tano-widgets.rc
        -o ${CMAKE_BINARY_DIR}/src/widgets/tano-widgets.obj
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/widgets"
    )
    SET(Tano_Widgets_Srcs ${Tano_Widgets_Srcs} ${CMAKE_BINARY_DIR}/src/widgets/tano-widgets.obj)
ENDIF(MINGW)

IF(MSVC)
    SET(Tano_Widgets_Srcs ${Tano_Widgets_Srcs} ${CMAKE_BINARY_DIR}/src/widgets/tano-widgets.rc)
ENDIF(MSVC)

# This line tells cmake to create the Tano Widgets library.
IF(STATIC)
    ADD_LIBRARY(${TANO_WIDGETS} STATIC ${Tano_Widgets_Srcs} ${Tano_Widgets_UIC_Srcs} ${Tano_Widgets_Resrcs_Srcs})
ELSE(STATIC)
    ADD_LIBRARY(${TANO_WIDGETS} SHARED ${Tano_Widgets_Srcs} ${Tano_Widgets_UIC_Srcs} ${Tano_Widgets_Resrcs_Srcs})
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
    QT5_USE_MODULES(${TANO_WIDGETS} Network Sql Widgets Xml)
    TARGET_LINK_LIBRARIES(${TANO_WIDGETS} ${TANO_CORE_LIB} ${LIBVLCQT_LIBRARY} ${LIBVLCQT_WIDGETS_LIBRARY})
ELSE(QT5)
    TARGET_LINK_LIBRARIES(${TANO_WIDGETS} ${QT_LIBRARIES} ${TANO_CORE_LIB} ${LIBVLCQT_LIBRARY} ${LIBVLCQT_WIDGETS_LIBRARY})
ENDIF(QT5)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    ADD_CUSTOM_COMMAND(TARGET ${TANO_WIDGETS} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change ${TANO_CORE_LIB} "@loader_path/../lib/${LIBSTART}${TANO_CORE}.${LIBEND}" $<TARGET_FILE:${TANO_WIDGETS}>)
    ADD_CUSTOM_COMMAND(TARGET ${TANO_WIDGETS} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change "libvlc-qt.dylib" "@loader_path/../lib/libvlc-qt.dylib" $<TARGET_FILE:${TANO_WIDGETS}>)
    ADD_CUSTOM_COMMAND(TARGET ${TANO_WIDGETS} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change "libvlc-qt-widgets.dylib" "@loader_path/../lib/libvlc-qt-widgets.dylib" $<TARGET_FILE:${TANO_WIDGETS}>)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

################
# Installation #
################
INSTALL(TARGETS ${TANO_WIDGETS} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_LIB_DIR}
)
