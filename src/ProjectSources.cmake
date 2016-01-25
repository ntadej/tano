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
SET(Project_Application_Srcs
    application/Common.cpp
    application/LocaleManager.cpp
    application/LocalServer.cpp
    application/Log.cpp
    application/Notifications.cpp
    application/Output.cpp
    application/SingleApplication.cpp
    application/TanoApplication.cpp
    application/Updates.cpp
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(Project_Application_Srcs
        ${Project_Application_Srcs}

        application/NotificationsOSX.mm
    )
ENDIF()

SET(Project_Common_Srcs
    common/File.h
    common/ListItem.h
    common/ListModel.cpp
    common/NetworkManager.cpp
    common/Resources.cpp
)

SET(Project_Network_Srcs
    network/NetworkDownload.cpp
    network/NetworkRequest.cpp
    network/NetworkUdpxy.cpp
)

SET(Project_Playlist_Srcs
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

SET(Project_Settings_Srcs
    settings/Settings.cpp
    settings/SettingsChannel.cpp
    settings/SettingsDefaults.cpp
    settings/SettingsKeys.cpp
    settings/SettingsPassword.cpp
    settings/SettingsShortcuts.cpp
    settings/SettingsShortcutsDefaults.cpp
)
SET(Project_Timers_Srcs
    timers/TimersGenerator.cpp
    timers/TimersHandler.cpp
    timers/TimersTimeManager.cpp
    timers/containers/Timer.cpp
    timers/models/TimersFilterModel.cpp
    timers/models/TimersModel.cpp
)
SET(Project_Xmltv_Srcs
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
    SET(Project_Platform_Srcs
        platform/Linux.cpp
    )
ENDIF()

IF(MSVC)
    SET(Project_Platform_Srcs
        platform/Windows.cpp
    )
ENDIF()

INCLUDE(WidgetsTmp)

SET(Project_Srcs
    main.cpp

    ${Project_Application_Srcs}
    ${Project_Common_Srcs}
    ${Project_Network_Srcs}
    ${Project_Playlist_Srcs}
    ${Project_Plugins_Srcs}
    ${Project_Settings_Srcs}
    ${Project_Timers_Srcs}
    ${Project_Xmltv_Srcs}

    ${Project_Platform_Srcs}

    ${Project_Widgets_Srcs}
)
