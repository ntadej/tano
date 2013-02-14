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
# Configuration #
#################
CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/src/Config.h.in
    ${CMAKE_BINARY_DIR}/src/Config.h
)


ADD_SUBDIRECTORY(core)

IF(NOT CORE_ONLY)
    ADD_SUBDIRECTORY(widgets)
    ADD_SUBDIRECTORY(desktop)

    IF(QT5)
        IF(EXISTS ${CMAKE_SOURCE_DIR}/src/qml/ui AND BUILD_QML)
            ADD_SUBDIRECTORY(qml)
        ENDIF(EXISTS ${CMAKE_SOURCE_DIR}/src/qml/ui AND BUILD_QML)
    ENDIF(QT5)
ENDIF(NOT CORE_ONLY)
