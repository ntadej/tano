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

IF(WITH_QT5)
    CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)
ELSE(WITH_QT5)
    CMAKE_MINIMUM_REQUIRED(VERSION 2.8.6)
ENDIF(WITH_QT5)

CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
)

INCLUDE(ManageString)

OPTION(CUSTOM_SETTINGS "Custom settings" OFF)
MESSAGE("Custom settings: ${CUSTOM_SETTINGS}")
OPTION(CORE_ONLY "Build only Tano Core" OFF)
MESSAGE("Build only Tano Core: ${CORE_ONLY}")


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
STRING_SPLIT(TANO_VERSION_LIST "." ${TANO_VERSION})
LIST(GET TANO_VERSION_LIST 0 TANO_VERSION_MAJOR)
LIST(GET TANO_VERSION_LIST 1 TANO_VERSION_MINOR)
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
# Automatically run moc, if required
SET(CMAKE_AUTOMOC ON)
# Verbose makefile
SET(CMAKE_VERBOSE_MAKEFILE ON)

# Show all warnings
IF(NOT MSVC)
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
ENDIF(NOT MSVC)

# Make final release build smaller
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(MINGW)
# Windows compatibility
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
# Statically link with libgcc
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
ELSEIF(MSVC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nologo -Zc:wchar_t")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nologo -Zc:wchar_t -w34100 -w34189")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NOLOGO /DYNAMICBASE /NXCOMPAT /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
ENDIF(MINGW)

# Set MingW bin dir, if MingW present
IF(MINGW)
    STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" MINGW_BIN_DIR_TMP "${CMAKE_CXX_COMPILER}")
    STRING(REGEX REPLACE "\\\\" "/" MINGW_BIN_DIR "${MINGW_BIN_DIR_TMP}") # Replace back slashes to slashes
ENDIF(MINGW)

# Qt5 support
OPTION(WITH_QT5 "Build with Qt5" OFF)
MESSAGE("Build with Qt5: ${WITH_QT5}")
IF(WITH_QT5)
    OPTION(WITH_QT_DUAL "System has Qt4 and Qt5 installed" OFF)
    MESSAGE("System has Qt4 and Qt5 installed: ${WITH_QT_DUAL}")
    IF(NOT CORE_ONLY)
        OPTION(BUILD_QML "Build Tano QML" OFF)
        MESSAGE("Build Tano QML: ${BUILD_QML}")
    ENDIF(NOT CORE_ONLY)
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
    FIND_PACKAGE(Qt5LinguistTools REQUIRED)
    FIND_PACKAGE(Qt5Widgets REQUIRED)

    IF(Qt5_POSITION_INDEPENDENT_CODE)
        SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
    ENDIF(Qt5_POSITION_INDEPENDENT_CODE)

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

    IF(WITH_QT_DUAL)
        SET(QT_MOC_EXECUTABLE "${QT_MOC_EXECUTABLE}-qt5")
        STRING(REGEX REPLACE "moc" "uic" Qt5Widgets_UIC_EXECUTABLE "${QT_MOC_EXECUTABLE}")
    ENDIF(WITH_QT_DUAL)
ELSE(WITH_QT5)
    FIND_PACKAGE(Qt4 REQUIRED)

    SET(QT_USE_QTNETWORK True)
    SET(QT_USE_QTXML True)

    ADD_DEFINITIONS(${QT_DEFINITIONS})
    INCLUDE(${QT_USE_FILE})
ENDIF(WITH_QT5)

IF(NOT CORE_ONLY)
    FIND_PACKAGE(LIBVLCQT REQUIRED)
ENDIF(NOT CORE_ONLY)

# Set Qt bin dir to find QtCoreX.dll and other libs to install
IF(MINGW OR MSVC)
    STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" QT_BIN_DIR_TMP "${QT_QMAKE_EXECUTABLE}")
    STRING(REGEX REPLACE "\\\\" "/" QT_BIN_DIR "${QT_BIN_DIR_TMP}")  # Replace back slashes to slashes
ENDIF(MINGW OR MSVC)

# Set default install path:
# In *nix environments this is /usr/local 
# Under M$ Windows this is C:\Program files\Tano
# WARNING: Leading slash should be added - this is for easier concatenation of the strings due to slash/backslash issues
IF(MINGW OR MSVC)
    IF(NOT CMAKE_INSTALL_PREFIX)
        SET(CMAKE_INSTALL_PREFIX "C:/Program files/Tano")
    ENDIF(NOT CMAKE_INSTALL_PREFIX)
    SET(TANO_INSTALL_DATA_DIR "")
    SET(TANO_INSTALL_BIN_DIR "")
ELSE(MINGW OR MSVC)
    IF (NOT CMAKE_INSTALL_PREFIX)
        SET(CMAKE_INSTALL_PREFIX "/usr/local")
    ENDIF (NOT CMAKE_INSTALL_PREFIX)
    SET(TANO_INSTALL_DATA_DIR "share/tano")
    SET(TANO_INSTALL_BIN_DIR "bin")

    IF(NOT DEFAULT_DATA_DIR)
        SET(DEFAULT_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    ENDIF(NOT DEFAULT_DATA_DIR)
ENDIF(MINGW OR MSVC)


#################
# Configuration #
#################
OPTION(REFRESH_TRANSLATIONS "Refresh translations on compile" OFF)
MESSAGE("Refresh translations on compile: ${REFRESH_TRANSLATIONS}")
OPTION(DISABLE_UPDATE_CHECK "Disable update checking" OFF)
MESSAGE("Disable update checking: ${DISABLE_UPDATE_CHECK}")
OPTION(DISABLE_TELETEXT "Disable teletext" OFF)
MESSAGE("Disable teletext: ${DISABLE_TELETEXT}")
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

IF(MSVC)
    SET(LIBSTART "")
ELSE(MSVC)
    SET(LIBSTART lib)
ENDIF(MSVC)

IF(MINGW)
    SET(LIBEND dll.a)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(LIBEND dylib)
ELSEIF(MSVC)
    SET(LIBEND lib)
ELSE(MINGW)
    SET(LIBEND so)
ENDIF(MINGW)

SET(TANO_CORE tanocore)
SET(TANO_CORE_LIB "${CMAKE_BINARY_DIR}/src/core/${LIBSTART}${TANO_CORE}.${LIBEND}")
SET(TANO_WIDGETS tanowidgets)
SET(TANO_WIDGETS_LIB "${CMAKE_BINARY_DIR}/src/widgets/${LIBSTART}${TANO_WIDGETS}.${LIBEND}")


###################
# Compile and set #
###################
IF(NOT CORE_ONLY)
    ADD_SUBDIRECTORY(lang)
ENDIF(NOT CORE_ONLY)

ADD_SUBDIRECTORY(src)

IF(MINGW OR MSVC)
    ADD_SUBDIRECTORY(windows)
ENDIF(MINGW OR MSVC)

SET(Tano_Icons
    data/icons/tano-default
)

SET(Tano_Desktop
    tano.desktop
)

SET(Tano_Logo
    tano.png
)

################
# Installation #
################
# Install data
IF(NOT CORE_ONLY)
    INSTALL(DIRECTORY ${Tano_Icons} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/icons")

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        INSTALL(FILES ${Tano_Desktop} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
        INSTALL(FILES ${Tano_Logo} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons")
    ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
ENDIF(NOT CORE_ONLY)
