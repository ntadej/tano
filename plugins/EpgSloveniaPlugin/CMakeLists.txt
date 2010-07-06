#############################################################################
# Tano - EpgSloveniaPlugin
#############################################################################
# Copyright (C) 2008-2010 Tadej Novak
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# This file may be used under the terms of the
# GNU General Public License version 3.0 as published by the
# Free Software Foundation and appearing in the file LICENSE.GPL
# included in the packaging of this file.
#############################################################################

PROJECT(EpgSloveniaPlugin)

SET(QT_USE_QTNETWORK True)

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
	${QT_INCLUDE_DIR}
	../../src
)

SET(SOURCES_CPP
	EpgSloveniaPluginCreator.cpp
	EpgSloveniaPlugin.cpp
)

SET(SOURCES_H
	EpgSloveniaPluginCreator.h
)

QT4_WRAP_CPP(SOURCES_H_MOC ${SOURCES_H})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

ADD_LIBRARY(EpgSloveniaPlugin SHARED ${SOURCES_CPP} ${SOURCES_H_MOC})
ADD_DEPENDENCIES(EpgSloveniaPlugin tano)

TARGET_LINK_LIBRARIES(EpgSloveniaPlugin ${QT_LIBRARIES})

IF(MINGW)
INSTALL(TARGETS EpgSloveniaPlugin DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR})
ELSE(MINGW)
INSTALL(TARGETS EpgSloveniaPlugin LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR})
ENDIF(MINGW)
