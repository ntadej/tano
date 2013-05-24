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

PROJECT(tano)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

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

OPTION(BUILD_DESKTOP "Build Tano Desktop" ON)
MESSAGE("Build Tano Desktop: ${BUILD_DESKTOP}")
OPTION(BRANDING "Enable branding" OFF)
MESSAGE("Enable branding: ${BRANDING}")
IF(BRANDING)
    SET(BRANDING_SET 1)
ELSE(BRANDING)
    SET(BRANDING_SET 0)
ENDIF(BRANDING)

IF(ENCRYPTION_KEY STREQUAL "" OR NOT DEFINED ENCRYPTION_KEY)
    SET(ENCRYPTION_KEY 0x0)
ENDIF(ENCRYPTION_KEY STREQUAL "" OR NOT DEFINED ENCRYPTION_KEY)

###########
# Version #
###########
# Tano version number.
IF(BRANDING)
    FILE(READ ${CMAKE_SOURCE_DIR}/branding/VERSION TANO_VERSION)
ELSE(BRANDING)
    FILE(READ ${CMAKE_SOURCE_DIR}/VERSION TANO_VERSION)
ENDIF(BRANDING)
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
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
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

# Search for Qt and other libraries
# Qt
SET(QT_MIN_VERSION "4.8.0")
FIND_PACKAGE(Qt4)
IF(NOT QT4_FOUND OR QT_VERSION_MAJOR MATCHES 5)
    FIND_PACKAGE(Qt5LinguistTools REQUIRED)
    FIND_PACKAGE(Qt5Widgets REQUIRED)
    SET(QT5 1)
    ADD_DEFINITIONS(-DQt5)
    MESSAGE("Build with Qt5")
ELSE(NOT QT4_FOUND OR QT_VERSION_MAJOR MATCHES 5)
    SET(QT5 0)
    ADD_DEFINITIONS(-DQt4)
    MESSAGE("Build with Qt4")
ENDIF(NOT QT4_FOUND OR QT_VERSION_MAJOR MATCHES 5)

IF(QT5)
#   Add compiler flags for building executables (-fPIE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

    IF(Qt5_POSITION_INDEPENDENT_CODE)
        SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
    ENDIF(Qt5_POSITION_INDEPENDENT_CODE)
ELSE(QT5)
    SET(QT_USE_QTNETWORK True)
    SET(QT_USE_QTSQL True)
    SET(QT_USE_QTXML True)

    ADD_DEFINITIONS(${QT_DEFINITIONS})
    INCLUDE(${QT_USE_FILE})
ENDIF(QT5)

IF(BUILD_DESKTOP)
    FIND_PACKAGE(LIBVLCQT REQUIRED)
ENDIF(BUILD_DESKTOP)

# Set Qt bin dir to find QtCoreX.dll and other libs to install
IF(MINGW OR MSVC)
    STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" QT_BIN_DIR_TMP "${QT_MOC_EXECUTABLE}")
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
MESSAGE("\n")

IF(MSVC)
    SET(LIBSTART "")
ELSE(MSVC)
    SET(LIBSTART lib)
ENDIF(MSVC)

IF(STATIC)
    SET(LIBEND a)
ELSEIF(MINGW)
    SET(LIBEND dll.a)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(LIBEND dylib)
ELSEIF(MSVC)
    SET(LIBEND lib)
ELSE(STATIC)
    SET(LIBEND so)
ENDIF(STATIC)

SET(TANO_CORE tanocore)
SET(TANO_CORE_LIB "${CMAKE_BINARY_DIR}/src/core/${LIBSTART}${TANO_CORE}.${LIBEND}")
SET(TANO_WIDGETS tanowidgets)
SET(TANO_WIDGETS_LIB "${CMAKE_BINARY_DIR}/src/widgets/${LIBSTART}${TANO_WIDGETS}.${LIBEND}")


###################
# Compile and set #
###################
IF(BUILD_DESKTOP)
    ADD_SUBDIRECTORY(lang)
ENDIF(BUILD_DESKTOP)

ADD_SUBDIRECTORY(src)

IF(MINGW OR MSVC)
    ADD_SUBDIRECTORY(windows)
ENDIF(MINGW OR MSVC)

ADD_SUBDIRECTORY(doc)

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
IF(BUILD_DESKTOP)
    INSTALL(DIRECTORY ${Tano_Icons} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/icons")

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        INSTALL(FILES ${Tano_Desktop} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
        INSTALL(FILES ${Tano_Logo} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons")
    ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
ENDIF(BUILD_DESKTOP)
