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

CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/platform
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/widgets
)

IF(NOT DEFINED BRANDING OR BRANDING STREQUAL "")
    SET(BRANDING Tano)
ENDIF()
STRING(TOLOWER ${BRANDING} PROJECT_NAME_LOWER)

INCLUDE(ParseVersion)

PROJECT(${BRANDING}
        VERSION ${VERSION_APP_FROM_FILE})

SET(PROJECT_AUTHOR "Tadej Novak")
SET(PROJECT_COPYRIGHT "Copyright (C) 2016 ${PROJECT_AUTHOR}.")
SET(PROJECT_DESCRIPTION "Tano - An Open IP TV Player")
SET(PROJECT_URL "http://player.tano.si")
SET(PROJECT_SUPPORT "http://player.tano.si")
SET(PROJECT_UPDATE_XML "https://cdn-direct.tano.si/sparkle/tano.xml")

INCLUDE(CustomTargets)
INCLUDE(ApplicationConfig)
INCLUDE(CompilerConfig)
INCLUDE(Dependencies)


#################
# Configuration #
#################
IF(MSVC)
    SET(LIBSTART "")
    SET(LIBEND lib)
ELSE()
    SET(LIBSTART lib)
    SET(LIBEND a)
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
