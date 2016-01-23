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
# Windows Extra #
#################
INSTALL(FILES ${VLCQtCoreLocation}/bin/VLCQtCore.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")
INSTALL(FILES ${VLCQtWidgetsLocation}/bin/VLCQtWidgets.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")

INSTALL(FILES ${VLCQtCoreLocation}/bin/libvlc.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")
INSTALL(FILES ${VLCQtCoreLocation}/bin/libvlccore.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")
INSTALL(DIRECTORY ${VLCQtCoreLocation}/bin/plugins/ DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins")

SET(WINSPARKLE_INCLUDE ${CMAKE_SOURCE_DIR}/platform/windows/winsparkle/include)
IF(BITS)
    SET(WINSPARKLE_LIBRARY ${CMAKE_SOURCE_DIR}/platform/windows/winsparkle/win64/WinSparkle.lib)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/platform/windows/winsparkle/win64/WinSparkle.dll
            DESTINATION "${CMAKE_INSTALL_PREFIX}")
ELSE()
    SET(WINSPARKLE_LIBRARY ${CMAKE_SOURCE_DIR}/platform/windows/winsparkle/win32/WinSparkle.lib)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/platform/windows/winsparkle/win32/WinSparkle.dll
            DESTINATION "${CMAKE_INSTALL_PREFIX}")
ENDIF()
