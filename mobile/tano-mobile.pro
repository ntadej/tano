#############################################################################
# Tano Mobile - An open IP TV player
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

QT += core gui xml

TARGET = tano
TEMPLATE = app

INCLUDEPATH = ../src

SOURCES += main.cpp\
	About.cpp\
	EditSettings.cpp\
	Mobile.cpp\
	SelectPlaylist.cpp\
	../src/channels/Channel.cpp\
	../src/channels/ChannelEpg.cpp\
	../src/core/Common.cpp\
	../src/core/LocaleManager.cpp\
	../src/core/Settings.cpp\
	../src/core/Version.cpp\
	../src/epg/EpgList.cpp\
	../src/epg/EpgSchedule.cpp\
	../src/plugins/PluginsLoader.cpp\
	../src/ui/PlaylistWidget.cpp\
	../src/xml/M3UHandler.cpp\
	../src/xml/M3UGenerator.cpp\
	../src/xml/tanohandler.cpp

HEADERS  += About.h\
	Config.h\
	EditSettings.h\
	Mobile.h\
	SelectPlaylist.h\
	../src/channels/Channel.h\
	../src/channels/ChannelEpg.h\
	../src/core/Common.h\
	../src/core/LocaleManager.h\
	../src/core/Settings.h\
	../src/core/Version.h\
	../src/epg/EpgList.h\
	../src/epg/EpgSchedule.h\
	../src/plugins/PluginsLoader.h\
	../src/ui/PlaylistWidget.h\
	../src/xml/M3UHandler.h\
	../src/xml/M3UGenerator.h\
	../src/xml/tanohandler.h


FORMS += About.ui\
	EditSettings.ui\
	Mobile.ui\
	SelectPlaylist.ui\
	../src/ui/EpgSchedule.ui\
	../src/ui/PlaylistWidget.ui


RESOURCES = ../src/ui/images.qrc
