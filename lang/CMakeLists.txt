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

SET(TEMPLATE ${CMAKE_SOURCE_DIR}/lang/source.ts)

IF(REFRESH_TRANSLATIONS)
    IF(NOT QT_LUPDATE_EXECUTABLE AND NOT Qt5_LUPDATE_EXECUTABLE)
        MESSAGE(WARNING "Tano could not find lupdate. You won't be able to update translations.")
    ENDIF(NOT QT_LUPDATE_EXECUTABLE AND NOT Qt5_LUPDATE_EXECUTABLE)
ENDIF(REFRESH_TRANSLATIONS)

IF(NOT QT_LRELEASE_EXECUTABLE AND NOT Qt5_LRELEASE_EXECUTABLE)
    MESSAGE(WARNING "Tano could not find lrelease. Your build won't contain translations.")
ENDIF(NOT QT_LRELEASE_EXECUTABLE AND NOT Qt5_LRELEASE_EXECUTABLE)

## Collect all translations files
FILE(GLOB Tano_TS ${CMAKE_SOURCE_DIR}/lang/*.ts)
LIST(REMOVE_ITEM Tano_TS ${TEMPLATE})

IF(QT_LRELEASE_EXECUTABLE OR Qt5_LRELEASE_EXECUTABLE)
    IF(REFRESH_TRANSLATIONS)
        FILE(GLOB_RECURSE translate_SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)
        FILE(GLOB_RECURSE translate_UIS ${CMAKE_SOURCE_DIR}/src/*.ui)
        SET(translate_SRCS ${translate_SRCS} ${translate_UIS})
    ENDIF(REFRESH_TRANSLATIONS)

    IF(WITH_QT5)
        IF(REFRESH_TRANSLATIONS)
            QT5_CREATE_TRANSLATION(Tano_QMS ${translate_SRCS} ${Tano_TS})
            QT5_CREATE_TRANSLATION(Tano_QMS_SRC ${translate_SRCS} ${TEMPLATE})
        ELSE(REFRESH_TRANSLATIONS)
            QT5_ADD_TRANSLATION(Tano_QMS ${Tano_TS})
        ENDIF(REFRESH_TRANSLATIONS)
    ELSE(WITH_QT5)
        IF(REFRESH_TRANSLATIONS)
            QT4_CREATE_TRANSLATION(Tano_QMS ${translate_SRCS} ${Tano_TS})
            QT4_CREATE_TRANSLATION(Tano_QMS_SRC ${translate_SRCS} ${TEMPLATE})
        ELSE(REFRESH_TRANSLATIONS)
            QT4_ADD_TRANSLATION(Tano_QMS ${Tano_TS})
        ENDIF(REFRESH_TRANSLATIONS)
    ENDIF(WITH_QT5)

    ADD_CUSTOM_TARGET(translations ALL DEPENDS ${Tano_QMS})
    IF(REFRESH_TRANSLATIONS)
        ADD_CUSTOM_TARGET(translations_src ALL DEPENDS ${Tano_QMS_SRC})
    ENDIF(REFRESH_TRANSLATIONS)
ENDIF(QT_LRELEASE_EXECUTABLE OR Qt5_LRELEASE_EXECUTABLE)


## Installation
INSTALL(FILES ${Tano_QMS} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/lang")
