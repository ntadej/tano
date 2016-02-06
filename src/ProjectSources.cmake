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
    common/Backend.cpp
    common/File.h
    common/ListItem.h
    common/ListModel.cpp
    common/NetworkManager.cpp
    common/qtcassert.cpp
    common/Resources.cpp
    common/widgets/BrowseWidget.cpp
    common/widgets/FileDialogs.cpp
)

SET(Project_Channels_Srcs
    channels/containers/Channel.cpp
    channels/generators/CSVGenerator.cpp
    channels/generators/M3UGenerator.cpp
    channels/generators/XmltvIdGenerator.cpp
    channels/handlers/CSVHandler.cpp
    channels/handlers/M3UHandler.cpp
    channels/helpers/ChannelsOpen.cpp
    channels/helpers/ChannelsSave.cpp
    channels/helpers/ChannelsUpdate.cpp
    channels/models/ChannelsFilterModel.cpp
    channels/models/ChannelsModel.cpp
    channels/widgets/ChannelsDisplayWidget.cpp
    channels/widgets/ChannelsSelect.cpp
    channels/editor/ChannelsEditor.cpp
    channels/editor/ChannelsEditorHelp.cpp
    channels/editor/ChannelsImportCSV.cpp
    channels/editor/ChannelsPrint.cpp
    channels/editor/ChannelsPrintDialog.cpp
    channels/editor/ChannelsScan.cpp
)

SET(Project_Network_Srcs
    network/NetworkDownload.cpp
    network/NetworkRequest.cpp
    network/NetworkUdpxy.cpp
)

SET(Project_Settings_Srcs
    settings/Settings.cpp
    settings/SettingsChannel.cpp
    settings/SettingsDefaults.cpp
    settings/SettingsKeys.cpp
    settings/SettingsShortcuts.cpp
    settings/SettingsShortcutsDefaults.cpp
    settings/SettingsShortcutsDesktop.cpp
    settings/widgets/SettingsDialog.cpp
    settings/widgets/SettingsKeyDelegate.cpp
    settings/widgets/SettingsNotEditableDelegate.cpp
    settings/widgets/SettingsPlayback.cpp
    settings/widgets/SettingsTv.cpp
)

SET(Project_Style_Srcs
    style/FancyLineEdit.cpp
    style/FancyTabWidget.cpp
    style/Icon.cpp
    style/ManhattanStyle.cpp
    style/MiniSplitter.cpp
    style/StyleAnimator.cpp
    style/StyleHelper.cpp
    style/StyledBar.cpp
    style/Theme.cpp
    style/Theme_p.h
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
    ${Project_Srcs}

    main.cpp

    ${Project_Application_Srcs}
    ${Project_Common_Srcs}
    ${Project_Channels_Srcs}
    ${Project_Network_Srcs}
    ${Project_Plugins_Srcs}
    ${Project_Settings_Srcs}
    ${Project_Style_Srcs}
    ${Project_Timers_Srcs}
    ${Project_Xmltv_Srcs}

    ${Project_Platform_Srcs}

    ${Project_Widgets_Tmp_Srcs}
)
