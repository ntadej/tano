#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

PROJECT(tano)
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

INCLUDE(ManageString)

OPTION(CUSTOM_SETTINGS "Custom settings" OFF)
MESSAGE("Custom settings: ${CUSTOM_SETTINGS}")

###########
# Version #
###########
# Tano version number.
IF(CUSTOM_SETTINGS)
    FILE(READ src/custom/VERSION TANO_VERSION)
ELSE(CUSTOM_SETTINGS)
    FILE(READ VERSION TANO_VERSION)
ENDIF(CUSTOM_SETTINGS)
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
    SET(PROJECT_VERSION_PATCH "${GIT_OUT}")
    MESSAGE("Git Version Patch: ${GIT_OUT}")
ELSE(GIT)
    IF(GITBUILD)
        SET(PROJECT_VERSION_PATCH "${GITBUILD}")
    ELSE(GITBUILD)
        SET(GIT_OUT 0)
    ENDIF(GITBUILD)
ENDIF(GIT)

MESSAGE("\n")

# Find the architecture
SET(BITS 0)
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(BITS 1)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)


####################
# Compiler options #
####################
# Set MingW bin dir, if MingW present
IF(MINGW)
    STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" MINGW_BIN_DIR_TMP "${CMAKE_CXX_COMPILER}")
    STRING(REGEX REPLACE "\\\\" "/" MINGW_BIN_DIR "${MINGW_BIN_DIR_TMP}") # Replace back slashes to slashes
ENDIF(MINGW)

# Show all warnings
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -W")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -W")

SET(CMAKE_VERBOSE_MAKEFILE ON)

IF(MINGW)
# Windows compatibility
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
# Statically link with libgcc
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
ENDIF(MINGW)

# Qt5 support
OPTION(WITH_QT5 "Build with Qt5" OFF)
MESSAGE("Build with Qt5: ${WITH_QT5}")

IF(WITH_QT5)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

    # Qt5 dirs
    INCLUDE(Qt5)
    QT5_FIND_LIBRARY_DIR(QT_LIBRARY_DIR)

    SET(Qt5Concurrent_DIR "${QT_LIBRARY_DIR}/cmake/Qt5Concurrent")
    SET(Qt5Core_DIR "${QT_LIBRARY_DIR}/cmake/Qt5Core")
    SET(Qt5Gui_DIR "${QT_LIBRARY_DIR}/cmake/Qt5Gui")
    SET(Qt5Network_DIR "${QT_LIBRARY_DIR}/cmake/Qt5Network")
    SET(Qt5PrintSupport_DIR "${QT_LIBRARY_DIR}/cmake/Qt5PrintSupport")
    SET(Qt5Widgets_DIR "${QT_LIBRARY_DIR}/cmake/Qt5Widgets")
    SET(Qt5Xml_DIR "${QT_LIBRARY_DIR}/cmake/Qt5Xml")
ENDIF(WITH_QT5)

# Search for Qt and other libraries
IF(WITH_QT5)
    SET(QT_MIN_VERSION "5.0.0")
    ADD_DEFINITIONS(-DQt5)
ELSE(WITH_QT5)
    SET(QT_MIN_VERSION "4.8.0")
    ADD_DEFINITIONS(-DQt4)
ENDIF(WITH_QT5)

# Qt
IF(WITH_QT5)
    FIND_PACKAGE(Qt5Concurrent REQUIRED)
    FIND_PACKAGE(Qt5Core REQUIRED)
    FIND_PACKAGE(Qt5Gui REQUIRED)
    FIND_PACKAGE(Qt5Network REQUIRED)
    FIND_PACKAGE(Qt5PrintSupport REQUIRED)
    FIND_PACKAGE(Qt5Widgets REQUIRED)
    FIND_PACKAGE(Qt5Xml REQUIRED)
ELSE(WITH_QT5)
    FIND_PACKAGE(Qt4 REQUIRED)
ENDIF(WITH_QT5)

FIND_PACKAGE(LIBVLCQT REQUIRED)

# Set Qt bin dir to find QtCoreX.dll and other libs to install
IF(MINGW)
    STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" QT_BIN_DIR_TMP "${QT_QMAKE_EXECUTABLE}")
    STRING(REGEX REPLACE "\\\\" "/" QT_BIN_DIR "${QT_BIN_DIR_TMP}")  # Replace back slashes to slashes
ENDIF(MINGW)

# Set default install path:
# In *nix environments this is /usr/local 
# Under M$ Windows this is C:\Program files\Tano
# WARNING: Leading slash should be added - this is for easier concatenation of the strings due to slash/backslash issues
IF(MINGW)
    IF(NOT CMAKE_INSTALL_PREFIX)
        SET(CMAKE_INSTALL_PREFIX "C:/Program files/Tano")
    ENDIF(NOT CMAKE_INSTALL_PREFIX)
    SET(TANO_INSTALL_DATA_DIR "")
    SET(TANO_INSTALL_BIN_DIR "")
ELSE(MINGW)
    IF (NOT CMAKE_INSTALL_PREFIX)
        SET(CMAKE_INSTALL_PREFIX "/usr/local")
    ENDIF (NOT CMAKE_INSTALL_PREFIX)
    SET(TANO_INSTALL_DATA_DIR "share/tano")
    SET(TANO_INSTALL_BIN_DIR "bin")

    IF(NOT DEFAULT_DATA_DIR)
        ADD_DEFINITIONS(-DDEFAULT_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    ELSE(NOT DEFAULT_DATA_DIR)
        ADD_DEFINITIONS(-DDEFAULT_DATA_DIR="${DEFAULT_DATA_DIR}")
    ENDIF(NOT DEFAULT_DATA_DIR)
ENDIF(MINGW)


#################
# Configuration #
#################
OPTION(REFRESH_TRANSLATIONS "Refresh translations on compile" OFF)
MESSAGE("Refresh translations on compile: ${REFRESH_TRANSLATIONS}")
OPTION(DISABLE_UPDATE_CHECK "Disable update checking" OFF)
MESSAGE("Disable update checking: ${DISABLE_UPDATE_CHECK}")
OPTION(DISABLE_TELETEXT "Disable teletext" OFF)
MESSAGE("Disable teletext: ${DISABLE_TELETEXT}")
IF(MINGW)
    OPTION(WINDOWS_INSTALLER "Enable Windows installer" ON)
    MESSAGE("Enable Windows installer: ${WINDOWS_INSTALLER}")
ENDIF(MINGW)
MESSAGE("\n")

IF(DISABLE_UPDATE_CHECK)
    SET(UPDATE 0)
ELSE(DISABLE_UPDATE_CHECK)
    SET(UPDATE 1)
ENDIF(DISABLE_UPDATE_CHECK)

IF(DISABLE_TELETEXT)
    SET(TELETEXT 0)
ELSE(DISABLE_TELETEXT)
    SET(TELETEXT 1)
ENDIF(DISABLE_TELETEXT)

SET(QT_SINGLE_APPLICATION "${CMAKE_BINARY_DIR}/lib/qtsingleapplication/libqtsingleapplication.a")


###################
# Compile and set #
###################
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(lang)
ADD_SUBDIRECTORY(src)
IF(MINGW)
    ADD_SUBDIRECTORY(windows)
ENDIF(MINGW)

SET(Tano_Files
    LICENSE.GPL
    data/logo/64x64/logo.png
)

SET(Tano_Icons
    data/icons/tano-default
)

################
# Installation #
################
# Install data
INSTALL(DIRECTORY ${Tano_Icons} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/icons")
IF(NOT MINGW)
    INSTALL(FILES ${Tano_Files} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
ENDIF(NOT MINGW)
