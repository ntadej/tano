#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

INCLUDEPATH += ../../src/ \
    ../../

DEFINES += MM=1 \
    MOBILE=1 \
    WITH_EDITOR_VLCQT=0

QT+= declarative

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

HEADERS += ../MobileCore.h \
    ../MobilePlaylistHandler.h \
    ../MobileXmltvHandler.h \
    ../../src/container/core/Channel.h \
    ../../src/container/playlist/CSVInfo.h \
    ../../src/container/xmltv/XmltvChannel.h \
    ../../src/container/xmltv/XmltvCrewMember.h \
    ../../src/container/xmltv/XmltvList.h \
    ../../src/container/xmltv/XmltvProgramme.h \
    ../../src/core/Common.h \
    ../../src/core/ConsoleOutput.h \
    ../../src/core/Enums.h \
    ../../src/core/ListItem.h \
    ../../src/core/ListModel.h \
    ../../src/core/LocaleManager.h \
    ../../src/core/Settings.h \
    ../../src/core/Shortcuts.h \
    ../../src/core/Udpxy.h \
    ../../src/epg/XmltvCommon.h \
    ../../src/epg/XmltvCrewModel.h \
    ../../src/epg/XmltvEnums.h \
    ../../src/epg/XmltvManager.h \
    ../../src/epg/XmltvProgrammeFilterModel.h \
    ../../src/epg/XmltvProgrammeModel.h \
    ../../src/playlist/PlaylistFilterModel.h \
    ../../src/playlist/PlaylistModel.h \
    ../../src/playlist/PlaylistOpen.h \
    ../../src/playlist/handlers/M3UHandler.h \
    ../../src/xml/XmltvHandler.h

SOURCES += ../main.cpp \
    ../MobileCore.cpp \
    ../MobilePlaylistHandler.cpp \
    ../MobileXmltvHandler.cpp \
    ../../src/container/core/Channel.cpp \
    ../../src/container/playlist/CSVInfo.cpp \
    ../../src/container/xmltv/XmltvChannel.cpp \
    ../../src/container/xmltv/XmltvCrewMember.cpp \
    ../../src/container/xmltv/XmltvList.cpp \
    ../../src/container/xmltv/XmltvProgramme.cpp \
    ../../src/core/Common.cpp \
    ../../src/core/ConsoleOutput.cpp \
    ../../src/core/Enums.cpp \
    ../../src/core/ListModel.cpp \
    ../../src/core/LocaleManager.cpp \
    ../../src/core/Settings.cpp \
    ../../src/core/Shortcuts.cpp \
    ../../src/core/Udpxy.cpp \
    ../../src/epg/XmltvCommon.cpp \
    ../../src/epg/XmltvCrewModel.cpp \
    ../../src/epg/XmltvEnums.cpp \
    ../../src/epg/XmltvManager.cpp \
    ../../src/epg/XmltvProgrammeFilterModel.cpp \
    ../../src/epg/XmltvProgrammeModel.cpp \
    ../../src/playlist/PlaylistFilterModel.cpp \
    ../../src/playlist/PlaylistModel.cpp \
    ../../src/playlist/PlaylistOpen.cpp \
    ../../src/playlist/handlers/M3UHandler.cpp \
    ../../src/xml/XmltvHandler.cpp

OTHER_FILES += \
    tanomobile.desktop \
    tanomobile.svg \
    tanomobile.png \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

RESOURCES += \
    ../qml/files.qrc

# Extra files
share.path = /opt/tanomobile/share
share.files = ../xmltv.xml

playlists.path = /opt/tanomobile/share/playlists
playlists.files += ../../playlists/playlists.xml

playlists-int.path = /opt/tanomobile/share/playlists/int
playlists-int.files += ../../playlists/int/worldtv.m3u

playlists-ru.path = /opt/tanomobile/share/playlists/ru
playlists-ru.files += ../../playlists/ru/diseltv-krasnodar.m3u \
../../playlists/ru/diseltv-rostov.m3u

playlists-sl.path = /opt/tanomobile/share/playlists/sl
playlists-sl.files += ../../playlists/sl/amis.m3u \
../../playlists/sl/siol-mpeg2.m3u \
../../playlists/sl/siol-mpeg4.m3u \
../../playlists/sl/t-2.m3u \
../../playlists/sl/tus.m3u

INSTALLS += share \
playlists \
playlists-int \
playlists-ru \
playlists-sl

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()
