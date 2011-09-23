#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})

SET(QTSA_SOURCES_CPP
    src/qtlocalpeer.cpp
    src/qtsingleapplication.cpp
    src/qtsinglecoreapplication.cpp
)

SET(QTSA_SOURCES_H
    src/qtlocalpeer.h
    src/qtsingleapplication.h
    src/qtsinglecoreapplication.h
)

QT4_WRAP_CPP(QTSA_SOURCES_H_MOC ${QTSA_SOURCES_H})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_STATIC)

ADD_LIBRARY(qtsingleapplication STATIC ${QTSA_SOURCES_CPP} ${QTSA_SOURCES_H_MOC})

TARGET_LINK_LIBRARIES(qtsingleapplication)

SET(QT_SINGLE_APPLICATION "${CMAKE_BINARY_DIR}/lib/qtsingleapplication/src/libqtsingleapplication.a")
