#############################################################################
# Tano - An open IP TV player
#############################################################################
# Copyright (C) 2008-2010 Tadej Novak
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# This file may be used under the terms of the
# GNU General Public License version 3.0 as published by the
# Free Software Foundation and appearing in the file LICENSE.GPL
# included in the packaging of this file.
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
	ui/dialogs/PluginsDialog.ui
	ui/dialogs/PrintDialog.ui
	ui/dialogs/UpdateDialog.ui
	ui/epg/EpgFull.ui
	ui/epg/EpgShow.ui
	ui/epg/EpgSchedule.ui
	ui/playlist/PlaylistEdit.ui
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
	MainWindow.h
	core/ChannelSelect.h
	core/GetFile.h
	core/Print.h
	core/Time.h
	core/UpdateManager.h
	epg/EpgList.h
	epg/EpgManager.h
	epg/EpgSloveniaLoader.h
	ui/core/CurrentTime.h
	ui/core/InfoBar.h
	ui/core/License.h
	ui/core/QtKeySequenceEdit.h
	ui/core/TrayIcon.h
	ui/dialogs/AboutDialog.h
	ui/dialogs/PluginsDialog.h
	ui/dialogs/PrintDialog.h
	ui/dialogs/UpdateDialog.h
	ui/epg/EpgFull.h
	ui/epg/EpgSchedule.h
	ui/epg/EpgShow.h
	ui/playlist/PlaylistEdit.h
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

SET(Tano_Resrcs
	../data/icons.qrc
	../data/images.qrc
	../data/logo.qrc
)

# Define the C++ source files used by Tano
SET(Tano_Container_Srcs
	container/Channel.cpp
	container/EpgItem.cpp
	container/EpgShowInfo.cpp
	container/UpdateInfo.cpp
	container/Timer.cpp
)
SET(Tano_Core_Srcs
	core/ChannelSelect.cpp
	core/Common.cpp
	core/GetFile.cpp
	core/LocaleManager.cpp
	core/PluginsLoader.cpp
	core/Print.cpp
	core/Settings.cpp
	core/Shortcuts.cpp
	core/Time.cpp
	core/UpdateManager.cpp
	core/Version.cpp
)
SET(Tano_Epg_Srcs
	epg/EpgList.cpp
	epg/EpgManager.cpp
	epg/EpgSlovenia.cpp
	epg/EpgSloveniaLoader.cpp
)
SET(Tano_UI_Srcs
	ui/core/CurrentTime.cpp
	ui/core/InfoBar.cpp
	ui/core/License.cpp
	ui/core/QtKeySequenceEdit.cpp
	ui/core/TrayIcon.cpp
	ui/dialogs/AboutDialog.cpp
	ui/dialogs/PluginsDialog.cpp
	ui/dialogs/PrintDialog.cpp
	ui/dialogs/UpdateDialog.cpp
	ui/epg/EpgFull.cpp
	ui/epg/EpgSchedule.cpp
	ui/epg/EpgShow.cpp
	ui/playlist/PlaylistEdit.cpp
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
	xml/M3UGenerator.cpp
	xml/M3UHandler.cpp
	xml/TanoHandlerOld.cpp
	xml/TimersGenerator.cpp
	xml/TimersHandler.cpp
	xml/UpdateHandler.cpp
)

SET(Tano_Srcs
	main.cpp
	MainWindow.cpp

	${Tano_Container_Srcs}
	${Tano_Core_Srcs}
	${Tano_Epg_Srcs}
	${Tano_UI_Srcs}
	${Tano_XML_Srcs}
)

SET(Tano_Plugins_Headers
	plugins/PluginsLoader.cpp
	plugins/PluginsManager.cpp
)
################
# Qt4 settings #
################
# This line adds the QtXML and QtNETWORK libraries to the Qt libraries
# loaded when linking Tano
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
INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}/src" "${CMAKE_SOURCE_DIR}/src")

# This line adds the include path of the Qt
# directory to the compiler include path
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})
# MinGW needs some more includes
IF(MINGW)
	INCLUDE_DIRECTORIES(${LIBVLC_INCLUDE_DIR})
ENDIF(MINGW)

IF(MINGW)
	# CMake currently doesn't support windres yet. Add command manually
	FIND_PROGRAM(
		WINDRES_PROGRAM windres.exe PATHS
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/bin"
		c:/MinGW/bin /MinGW/bin
	)
	ADD_CUSTOM_COMMAND(
		OUTPUT ${CMAKE_BINARY_DIR}/src/tano.obj
		COMMAND ${WINDRES_PROGRAM}
		-i ${CMAKE_SOURCE_DIR}/src/tano.rc
		-o ${CMAKE_BINARY_DIR}/src/tano.obj
		WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src"
	)
	SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_BINARY_DIR}/src/tano.obj)
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
