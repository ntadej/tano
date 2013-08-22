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
# OS X Extra #
##############
STRING(REGEX REPLACE "include" "" LIBVLCQT_DIR "${LIBVLCQT_INCLUDE_DIR}")
STRING(REGEX REPLACE "include" "lib" LIBVLCQT_LIB_DIR "${LIBVLCQT_INCLUDE_DIR}")

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/osx/deploy.sh.in
    ${CMAKE_BINARY_DIR}/osx/deploy.sh
)

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/osx/Info.plist.in
    ${CMAKE_BINARY_DIR}/osx/Info.plist
)

SET(Tano_Deploy
    ${CMAKE_BINARY_DIR}/osx/deploy.sh
)

SET(Tano_VlcQt_Libs
    ${LIBVLCQT_LIB_DIR}/libvlc-qt-widgets.dylib
    ${LIBVLCQT_LIB_DIR}/libvlc-qt.dylib
)

SET(Tano_Vlc_Libs
    ${LIBVLCQT_LIB_DIR}/libvlc.dylib
    ${LIBVLCQT_LIB_DIR}/libvlc.5.dylib
    ${LIBVLCQT_LIB_DIR}/libvlccore.dylib
    ${LIBVLCQT_LIB_DIR}/libvlccore.5.dylib
)

FILE(GLOB Tano_Vlc_Plugins ${LIBVLCQT_DIR}/plugins/*.dylib)

SET(Tano_Retina_Icons
    ${CMAKE_SOURCE_DIR}/data/icons/tano-default@2x
)


################
# Installation #
################
INSTALL(FILES ${Tano_Deploy} DESTINATION "${CMAKE_INSTALL_PREFIX}")
IF(NOT STATIC)
    INSTALL(FILES ${Tano_VlcQt_Libs} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_LIB_DIR}")
ENDIF(NOT STATIC)
INSTALL(FILES ${Tano_Vlc_Libs} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_LIB_DIR}")

INSTALL(FILES ${Tano_Vlc_Plugins} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}/plugins")

INSTALL(DIRECTORY ${Tano_Retina_Icons} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/icons")
