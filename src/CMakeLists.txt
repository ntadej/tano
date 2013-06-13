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
# Set features depending on platform #
######################################

IF(BRANDING)
    FILE(READ ${CMAKE_SOURCE_DIR}/branding/FEATURES BRANDING_FEATURES)
    STRING_SPLIT(BRANDING_FEATURES "\n" ${BRANDING_FEATURES})
    FOREACH(F ${BRANDING_FEATURES})
        STRING_SPLIT(TMP_LIST " " ${F})
        LIST(GET TMP_LIST 0 TMP_ARG)
        LIST(GET TMP_LIST 1 TMP_VALUE)
        SET(${TMP_ARG} ${TMP_VALUE})
    ENDFOREACH(F)
ELSE(BRANDING)
    SET(FEATURE_RECORDER 1)
    SET(FEATURE_TELETEXT 1)
    SET(FEATURE_UPDATE 1)
ENDIF(BRANDING)

IF(CMAKE_SYSTEM_NAME EQUAL "Windows")
    SET(FEATURE_TELETEXT 0)
ENDIF(CMAKE_SYSTEM_NAME EQUAL "Windows")

#################
# Configuration #
#################
CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/src/Config.h.in
    ${CMAKE_BINARY_DIR}/src/Config.h
)
IF(BRANDING)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/branding/ExtraConfig.h.in
        ${CMAKE_BINARY_DIR}/branding/ExtraConfig.h
    )
ENDIF(BRANDING)

ADD_SUBDIRECTORY(core)

IF(BUILD_PLAYER OR BUILD_EDITOR)
    ADD_SUBDIRECTORY(widgets)
ENDIF(BUILD_PLAYER OR BUILD_EDITOR)

IF(BUILD_PLAYER)
    ADD_SUBDIRECTORY(player)
ENDIF(BUILD_PLAYER)

IF(BUILD_EDITOR)
    ADD_SUBDIRECTORY(editor)
ENDIF(BUILD_EDITOR)
