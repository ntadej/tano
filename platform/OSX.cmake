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
# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
# the application bundle
SET(MACOSX_BUNDLE_COPYRIGHT "2015 Tadej Novak")
SET(MACOSX_BUNDLE_ICON_FILE "${PROJECT_NAME}.icns")
SET(MACOSX_BUNDLE_GUI_IDENTIFIER "si.tano.${PROJECT_NAME_LOWER}")

IF(NOT CMAKE_INSTALL_PREFIX)
    SET(CMAKE_INSTALL_PREFIX "/Applications")
ENDIF()
SET(PROJECT_INSTALL_DATA_DIR "${PROJECT_NAME}.app/Contents/Resources")
SET(PROJECT_INSTALL_BIN_DIR "${PROJECT_NAME}.app/Contents/MacOS")
SET(PROJECT_INSTALL_LIB_DIR "${PROJECT_NAME}.app/Contents/MacOS/lib")

# Support OS X 10.6 or later (64-bit only)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(CMAKE_MACOSX_RPATH ON)
    SET(CMAKE_INSTALL_RPATH "${QT_LIB_DIR};@executable_path/../Frameworks;@executable_path/lib")
    SET(CMAKE_OSX_ARCHITECTURES x86_64)
ENDIF()

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/platform/osx/Info.plist.in
    ${CMAKE_BINARY_DIR}/platform/osx/Info.plist
)

INSTALL(DIRECTORY ${VLCQtCoreFramework} DESTINATION "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app/Contents/Frameworks")
INSTALL(DIRECTORY ${VLCQtWidgetsFramework} DESTINATION "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app/Contents/Frameworks")
INSTALL(DIRECTORY ${SPARKLE_LIBRARY} DESTINATION "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app/Contents/Frameworks")

ADD_CUSTOM_TARGET(dmg
    "${QT_BIN_DIR}/macdeployqt"
    "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app"
    -dmg
    WORKING_DIRECTORY ${QT_BIN_DIR})
