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
# Search for Qt and other libraries
# Qt 5
FIND_PACKAGE(Qt5Concurrent REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5LinguistTools REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)
FIND_PACKAGE(Qt5Xml REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    FIND_PACKAGE(Qt5MacExtras REQUIRED)
ENDIF()
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    FIND_PACKAGE(Qt5DBus REQUIRED)
ENDIF()

# Set Qt bin dir to find QtCoreX.dll and other libs to install
STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" QT_BIN_DIR_TMP "${QT_MOC_EXECUTABLE}")
STRING(REGEX REPLACE "\\\\" "/" QT_BIN_DIR "${QT_BIN_DIR_TMP}")  # Replace back slashes to slashes
STRING(REGEX REPLACE "bin" "lib" QT_LIB_DIR "${QT_BIN_DIR}")

# VLC-Qt
FIND_PACKAGE(VLCQt COMPONENTS Core Widgets REQUIRED)
