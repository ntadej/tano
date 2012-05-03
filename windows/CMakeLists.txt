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

MACRO(NSIS_FILE_OUTPUT outfiles path)
    FOREACH(it ${ARGN})
        STRING_SPLIT(list "/" ${it})
        LIST(GET list -1 file)

        SET(files "${files}\${File} \"${path}\" \"${file}\"\n")
    ENDFOREACH(it)

    SET(${outfiles} ${files})
    SET(files "")
ENDMACRO(NSIS_FILE_OUTPUT)

MACRO(NSIS_VLC_PLUGINS outfiles)
    FOREACH(type ${ARGN})
        FILE(GLOB plugins ${LIBVLCQT_BIN_DIR}/plugins/${type}/*.dll)
        NSIS_FILE_OUTPUT(fplugins "..\\\\plugins\\\\${type}" ${plugins})

        SET(output "${output}\${SetOutPath} \"$INSTDIR\\plugins\\${type}\"\n")
        SET(output "${output}${fplugins}\n")
    ENDFOREACH(type)

    SET(output "${output}\${SetOutPath} \"$INSTDIR\\plugins\"\n")
    SET(output "${output}\${File} \"..\\plugins\" \"plugins.dat\"\n")
    SET(${outfiles} ${output})
ENDMACRO(NSIS_VLC_PLUGINS)

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

IF(MINGW)
    SET(Tano_Installer
        LICENSE.GPL.TXT

        tano-player.nsi
        tano-editor.nsi

        InstallerLanguageEnglish.nsh
        InstallerUI.nsh

        UAC.nsh
        UninstallLog.nsh
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

    STRING(REGEX REPLACE "bin/gcc.exe" "bin" MINGW_BIN_DIR "${CMAKE_C_COMPILER}")
    STRING(REGEX REPLACE "bin" "plugins" QT_PLUGINS_DIR "${QT_BIN_DIR}")
    STRING(REGEX REPLACE "\\\\" "/" LIBVLCQT_INCLUDE_DIR "${LIBVLCQT_INCLUDE_DIR}")
    STRING(REGEX REPLACE "include" "bin" LIBVLCQT_BIN_DIR "${LIBVLCQT_INCLUDE_DIR}")

    SET(Tano_Qt_Lib
        ${MINGW_BIN_DIR}/libgcc_s_dw2-1.dll
        ${MINGW_BIN_DIR}/mingwm10.dll
        ${QT_BIN_DIR}/QtCore4.dll
        ${QT_BIN_DIR}/QtGui4.dll
        ${QT_BIN_DIR}/QtNetwork4.dll
        ${QT_BIN_DIR}/QtXml4.dll
    )
    SET(Tano_Qt_Plugins
        ${QT_PLUGINS_DIR}/imageformats/qgif4.dll
        ${QT_PLUGINS_DIR}/imageformats/qico4.dll
        ${QT_PLUGINS_DIR}/imageformats/qjpeg4.dll
        ${QT_PLUGINS_DIR}/imageformats/qmng4.dll
        ${QT_PLUGINS_DIR}/imageformats/qsvg4.dll
        ${QT_PLUGINS_DIR}/imageformats/qtga4.dll
        ${QT_PLUGINS_DIR}/imageformats/qtiff4.dll
    )
    SET(Tano_Vlc_Libs
        ${LIBVLCQT_BIN_DIR}/libvlc-qt.dll
        ${LIBVLCQT_BIN_DIR}/libvlc.dll
        ${LIBVLCQT_BIN_DIR}/libvlccore.dll
    )

    FILE(GLOB Tano_Language ${CMAKE_BINARY_DIR}/lang/*.qm)
ENDIF(MINGW)


########################
# Installer generation #
########################
IF(GIT_OUT)
    SET(TANO_WIN_VERSION ${TANO_VERSION}-${PROJECT_VERSION_PATCH})
ELSE(GIT_OUT)
    SET(TANO_WIN_VERSION ${TANO_VERSION})
ENDIF(GIT_OUT)

NSIS_FILE_OUTPUT(NSIS_Qt ".." ${Tano_Qt_Lib})
NSIS_FILE_OUTPUT(NSIS_Qt_Plugins "..\\\\imageformats" ${Tano_Qt_Plugins})
NSIS_FILE_OUTPUT(NSIS_Language "..\\\\lang" ${Tano_Language})
NSIS_FILE_OUTPUT(NSIS_VLC ".." ${Tano_Vlc_Libs})
NSIS_VLC_PLUGINS(NSIS_VLC_Plugins ${Tano_Vlc_Plugin_Types})

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/windows/InstallerConfig.nsh.in
    ${CMAKE_BINARY_DIR}/windows/InstallerConfig.nsh
    @ONLY
)

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/windows/InstallerFiles.nsh.in
    ${CMAKE_BINARY_DIR}/windows/InstallerFiles.nsh
    @ONLY
)

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/windows/InstallerFilesEditor.nsh.in
    ${CMAKE_BINARY_DIR}/windows/InstallerFilesEditor.nsh
    @ONLY
)

SET(Tano_Installer_Generated
    ${CMAKE_BINARY_DIR}/windows/InstallerConfig.nsh
    ${CMAKE_BINARY_DIR}/windows/InstallerFiles.nsh
    ${CMAKE_BINARY_DIR}/windows/InstallerFilesEditor.nsh
)

################
# Installation #
################
IF(MINGW)
    INSTALL(FILES ${Tano_Installer} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/windows")
    INSTALL(FILES ${Tano_Installer_Generated} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/windows")
    INSTALL(FILES ${Tano_Installer_Theme} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/windows/installer-theme")
    INSTALL(FILES ${Tano_Qt_Lib} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    INSTALL(FILES ${Tano_Qt_Plugins} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/imageformats")
    INSTALL(FILES ${Tano_Vlc_Libs} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    INSTALL_VLC_PLUGINS(${Tano_Vlc_Plugin_Types})
ENDIF(MINGW)
