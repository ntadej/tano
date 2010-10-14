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
# The name of our project is "Tano". CMakeLists files in this project can
# refer to the root source directory of the project as ${TANO_SOURCE_DIR}
# and to the root binary directory of the project as ${TANO_BINARY_DIR}.

PROJECT(Tano)
cmake_minimum_required(VERSION 2.6)

CONFIGURE_FILE(
  "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

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

# Search for Qt4 and other libraries
SET(QT_MIN_VERSION "4.5.0")

FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(LIBVLCQT REQUIRED)
# MinGW needs some more packages
IF(MINGW)
	FIND_PACKAGE(LIBVLC REQUIRED)
ENDIF(MINGW)

# Set Qt4 bin dir to find QtCore4.dll and other libs to install
STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" QT_BIN_DIR_TMP "${QT_QMAKE_EXECUTABLE}")
STRING(REGEX REPLACE "\\\\" "/" QT_BIN_DIR "${QT_BIN_DIR_TMP}")  # Replace back slashes to slashes

# Set default install path:
# In *nix environments this is /usr/local 
# Under M$ Windows this is C:\Program files\Tano
# WARNING: Leading slash should be added - this is for easier concatenation of the strings due to slash/backslash issues
IF(MINGW)
	IF (NOT CMAKE_INSTALL_PREFIX)
		SET(CMAKE_INSTALL_PREFIX "C:/Program files/Tano")
	ENDIF (NOT CMAKE_INSTALL_PREFIX)
	SET(TANO_INSTALL_DATA_DIR "")
	SET(TANO_INSTALL_PLUGIN_DIR "plugins")
	SET(TANO_INSTALL_BIN_DIR "")
ELSE(MINGW)
	IF (NOT CMAKE_INSTALL_PREFIX)
		SET(CMAKE_INSTALL_PREFIX "/usr/local")
	ENDIF (NOT CMAKE_INSTALL_PREFIX)
	SET(TANO_INSTALL_DATA_DIR "share/tano")
	SET(TANO_INSTALL_PLUGIN_DIR "lib/tano")
	SET(TANO_INSTALL_BIN_DIR "bin")
ENDIF(MINGW)


###########
# Version #
###########
# Tano version number.
FILE(READ VERSION TANO_VERSION)
STRING(REGEX REPLACE "\n" "" TANO_VERSION "${TANO_VERSION}") # get rid of the newline at the end
MESSAGE("You are compiling Tano ${TANO_VERSION}")

# Find Git Version Patch
FIND_PROGRAM(GIT git)
IF(GIT)
	EXECUTE_PROCESS(
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMAND ${GIT} rev-parse --short HEAD
		OUTPUT_VARIABLE GIT_OUT OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	SET(PROJECT_VERSION_PATCH "${GIT_OUT}-git")
	MESSAGE(STATUS "Git Version Patch: ${GIT_OUT}")
ELSE(GIT)
	SET(GIT_OUT 0)
ENDIF(GIT)

# Qt version check
IF(QT_VERSION_MINOR MATCHES 5)
    SET(QT_OLD 1)
ELSE(QT_VERSION_MINOR MATCHES 5)
    SET(QT_OLD 0)
ENDIF(QT_VERSION_MINOR MATCHES 5)


#################
# Configuration #
#################
OPTION(REFRESH_TRANSLATIONS "Refresh translations on compile")
MESSAGE("Refresh translations on compile: ${REFRESH_TRANSLATIONS}")


###################
# Compile and set #
###################
ADD_SUBDIRECTORY(lang)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(plugins)

SET(Tano_Files
	LICENSE.GPL
	data/logo/64x64/logo.png
)

FILE(GLOB Tano_Playlists ${CMAKE_SOURCE_DIR}/playlists/*.m3u)

# Windows
IF(MINGW)
	STRING(REGEX REPLACE "include" "bin" LIBVLCQT_BIN_DIR "${LIBVLCQT_INCLUDE_DIR}")
	STRING(REGEX REPLACE "sdk/include" "" LIBVLC_BIN_DIR "${LIBVLC_INCLUDE_DIR}")
	SET(Tano_Qt
		${QT_BIN_DIR}/libgcc_s_dw2-1.dll
		${QT_BIN_DIR}/mingwm10.dll
		${QT_BIN_DIR}/QtCore4.dll
		${QT_BIN_DIR}/QtGui4.dll
		${QT_BIN_DIR}/QtNetwork4.dll
		${QT_BIN_DIR}/QtSvg4.dll
		${QT_BIN_DIR}/QtXml4.dll
	)
	SET(Tano_Libs
		${LIBVLCQT_BIN_DIR}/libvlc-qt.dll
		${LIBVLC_BIN_DIR}/libvlc.dll
		${LIBVLC_BIN_DIR}/libvlccore.dll
	)
	FILE(GLOB Tano_Vlc_Plugins ${LIBVLC_BIN_DIR}/plugins/*.dll)
ENDIF(MINGW)

################
# Installation #
################
# Install data
INSTALL(FILES ${Tano_Files} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
# Install playlists
INSTALL(FILES ${Tano_Playlists} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/playlists")

# Windows Extra
IF(MINGW)
	INSTALL(FILES ${Tano_Qt} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
	INSTALL(FILES ${Tano_Libs} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
	INSTALL(FILES ${Tano_Vlc_Plugins} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/vlc/plugins")
ENDIF(MINGW)
