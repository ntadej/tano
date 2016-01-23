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
IF(NOT CMAKE_INSTALL_PREFIX)
    SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF()
SET(PROJECT_INSTALL_DATA_DIR "share/${PROJECT_NAME_LOWER}")
SET(PROJECT_INSTALL_BIN_DIR "bin")
SET(PROJECT_INSTALL_LIB_DIR "lib")

SET(PROJECT_LOGO "${CMAKE_SOURCE_DIR}/${PROJECT_NAME_LOWER}/${PROJECT_NAME_LOWER}.png")
SET(PROJECT_DESKTOP "${CMAKE_SOURCE_DIR}/${PROJECT_NAME_LOWER}/${PROJECT_NAME_LOWER}.desktop")


######################
# Extra installation #
######################
INSTALL(FILES ${PROJECT_LOGO} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pixmaps")
INSTALL(FILES ${PROJECT_DESKTOP} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
