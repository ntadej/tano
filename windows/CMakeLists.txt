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
# Windows Extra #
#################
MACRO(INSTALL_VLC_PLUGINS)
    FOREACH(type ${ARGN})
        FILE(GLOB Vlc_Plugins ${LIBVLCQT_BIN_DIR}/plugins/${type}/*.dll)
        INSTALL(FILES ${Vlc_Plugins} DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins/${type}")
    ENDFOREACH(type)
ENDMACRO(INSTALL_VLC_PLUGINS)

SET(Tano_Vlc_Plugin_Types
    3dnow
    access
    access_output
    audio_filter
    audio_mixer
    audio_output
    codec
    demux
    meta_engine
    misc
    mmx
    mmxext
    mux
    packetizer
    services_discovery
    sse2
    stream_filter
    stream_out
    text_renderer
    video_chroma
    video_filter
    video_output
)

IF(MINGW)
    STRING(REGEX REPLACE "bin/gcc.exe" "bin" MINGW_BIN_DIR "${CMAKE_C_COMPILER}")
ENDIF(MINGW)

STRING(REGEX REPLACE "bin" "plugins" QT_PLUGINS_DIR "${QT_BIN_DIR}")
STRING(REGEX REPLACE "\\\\" "/" LIBVLCQT_INCLUDE_DIR "${LIBVLCQT_INCLUDE_DIR}")
STRING(REGEX REPLACE "include" "bin" LIBVLCQT_BIN_DIR "${LIBVLCQT_INCLUDE_DIR}")

SET(Tano_Qt4_Libs
    ${QT_BIN_DIR}/QtCore4.dll
    ${QT_BIN_DIR}/QtGui4.dll
    ${QT_BIN_DIR}/QtNetwork4.dll
    ${QT_BIN_DIR}/QtXml4.dll
    ${QT_BIN_DIR}/zlib1.dll
)

SET(Tano_Qt5_Libs
    ${QT_BIN_DIR}/Qt5Core.dll
    ${QT_BIN_DIR}/Qt5Gui.dll
    ${QT_BIN_DIR}/Qt5Network.dll
    ${QT_BIN_DIR}/Qt5PrintSupport.dll
    ${QT_BIN_DIR}/Qt5Widgets.dll
    ${QT_BIN_DIR}/Qt5Xml.dll
    ${QT_BIN_DIR}/icudt49.dll
    ${QT_BIN_DIR}/icuin49.dll
    ${QT_BIN_DIR}/icuuc49.dll
    ${QT_BIN_DIR}/libEGL.dll
    ${QT_BIN_DIR}/libGLESv2.dll
    ${QT_BIN_DIR}/D3DCompiler_43.dll
)

SET(Tano_Qt5_Platforms
    ${QT_PLUGINS_DIR}/platforms/qminimal.dll
    ${QT_PLUGINS_DIR}/platforms/qwindows.dll
)

SET(Tano_Qt4_ImageFormats
    ${QT_PLUGINS_DIR}/imageformats/qgif4.dll
    ${QT_PLUGINS_DIR}/imageformats/qico4.dll
    ${QT_PLUGINS_DIR}/imageformats/qjpeg4.dll
    ${QT_PLUGINS_DIR}/imageformats/qmng4.dll
    ${QT_PLUGINS_DIR}/imageformats/qsvg4.dll
    ${QT_PLUGINS_DIR}/imageformats/qtga4.dll
    ${QT_PLUGINS_DIR}/imageformats/qtiff4.dll
)

SET(Tano_Qt5_ImageFormats
    ${QT_PLUGINS_DIR}/imageformats/qgif.dll
    ${QT_PLUGINS_DIR}/imageformats/qico.dll
    ${QT_PLUGINS_DIR}/imageformats/qjpeg.dll
    ${QT_PLUGINS_DIR}/imageformats/qmng.dll
    ${QT_PLUGINS_DIR}/imageformats/qsvg.dll
    ${QT_PLUGINS_DIR}/imageformats/qtga.dll
    ${QT_PLUGINS_DIR}/imageformats/qtiff.dll
    ${QT_PLUGINS_DIR}/imageformats/qwbmp.dll
)

IF(MSVC)
    SET(Tano_VlcQt_Libs
        ${LIBVLCQT_BIN_DIR}/vlc-qt.dll
        ${LIBVLCQT_BIN_DIR}/vlc-qt-widgets.dll
    )
ELSE(MSVC)
    SET(Tano_VlcQt_Libs
        ${LIBVLCQT_BIN_DIR}/vlc-qt.dll
        ${LIBVLCQT_BIN_DIR}/vlc-qt-widgets.dll
    )
ENDIF(MSVC)

SET(Tano_Vlc_Libs
    ${LIBVLCQT_BIN_DIR}/libvlc.dll
    ${LIBVLCQT_BIN_DIR}/libvlccore.dll
)

FILE(GLOB Tano_Language ${CMAKE_BINARY_DIR}/lang/*.qm)


################
# Installation #
################
IF(WITH_QT5)
    INSTALL(FILES ${Tano_Qt5_Libs} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    INSTALL(FILES ${Tano_Qt5_ImageFormats} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/imageformats")
    INSTALL(FILES ${Tano_Qt5_Platforms} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/platforms")
ELSE(WITH_QT5)
    INSTALL(FILES ${Tano_Qt4_Libs} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    INSTALL(FILES ${Tano_Qt4_ImageFormats} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/imageformats")
ENDIF(WITH_QT5)

INSTALL(FILES ${Tano_VlcQt_Libs} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
INSTALL(FILES ${Tano_Vlc_Libs} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")

INSTALL_VLC_PLUGINS(${Tano_Vlc_Plugin_Types})
