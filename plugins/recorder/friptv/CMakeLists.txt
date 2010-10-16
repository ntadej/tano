#############################################################################
# Tano - Frip recorder plugin
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

PROJECT(tanorecorder-friptv)

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
	${QT_INCLUDE_DIR}
	../../../src
)

SET(SOURCES_CPP
	FripTvCreator.cpp
	FripTv.cpp
)

SET(SOURCES_H
	FripTvCreator.h
)

QT4_WRAP_CPP(SOURCES_H_MOC ${SOURCES_H})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

ADD_LIBRARY(tanorecorder-friptv SHARED ${SOURCES_CPP} ${SOURCES_H_MOC})
ADD_DEPENDENCIES(tanorecorder-friptv tano)

TARGET_LINK_LIBRARIES(tanorecorder-friptv ${QT_LIBRARIES})

IF(MINGW)
INSTALL(TARGETS tanorecorder-friptv DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR})
ELSE(MINGW)
INSTALL(TARGETS tanorecorder-friptv LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR})
ENDIF(MINGW)
