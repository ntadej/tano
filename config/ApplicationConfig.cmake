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

OPTION(LOGGING "Enable logging" ON)
MESSAGE("Enable logging: ${LOGGING}")

IF(LOGGING)
    SET(LOGGING_SET 1)
ELSE()
    SET(LOGGING_SET 0)
ENDIF()


OPTION(RELEASE "Enable release" OFF)
MESSAGE("Enable release: ${RELEASE}")

IF(RELEASE)
    SET(RELEASE_SET 1)
ELSE()
    SET(RELEASE_SET 0)
ENDIF()


OPTION(REFRESH_TRANSLATIONS "Refresh translations" ON)
MESSAGE("Refresh translations: ${REFRESH_TRANSLATIONS}")
