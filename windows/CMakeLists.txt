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
SET(Tano_Vlc_Plugin_Types
    3dnow
    access
    access_output
    audio_filter
    audio_mixer
    audio_output
    codec
    control
    demux
    media_library
    meta_engine
    misc
    mmx
    mmxext
    mux
    notify
    packetizer
    services_discovery
    sse2
    stream_filter
    stream_out
    text_renderer
    video_chroma
    video_filter
    video_output
    visualization
)

MACRO(VLC_INSTALL_PLUGINS)
    FOREACH(type ${ARGN})
        FILE(GLOB Vlc_Plugins ${LIBVLCQT_BIN_DIR}/plugins/${type}/*.dll)
        INSTALL(FILES ${Vlc_Plugins} DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins/${type}")
    ENDFOREACH(type)
ENDMACRO(VLC_INSTALL_PLUGINS)

IF(MINGW)
    SET(Tano_Installer
        LICENSE.GPL.TXT
        tano.nsi
        tano-editor.nsi
        tano-install.nsh
        tano-install-editor.nsh
        tano-language.nsh
        tano-ui.nsh
        tano-uninstall.nsh
        tano-uninstall-editor.nsh
    )
    SET(Tano_Installer_Theme
        installer-theme/header.bmp
        installer-theme/installer.ico
        installer-theme/installer-nopng.ico
        installer-theme/installer.png
        installer-theme/License.txt
        installer-theme/uninstaller.ico
        installer-theme/uninstaller-nopng.ico
        installer-theme/uninstaller.png
        installer-theme/wizard.bmp
    )

    STRING(REGEX REPLACE "\\\\" "/" LIBVLCQT_INCLUDE_DIR "${LIBVLCQT_INCLUDE_DIR}")
    STRING(REGEX REPLACE "include" "bin" LIBVLCQT_BIN_DIR "${LIBVLCQT_INCLUDE_DIR}")

    SET(Tano_Qt_Lib
        ${QT_BIN_DIR}/QtCore4.dll
        ${QT_BIN_DIR}/QtGui4.dll
        ${QT_BIN_DIR}/QtNetwork4.dll
        ${QT_BIN_DIR}/QtSvg4.dll
        ${QT_BIN_DIR}/QtXml4.dll
        ${QT_BIN_DIR}/libgcc_s_dw2-1.dll
        ${QT_BIN_DIR}/mingwm10.dll
    )
    SET(Tano_Vlc_Libs
        ${LIBVLCQT_BIN_DIR}/libvlc-qt.dll
        ${LIBVLCQT_BIN_DIR}/libvlc.dll
        ${LIBVLCQT_BIN_DIR}/libvlccore.dll
    )
ENDIF(MINGW)


################
# Installation #
################
IF(MINGW)
    INSTALL(FILES ${Tano_Installer} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/windows")
    INSTALL(FILES ${Tano_Installer_Theme} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/windows/installer-theme")
    INSTALL(FILES ${Tano_Qt_Lib} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    INSTALL(FILES ${Tano_Vlc_Libs} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    VLC_INSTALL_PLUGINS(${Tano_Vlc_Plugin_Types})
ENDIF(MINGW)
