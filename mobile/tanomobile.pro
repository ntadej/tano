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
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#############################################################################

DEFINES += MOBILE=1 \
    WITH_EDITOR_VLCQT=0

# Add more folders to ship with the application, here
qml_files.source = src/qml
qml_files.target =
DEPLOYMENTFOLDERS = qml_files

# Additional Qt requirements
QT += xml

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xEBABAB0D

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Add dependency to symbian components
# CONFIG += qtquickcomponents

INCLUDEPATH += \
    ../ \
    ../src/

SOURCES += \
    src/main.cpp \
    src/MobileConstants.cpp \
    src/MobileCore.cpp \
    src/MobileXmltvHandler.cpp

SOURCES += \
    ../src/container/xmltv/XmltvChannel.cpp \
    ../src/container/xmltv/XmltvCrewMember.cpp \
    ../src/container/xmltv/XmltvList.cpp \
    ../src/container/xmltv/XmltvProgramme.cpp \
    ../src/core/Common.cpp \
    ../src/core/ConsoleOutput.cpp \
    ../src/core/Enums.cpp \
    ../src/core/ListModel.cpp \
    ../src/core/LocaleManager.cpp \
    ../src/core/Settings.cpp \
    ../src/epg/XmltvChannelsFilterModel.cpp \
    ../src/epg/XmltvChannelsModel.cpp \
    ../src/epg/XmltvCommon.cpp \
    ../src/epg/XmltvCrewFilterModel.cpp \
    ../src/epg/XmltvCrewModel.cpp \
    ../src/epg/XmltvEnums.cpp \
    ../src/epg/XmltvHandler.cpp \
    ../src/epg/XmltvManager.cpp \
    ../src/epg/XmltvProgrammeFilterModel.cpp \
    ../src/epg/XmltvProgrammeModel.cpp

HEADERS += \
    src/MobileConstants.h \
    src/MobileCore.h \
    src/MobileXmltvHandler.h

HEADERS += \
    ../src/container/xmltv/XmltvChannel.h \
    ../src/container/xmltv/XmltvCrewMember.h \
    ../src/container/xmltv/XmltvList.h \
    ../src/container/xmltv/XmltvProgramme.h \
    ../src/core/Common.h \
    ../src/core/ConsoleOutput.h \
    ../src/core/Enums.h \
    ../src/core/ListItem.h \
    ../src/core/ListModel.h \
    ../src/core/LocaleManager.h \
    ../src/core/Settings.h \
    ../src/epg/XmltvChannelsFilterModel.h \
    ../src/epg/XmltvChannelsModel.h \
    ../src/epg/XmltvCommon.h \
    ../src/epg/XmltvCrewFilterModel.h \
    ../src/epg/XmltvCrewModel.h \
    ../src/epg/XmltvEnums.h \
    ../src/epg/XmltvHandler.h \
    ../src/epg/XmltvManager.h \
    ../src/epg/XmltvProgrammeFilterModel.h \
    ../src/epg/XmltvProgrammeModel.h

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    tanomobile.desktop \
    tanomobile_harmattan.desktop \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
