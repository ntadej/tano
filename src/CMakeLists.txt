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
	ui/dialogs/UpdateDialog.ui
	ui/epg/EpgShow.ui
	ui/epg/EpgScheduleChannel.ui
	ui/epg/EpgScheduleFull.ui
	ui/playlist/PlaylistEdit.ui
	ui/playlist/PlaylistExportTvheadend.ui
	ui/playlist/PlaylistImportCSV.ui
	ui/playlist/PlaylistImportWeb.ui
	ui/playlist/PlaylistSelect.ui
	ui/playlist/PlaylistWidget.ui
	ui/recorder/Recorder.ui
	ui/recorder/TimersEdit.ui
	ui/settings/SettingsEdit.ui
	ui/settings/SettingsPlaylist.ui
	ui/settings/SettingsShortcuts.ui
	ui/wizard/ConclusionPage.ui
	ui/wizard/IntroPage.ui
	ui/wizard/PlaylistPage.ui
	ui/wizard/SettingsPage.ui
)

# Define the MOC source files used by Tano
SET(Tano_MOCs	# List of classes which Qt needs to produce MOCs for them (in order for events, signals, slots and other macros to work
	core/ChannelSelect.h
	core/GetFile.h
	core/Print.h
	core/UpdateManager.h
	epg/XmltvManager.h
	ui/MainWindow.h
	ui/core/CurrentTime.h
	ui/core/InfoBar.h
	ui/core/License.h
	ui/core/QtKeySequenceEdit.h
	ui/core/Teletext.h
	ui/core/TrayIcon.h
	ui/dialogs/AboutDialog.h
	ui/dialogs/DonationDialog.h
	ui/dialogs/PrintDialog.h
	ui/dialogs/UpdateDialog.h
	ui/epg/EpgList.h
	ui/epg/EpgScheduleChannel.h
	ui/epg/EpgScheduleFull.h
	ui/epg/EpgShow.h
	ui/menu/MenuAspectRatio.h
	ui/menu/MenuCore.h
	ui/menu/MenuCrop.h
	ui/menu/MenuDeinterlacing.h
	ui/menu/MenuTrackAudio.h
	ui/menu/MenuTrackSubtitles.h
	ui/menu/MenuTrackVideo.h
	ui/playlist/PlaylistEdit.h
	ui/playlist/PlaylistExportTvheadend.h
	ui/playlist/PlaylistImportCSV.h
	ui/playlist/PlaylistImportWeb.h
	ui/playlist/PlaylistSelect.h
	ui/playlist/PlaylistWidget.h
	ui/recorder/Recorder.h
	ui/recorder/TimersEdit.h
	ui/settings/SettingsEdit.h
	ui/settings/SettingsPlaylist.h
	ui/settings/SettingsShortcuts.h
	ui/wizard/ConclusionPage.h
	ui/wizard/FirstRunWizard.h
	ui/wizard/IntroPage.h
	ui/wizard/PlaylistPage.h
	ui/wizard/SettingsPage.h
)

IF(DBUS_FOUND)
	SET(Tano_MOCs
		${Tano_MOCs}
		core/RecorderController.h
	)
ENDIF(DBUS_FOUND)

SET(Tano_Resrcs
	../info.qrc
	../data/icons.qrc
	../data/images.qrc
	../data/logo.qrc
)

# Define the C++ source files used by Tano
SET(Tano_Container_Srcs
	container/Channel.cpp
	container/PlaylistFile.cpp
	container/UpdateInfo.cpp
	container/Timer.cpp
	container/xmltv/XmltvChannel.cpp
	container/xmltv/XmltvList.cpp
	container/xmltv/XmltvProgramme.cpp
)
SET(Tano_Core_Srcs
	core/Arguments.cpp
	core/ChannelSelect.cpp
	core/Common.cpp
	core/ConsoleOutput.cpp
	core/Enums.cpp
	core/GetFile.cpp
	core/LocaleManager.cpp
	core/Print.cpp
	core/Settings.cpp
	core/Shortcuts.cpp
	core/Udpxy.cpp
	core/UpdateManager.cpp
)
IF(DBUS_FOUND)
	SET(Tano_Core_Srcs
		${Tano_Core_Srcs}
		core/RecorderController.cpp
		core/RecorderProcess.cpp
	)
ENDIF(DBUS_FOUND)
SET(Tano_Epg_Srcs
	epg/XmltvCommon.cpp
	epg/XmltvManager.cpp
)
SET(Tano_Playlist_Srcs
	playlist/CSVGenerator.cpp
	playlist/CSVHandler.cpp
	playlist/JsGenerator.cpp
	playlist/JsHandler.cpp
	playlist/M3UGenerator.cpp
	playlist/M3UHandler.cpp
	playlist/PlaylistHandler.cpp
	playlist/TvheadendGenerator.cpp
)
SET(Tano_UI_Srcs
	ui/MainWindow.cpp
	ui/core/CurrentTime.cpp
	ui/core/InfoBar.cpp
	ui/core/License.cpp
	ui/core/QtKeySequenceEdit.cpp
	ui/core/Teletext.cpp
	ui/core/TrayIcon.cpp
	ui/dialogs/AboutDialog.cpp
	ui/dialogs/DonationDialog.cpp
	ui/dialogs/PrintDialog.cpp
	ui/dialogs/UpdateDialog.cpp
	ui/epg/EpgList.cpp
	ui/epg/EpgScheduleChannel.cpp
	ui/epg/EpgScheduleFull.cpp
	ui/epg/EpgShow.cpp
	ui/menu/MenuAspectRatio.cpp
	ui/menu/MenuCore.cpp
	ui/menu/MenuCrop.cpp
	ui/menu/MenuDeinterlacing.cpp
	ui/menu/MenuTrackAudio.cpp
	ui/menu/MenuTrackSubtitles.cpp
	ui/menu/MenuTrackVideo.cpp
	ui/playlist/PlaylistEdit.cpp
	ui/playlist/PlaylistExportTvheadend.cpp
	ui/playlist/PlaylistImportCSV.cpp
	ui/playlist/PlaylistImportWeb.cpp
	ui/playlist/PlaylistSelect.cpp
	ui/playlist/PlaylistWidget.cpp
	ui/recorder/Recorder.cpp
	ui/recorder/TimersEdit.cpp
	ui/settings/SettingsEdit.cpp
	ui/settings/SettingsPlaylist.cpp
	ui/settings/SettingsShortcuts.cpp
	ui/wizard/ConclusionPage.cpp
	ui/wizard/FirstRunWizard.cpp
	ui/wizard/IntroPage.cpp
	ui/wizard/PlaylistPage.cpp
	ui/wizard/SettingsPage.cpp
)
SET(Tano_XML_Srcs
	xml/PlaylistListHandler.cpp
	xml/TanoHandlerOld.cpp
	xml/TimersGenerator.cpp
	xml/TimersHandler.cpp
	xml/UpdateHandler.cpp
	xml/XmltvHandler.cpp
)

SET(Tano_Srcs
	main.cpp

	${Tano_Container_Srcs}
	${Tano_Core_Srcs}
	${Tano_Epg_Srcs}
	${Tano_Playlist_Srcs}
	${Tano_UI_Srcs}
	${Tano_XML_Srcs}
)

################
# Qt4 settings #
################
# This line adds the QtXML, QtNETWORK and QtDBUS libraries to the Qt libraries
# loaded when linking Tano
SET(QT_USE_QTXML True)
SET(QT_USE_QTNETWORK True)
IF(DBUS_FOUND)
	SET(QT_USE_QTDBUS True)
ENDIF(DBUS_FOUND)

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
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} "${CMAKE_BINARY_DIR}/src" "${CMAKE_SOURCE_DIR}/src")

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

# Here the needed Qt Libraries are added to the link command.
TARGET_LINK_LIBRARIES(tano ${QT_LIBRARIES} ${LIBVLCQT_LIBRARY})


################
# Installation #
################
# Install tano exe
INSTALL(TARGETS tano
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
)
