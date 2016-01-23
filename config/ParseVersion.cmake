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
FILE(READ VERSION VERSION_FROM_FILE)
STRING(REGEX REPLACE "\n" "" VERSION_FROM_FILE "${VERSION_FROM_FILE}") # get rid of the newline at the end

IF(EXISTS ${PROJECT_NAME_LOWER}/VERSION)
    FILE(READ ${PROJECT_NAME_LOWER}/VERSION VERSION_APP_FROM_FILE)
    STRING(REGEX REPLACE "\n" "" VERSION_APP_FROM_FILE "${VERSION_APP_FROM_FILE}") # get rid of the newline at the end

    MESSAGE("You are compiling Tano Core ${VERSION_FROM_FILE} and ${BRANDING} ${VERSION_APP_FROM_FILE}")
ELSE()
    SET(VERSION_APP_FROM_FILE ${VERSION_FROM_FILE})
    MESSAGE("You are compiling ${BRANDING} ${VERSION_FROM_FILE}")
ENDIF()

# Find Git Version Patch
FIND_PROGRAM(GIT git)
IF(GIT AND NOT NO_GIT)
    EXECUTE_PROCESS(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GIT} rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_OUT OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    IF(NOT GIT_OUT STREQUAL "")
        SET(PROJECT_VERSION_VCS "${GIT_OUT}-git")
        MESSAGE(STATUS "Git Version Patch: ${GIT_OUT}")
    ENDIF()

    EXECUTE_PROCESS(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GIT} rev-list --tags --no-walk --max-count=1
        OUTPUT_VARIABLE GIT_TAG_OUT OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    EXECUTE_PROCESS(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GIT} rev-list ${GIT_TAG_OUT}..HEAD --count
        OUTPUT_VARIABLE GIT_BUILD_OUT OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    SET(PROJECT_VERSION_BUILD "${GIT_BUILD_OUT}")
    MESSAGE("Git Version Build: ${GIT_BUILD_OUT}")
ELSEIF(GITBUILD)
    SET(PROJECT_VERSION_VCS "${GITBUILD}")
    SET(GIT_BUILD_OUT 0)
ELSE()
    SET(GIT_OUT 0)
    SET(GIT_BUILD_OUT 0)
ENDIF()
