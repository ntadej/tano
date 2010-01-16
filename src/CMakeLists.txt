SET(MAIN_SOURCE_DIR "../")

IF(MINGW)
add_definitions(-DCMAKE_TOOLCHAIN=../Toolchain-win32.cmake)
find_package(VLC REQUIRED)
if (VLC_FOUND)
	include_directories(${VLC_INCLUDE_DIRS})
endif (VLC_FOUND)
ENDIF(MINGW)

# Search for Qt4. We need the latest version of Qt
SET(QT_MIN_VERSION "4.4.0")

# this line includes FindQt4.cmake, which searches the Qt library and headers
FIND_PACKAGE(Qt4 REQUIRED)

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
	ui/EpgShow.ui
	ui/InfoBar.ui
	ui/MainWindow.ui
	ui/Osd.ui
	ui/PlaylistWidget.ui
	ui/Recorder.ui
	ui/TimersManager.ui
)

# Define the MOC source files used by Tano
SET(Tano_MOCs	# List of classes which Qt needs to produce MOCs for them (in order for events, signals, slots and other macros to work
	MainWindow.h
	Updates.h
	channels/EditPlaylist.h
	control/ChannelSelect.h
	control/Shortcuts.h
	epg/Epg.h
	epg/EpgList.h
	epg/EpgLoader.h
	epg/EpgShow.h
	recorder/Recorder.h
	recorder/Timer.h
	recorder/TimersManager.h
	ui/About.h
	ui/EditSettings.h
	ui/FirstRun.h
	ui/InfoBar.h
	ui/License.h
	ui/Osd.h
	ui/PlaylistWidget.h
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
	epg/EpgList.cpp
	epg/EpgLoader.cpp
	epg/EpgShow.cpp
	recorder/Recorder.cpp
	recorder/Timer.cpp
	recorder/TimersManager.cpp
	ui/About.cpp
	ui/EditSettings.cpp
	ui/FirstRun.cpp
	ui/InfoBar.cpp
	ui/License.cpp
	ui/Osd.cpp
	ui/PlaylistWidget.cpp
	ui/TrayIcon.cpp
	ui/TrayRecorder.cpp
	xml/M3UGenerator.cpp
	xml/M3UHandler.cpp
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

# Set translation files
SET(Tano_Translation_Srcs 
	lang/tano_sl.ts
)
# lang/*.ts -> Tano_Translation_Srcs
SET ( Tano_Translation_Bins
	lang/tano_sl.qm
)	

################
# Qt4 settings #
###############
# This line adds the QtXML and QtNETWORK libraries to the Qt libraries
# loaded when linking Tano
SET(QT_USE_QTXML True)
SET(QT_USE_QTNETWORK True)

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
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} "${MAIN_SOURCE_DIR}/lib")

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
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_CURRENT_BINARY_DIR}/tano.obj)
ENDIF(MINGW)


# This line tells cmake to create the Tano program.
# Attention: In contrast to Makefiles don't add "\" to separate lines
ADD_EXECUTABLE(tano ${Tano_UIC_Srcs} ${Tano_Srcs} ${Tano_MOC_Srcs} ${Tano_Resrcs_Srcs})

# Here the needed Qt Libraries are added to the link
# command. Never remove that line :-)
TARGET_LINK_LIBRARIES(tano ${QT_LIBRARIES} QVlc QtKeySequenceEdit)


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
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
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
# Install translations
INSTALL(FILES ${Tano_Translation_Bins} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/lang")

IF(MINGW)
	# Install Qt libs
	INSTALL( FILES ${QT_BIN_DIR}/QtCore4.dll ${QT_BIN_DIR}/QtGui4.dll ${QT_BIN_DIR}/QtNetwork4.dll ${QT_BIN_DIR}/QtSvg4.dll ${QT_BIN_DIR}/QtXml4.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR} )
	INSTALL( FILES ${QT_PLUGINS_DIR}/imageformats/qgif4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qmng4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qjpeg4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qtiff4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qico4.dll
	    ${QT_PLUGINS_DIR}/imageformats/qsvg4.dll
	  DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}/imageformats)
	
	# Install OpenSSL
	INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/win/3rdparty/openssl/libeay32.dll ${CMAKE_CURRENT_SOURCE_DIR}/win/3rdparty/openssl/ssleay32.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR} )
ENDIF(MINGW)