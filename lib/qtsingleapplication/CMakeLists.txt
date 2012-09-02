#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

PROJECT(qtsingleapplication)

# Define the C++ source files used by QtSingleApplication
SET(QtSA_Srcs
    src/qtlocalpeer.cpp
    src/qtsingleapplication.cpp
    src/qtsinglecoreapplication.cpp
)

# Qt definitions
ADD_DEFINITIONS(-DQT_STATIC)

# Create library
ADD_LIBRARY(qtsingleapplication STATIC ${QtSA_Srcs} ${QtSA_MOCs_Srcs})

IF(WITH_QT5)
    QT5_USE_MODULES(qtsingleapplication Network Widgets)
ELSE(WITH_QT5)
    TARGET_LINK_LIBRARIES(qtsingleapplication)
ENDIF(WITH_QT5)
