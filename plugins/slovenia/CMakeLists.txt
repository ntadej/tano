#############################################################################
# Slovenian EPG - Tano EPG Plugin
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

PROJECT(tano-epgslovenia)

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
    ${QT_INCLUDE_DIR}
    ../
)

SET(SOURCES_CPP
    SloveniaCreator.cpp
    Slovenia.cpp
)

SET(SOURCES_H
    SloveniaCreator.h
)

QT4_WRAP_CPP(SOURCES_H_MOC ${SOURCES_H})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
#ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

ADD_LIBRARY(tano-epgslovenia SHARED ${SOURCES_CPP} ${SOURCES_H_MOC})
ADD_DEPENDENCIES(tano-epgslovenia tano)

TARGET_LINK_LIBRARIES(tano-epgslovenia ${QT_LIBRARIES})

IF(MINGW)
    INSTALL(TARGETS tano-epgslovenia DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR})
ELSE(MINGW)
    INSTALL(TARGETS tano-epgslovenia LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR})
ENDIF(MINGW)
