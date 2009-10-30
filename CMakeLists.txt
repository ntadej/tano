# The name of our project is "Tano". CMakeLists files in this project can
# refer to the root source directory of the project as ${TANO_SOURCE_DIR}
# and to the root binary directory of the project as ${TANO_BINARY_DIR}.
PROJECT(Tano)

cmake_minimum_required(VERSION 2.6)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
)

# Set MingW bin dir, if MingW present
IF(MINGW)
	STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" MINGW_BIN_DIR_TMP "${CMAKE_CXX_COMPILER}")
	STRING(REGEX REPLACE "\\\\" "/" MINGW_BIN_DIR "${MINGW_BIN_DIR_TMP}") # Replace back slashes to slashes
ENDIF(MINGW)

IF(MINGW)
	SET(CMAKE_EXE_LINKER_FLAGS "-enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -mthreads -Wl -Wl,-subsystem,windows")
ENDIF(MINGW)

# Set Qt4 bin dir to find QtCore4.dll and other libs to install
STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" QT_BIN_DIR_TMP "${QT_QMAKE_EXECUTABLE}")
STRING(REGEX REPLACE "\\\\" "/" QT_BIN_DIR "${QT_BIN_DIR_TMP}")  # Replace back slashes to slashes

#-----------------------------------------------------------------------------
# Tano version number.
FILE(READ VERSION TANO_VERSION)
STRING(REGEX REPLACE "\n" "" TANO_VERSION "${TANO_VERSION}") # get rid of the newline at the end

# Set default install path:
# In *nix environments this is /usr/local 
# Under M$ Windows this is C:\Program files\Tano
# WARNING: Leading slash should be added - this is for easier concatenation of the strings due to slash/backslash issues
IF(MINGW)
	IF (NOT CMAKE_INSTALL_PREFIX)
		SET(CMAKE_INSTALL_PREFIX "C:/Program files/Tano")
	ENDIF (NOT CMAKE_INSTALL_PREFIX)
	SET(TANO_INSTALL_DATA_DIR "")
	SET(TANO_INSTALL_BIN_DIR "")
ELSE(MINGW)
	IF (NOT CMAKE_INSTALL_PREFIX)
		SET(CMAKE_INSTALL_PREFIX "/usr/local")
	ENDIF (NOT CMAKE_INSTALL_PREFIX)
	SET(TANO_INSTALL_DATA_DIR "share/tano")
	SET(TANO_INSTALL_BIN_DIR "bin")
ENDIF(MINGW)

#-----------------------------------------------------------------------------
# Search for Qt4. We need the latest version of Qt
SET(QT_MIN_VERSION "4.4.0")

# this line includes FindQt4.cmake, which searches the Qt library and headers
FIND_PACKAGE(Qt4 REQUIRED)


find_package(VLC REQUIRED)
if (VLC_FOUND)
	include_directories(${VLC_INCLUDE_DIRS})
endif (VLC_FOUND)

# This module is used to resolve the file dependencies
# for QT files using an UI (User Unterface) Qt file
# created with the Qt4 designer
# INCLUDE(AddFileDependencies)

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")


###################
# List of sources #
###################

# Define the UI source files used by Tano
SET(Tano_UIs		# List of .ui forms drawn by qt-designer that need to be compiled
	ui/About.ui
	ui/EditPlaylist.ui
	ui/EditSettings.ui
	ui/EpgBrowser.ui
	ui/EpgShow.ui
	ui/MainWindow.ui
	ui/Osd.ui
	ui/Recorder.ui
)

# Define the MOC source files used by Tano
SET(Tano_MOCs	# List of classes which Qt needs to produce MOCs for them (in order for events, signals, slots and other macros to work
	MainWindow.h
	Updates.h
	channels/EditPlaylist.h
	control/ChannelSelect.h
	control/Shortcuts.h
	epg/Epg.h
	epg/EpgBrowser.h
	epg/EpgLoader.h
	epg/EpgShow.h
	epg/EpgToday.h
	lib/QtKeySequenceEdit.h
	libvlc/VlcControl.h	
	libvlc/VlcInstance.h
	libvlc/VlcSeekWidget.h
    libvlc/VlcVideoWidget.h
	libvlc/VlcVolumeSlider.h
	recorder/Recorder.h
	ui/About.h
	ui/EditSettings.h
	ui/FirstRun.h
	ui/License.h
	ui/Osd.h
	ui/TrayIcon.h
	ui/TrayRecorder.h
)

SET(Tano_Resrcs ui/images.qrc)

SET(Tano_Class_Srcs		# Core sources - data part of the MVC model
	channels/Channel.cpp
	channels/ChannelEpg.cpp
	channels/EditPlaylist.cpp
	control/ChannelSelect.cpp
	control/Shortcuts.cpp
	epg/Epg.cpp
	epg/EpgBrowser.cpp
	epg/EpgLoader.cpp
	epg/EpgShow.cpp
	epg/EpgToday.cpp
	lib/QtKeySequenceEdit.cpp
	libvlc/VlcControl.cpp
	libvlc/VlcInstance.cpp
	libvlc/VlcSeekWidget.cpp
	libvlc/VlcVideoWidget.cpp
	libvlc/VlcVolumeSlider.cpp
	recorder/Recorder.cpp
	ui/About.cpp
	ui/EditSettings.cpp
	ui/FirstRun.cpp
	ui/License.cpp
	ui/Osd.cpp
	ui/TrayIcon.cpp
	ui/TrayRecorder.cpp
	xml/tanogenerator.cpp
	xml/tanohandler.cpp
)

SET(Tano_Srcs
	main.cpp
	Common.cpp
	MainWindow.cpp
	Updates.cpp
	
	${Tano_Class_Srcs}
)

SET (Tano_Files
    LICENSE
	ui/images/tano.png
	ui/images/tano-small.png
)

SET (Tano_Playlists
	playlists/siol-mpeg2.xml
	playlists/siol-mpeg4.xml
	playlists/t-2-cat.xml
	playlists/t-2-full.xml
)

# Set translation files
SET(Tano_Translation_Srcs 
	${CMAKE_CURRENT_SOURCE_DIR}/lang/tano_sl.ts
)
# lang/*.ts -> Tano_Translation_Srcs
SET ( Tano_Translation_Bins
	${CMAKE_CURRENT_SOURCE_DIR}/lang/tano_sl.qm
)	

################
# Qt4 settings #
###############
# This line adds the QtXML, QtNETWORK and QtWEBKIT libraries to the Qt libraries
# loaded when linking Tano
SET(QT_USE_QTXML True)
SET(QT_USE_QTNETWORK True)
SET(QT_USE_QTWEBKIT True)

# Add all resources to Tano_Resrcs (icons, html files etc.)
#SET(Tano_Resrcs ui/mainwin.qrc) # not needed, all resources are loaded on-the-fly

# Add all the dependencies needed for the MainWindow
# add_file_dependencies( ui/mainwin.cpp ui/ui_mainwin.h )

# This scripts runs the resource compiler rcc.
# The result of it is stored into Tano_Resrcs_Srcs
# variable. This has to be included later in the
# add_executable line.
QT4_ADD_RESOURCES(Tano_Resrcs_Srcs ${Tano_Resrcs})

# This scripts runs the user interface compiler uic.
# The result of it is stored into Tano_UIC_Srcs
# variable. This has to be included later in the
# add_executable line.
QT4_WRAP_UI(Tano_UIC_Srcs ${Tano_UIs})

# This script runs the meta object compiler moc.
# The result of it is stored into Tano_MOC_Srcs
# variable. This has to be included later in the
# add_executable line.
QT4_WRAP_CPP(Tano_MOC_Srcs ${Tano_MOCs})

#########################
# Compiler Define flags #
#########################
# RtMidi cross-platform definitions
IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux") #IF(LINUX) isn't supported by CMake yet
	ADD_DEFINITIONS(-D__LINUX_ALSASEQ__)
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

IF(MINGW)
 	ADD_DEFINITIONS(-D__WINDOWS_MM__)
ENDIF(MINGW)

# Define default data dir, if under POSIX systems.
IF(NOT MINGW)
	IF (NOT DEFAULT_DATA_DIR)
		ADD_DEFINITIONS(-DDEFAULT_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
	ELSE (NOT DEFAULT_DATA_DIR)
		ADD_DEFINITIONS(-DDEFAULT_DATA_DIR="${DEFAULT_DATA_DIR}")
	ENDIF (NOT DEFAULT_DATA_DIR)
ENDIF(NOT MINGW)


# Define Tano version
ADD_DEFINITIONS(-DTANO_VERSION="${TANO_VERSION}")
# Define Tano build
ADD_DEFINITIONS(-DTANO_BUILD="${TANO_BUILD}")
# Define Qt version
ADD_DEFINITIONS(-DTANO_QT_VERSION="${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")

####################################
# Include paths and set up targets #
####################################
# This needs to be added in order for #include to work inside .cpp files - it adds src/ to include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# This line adds the include path of the Qt
# directory to the compiler include path
INCLUDE(${QT_USE_FILE})


IF(MINGW)
	# CMake currently doesn't support windres yet. Add command manually
	FIND_PROGRAM(
		WINDRES_PROGRAM windres.exe PATHS
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/bin"
		c:/MinGW/bin /MinGW/bin
	)
	ADD_CUSTOM_COMMAND(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tano.obj
		COMMAND ${WINDRES_PROGRAM}
		-i ${CMAKE_CURRENT_SOURCE_DIR}/tano.rc
		-o ${CMAKE_CURRENT_BINARY_DIR}/tano.obj
	)
	SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_CURRENT_BINARY_DIR}/tano.obj)
ENDIF(MINGW)


# This line tells cmake to create the Tano program.
# Attention: In contrast to Makefiles don't add "\" to separate lines
ADD_EXECUTABLE(tano ${Tano_UIC_Srcs} ${Tano_Srcs} ${Tano_MOC_Srcs} ${Tano_Resrcs_Srcs})

# Here the needed Qt Libraries are added to the link
# command. Never remove that line :-)
TARGET_LINK_LIBRARIES(tano ${QT_LIBRARIES} ${VLC_LIBRARIES})


###############
# Translation #
###############
ADD_CUSTOM_COMMAND(
	TARGET tano
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	PRE_BUILD
	COMMAND ${QT_LUPDATE_EXECUTABLE}
	ARGS -noobsolete ${Tano_UIC_Srcs} ${Tano_Srcs} -ts ${Tano_Translation_Srcs}
)
ADD_CUSTOM_COMMAND(
	TARGET tano
	PRE_BUILD
	COMMAND ${QT_LRELEASE_EXECUTABLE}
	ARGS ${Tano_Translation_Srcs} -qm ${Tano_Translation_Bins}
)

################
# Installation #
################
# Install tano exe
INSTALL(TARGETS tano
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
)

# Install data - icons, images, sounds etc.
INSTALL( DIRECTORY ${Tano_Images} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}" PATTERN ".svn" EXCLUDE )
# Install data
INSTALL( FILES ${Tano_Files} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
# Install playlists
INSTALL( FILES ${Tano_Playlists} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/playlists")
# Install translations
INSTALL(FILES ${Tano_Translation_Bins} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/lang")

IF(MINGW)
	# Install Qt libs
	INSTALL( FILES ${QT_BIN_DIR}/QtCore4.dll ${QT_BIN_DIR}/QtGui4.dll ${QT_BIN_DIR}/QtNetwork4.dll ${QT_BIN_DIR}/QtWebkit4.dll ${QT_BIN_DIR}/QtXml4.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR} )
	INSTALL( FILES ${QT_PLUGINS_DIR}/imageformats/qgif4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qmng4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qjpeg4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qtiff4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qico4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qsvg4.dll
	  DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}/imageformats)
	
	# Install OpenSSL
	INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/win/3rdparty/openssl/libeay32.dll ${CMAKE_CURRENT_SOURCE_DIR}/win/3rdparty/openssl/ssleay32.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR} )
		
	# Install mingwm10.dll
	INSTALL( FILES ${MINGW_BIN_DIR}/mingwm10.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR} )

ENDIF(MINGW)