#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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
SET(Tano_Application_Srcs
    application/Arguments.cpp
    application/LocaleManager.cpp
    application/LocalServer.cpp
    application/Log.cpp
    application/Output.cpp
)
SET(Tano_Common_Srcs
    common/Common.cpp
    common/File.h
    common/ListItem.h
    common/ListModel.cpp
    common/Resources.cpp
    common/UpdateHandler.cpp
)
SET(Tano_Network_Srcs
    network/NetworkDownload.cpp
    network/NetworkRequest.cpp
    network/NetworkUdpxy.cpp
)
SET(Tano_Playlist_Srcs
    playlist/PlaylistFilterModel.cpp
    playlist/PlaylistModel.cpp
    playlist/PlaylistOpen.cpp
    playlist/PlaylistSave.cpp
    playlist/PlaylistUpdate.cpp
    playlist/containers/Channel.cpp
    playlist/generators/CSVGenerator.cpp
    playlist/generators/M3UGenerator.cpp
    playlist/generators/XmltvIdGenerator.cpp
    playlist/handlers/CSVHandler.cpp
    playlist/handlers/M3UHandler.cpp
)
SET(Tano_Plugins_Srcs
    plugins/ConfigPlugin.h
    plugins/Plugins.cpp
)
SET(Tano_Settings_Srcs
    settings/Settings.cpp
    settings/SettingsChannel.cpp
    settings/SettingsDefaults.cpp
    settings/SettingsKeys.cpp
    settings/SettingsPassword.cpp
    settings/SettingsShortcuts.cpp
    settings/SettingsShortcutsDefaults.cpp
)
SET(Tano_Timers_Srcs
    timers/TimersGenerator.cpp
    timers/TimersHandler.cpp
    timers/TimersTimeManager.cpp
    timers/containers/Timer.cpp
    timers/models/TimersFilterModel.cpp
    timers/models/TimersModel.cpp
)
SET(Tano_Xmltv_Srcs
    xmltv/XmltvCommon.cpp
    xmltv/XmltvCommonCategories.cpp
    xmltv/XmltvGenerator.cpp
    xmltv/XmltvHandler.cpp
    xmltv/XmltvManager.cpp
    xmltv/XmltvSql.cpp
    xmltv/containers/XmltvChannel.cpp
    xmltv/containers/XmltvCrewMember.cpp
    xmltv/containers/XmltvProgramme.cpp
    xmltv/models/XmltvCrewFilterModel.cpp
    xmltv/models/XmltvCrewModel.cpp
    xmltv/models/XmltvProgrammeFilterModel.cpp
    xmltv/models/XmltvProgrammeModel.cpp
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(Tano_Platform_Srcs
        platform/Linux.cpp
    )
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(Tano_Platform_Srcs
        platform/OSX.cpp
    )
ENDIF()

IF(MSVC)
    SET(Tano_Platform_Srcs
        platform/Windows.cpp
    )
ENDIF()

INCLUDE(WidgetsTmp)

SET(Tano_Srcs
    ${Tano_Application_Srcs}
    ${Tano_Common_Srcs}
    ${Tano_Network_Srcs}
    ${Tano_Playlist_Srcs}
    ${Tano_Plugins_Srcs}
    ${Tano_Settings_Srcs}
    ${Tano_Timers_Srcs}
    ${Tano_Xmltv_Srcs}

    ${Tano_Platform_Srcs}

    ${Tano_Widgets_Srcs}
)
