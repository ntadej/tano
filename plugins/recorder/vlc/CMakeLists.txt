#############################################################################
# Tano - VlcPlugin
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

PROJECT(tanorecorder-vlc)

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
	${QT_INCLUDE_DIR}
	../../../src
)

SET(SOURCES_CPP
	VlcCreator.cpp
	Vlc.cpp
)

SET(SOURCES_H
	VlcCreator.h
)

QT4_WRAP_CPP(SOURCES_H_MOC ${SOURCES_H})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

ADD_LIBRARY(tanorecorder-vlc SHARED ${SOURCES_CPP} ${SOURCES_H_MOC})
ADD_DEPENDENCIES(tanorecorder-vlc tano)

TARGET_LINK_LIBRARIES(tanorecorder-vlc ${QT_LIBRARIES})

IF(MINGW)
INSTALL(TARGETS tanorecorder-vlc DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR})
ELSE(MINGW)
INSTALL(TARGETS tanorecorder-vlc LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR})
ENDIF(MINGW)
