#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
CMAKE_POLICY(VERSION 3.0.2)

SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/platform
)

IF(NOT DEFINED BRANDING OR BRANDING STREQUAL "")
    SET(BRANDING Tano)
ENDIF()
STRING(TOLOWER ${BRANDING} PROJECT_NAME_LOWER)

INCLUDE(ParseVersion)

PROJECT(${BRANDING}
        VERSION ${VERSION_FROM_FILE})

SET(PROJECT_AUTHOR "Tadej Novak")
SET(PROJECT_COPYRIGHT "Copyright (C) 2015 ${PROJECT_AUTHOR}.")
SET(PROJECT_DESCRIPTION "Tano - An Open IP TV Player")

INCLUDE(CustomTargets)
INCLUDE(ApplicationConfig)
INCLUDE(CompilerConfig)
INCLUDE(Dependencies)

# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
# the application bundle
SET(MACOSX_BUNDLE_COPYRIGHT "2014 Tadej Novak")
SET(MACOSX_BUNDLE_ICON_FILE "${PROJECT_NAME}.icns")
SET(MACOSX_BUNDLE_GUI_IDENTIFIER "si.tano.${PROJECT_NAME_LOWER}")


#################
# Configuration #
#################
IF(MSVC)
    SET(LIBSTART "")
ELSE()
    SET(LIBSTART lib)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(LIBEND dylib)
ELSEIF(MSVC)
    SET(LIBEND lib)
ELSE()
    SET(LIBEND so)
ENDIF()

SET(TANO_CORE TanoCore)
SET(TANO_WIDGETS TanoWidgets)
SET(TANO_CONFIG ConfigPlugin)

SET(TANO_CORE_LIB "${CMAKE_BINARY_DIR}/src/core/${LIBSTART}${TANO_CORE}.${LIBEND}")
SET(TANO_WIDGETS_LIB "${CMAKE_BINARY_DIR}/src/widgets/${LIBSTART}${TANO_WIDGETS}.${LIBEND}")
IF(MSVC)
    SET(TANO_CONFIG_LIB "${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}/config/${LIBSTART}${TANO_CONFIG}.lib")
ELSE()
    SET(TANO_CONFIG_LIB "${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}/config/${LIBSTART}${TANO_CONFIG}.a")
ENDIF()


###################
# Compile and SET #
###################
IF(MSVC)
    INCLUDE(Windows)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    INCLUDE(OSX)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    INCLUDE(Linux)
ENDIF()

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(${PROJECT_NAME_LOWER})
ADD_SUBDIRECTORY(i18n)
