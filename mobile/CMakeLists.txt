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
###############################################################################

PROJECT(tano-mobile)
cmake_minimum_required(VERSION 2.6)

###################
# List of sources #
###################
# Define the MOC source files used by Tano Mobile
# List of classes which Qt needs to produce MOCs for them (in order for events, signals, slots and other macros to work
SET(Tano_Mobile_MOCs
    src/MobileCore.h
    src/MobileXmltvHandler.h

    qmlapplicationviewer/qmlapplicationviewer.h

    ../src/container/xmltv/XmltvChannel.h
    ../src/container/xmltv/XmltvCrewMember.h
    ../src/container/xmltv/XmltvList.h
    ../src/container/xmltv/XmltvProgramme.h
    ../src/core/ListItem.h
    ../src/core/ListModel.h
    ../src/epg/XmltvChannelsFilterModel.h
    ../src/epg/XmltvChannelsModel.h
    ../src/epg/XmltvCrewFilterModel.h
    ../src/epg/XmltvCrewModel.h
    ../src/epg/XmltvManager.h
    ../src/epg/XmltvProgrammeFilterModel.h
    ../src/epg/XmltvProgrammeModel.h
)

# Define the C++ source files used by Tano
SET(Tano_Mobile_Srcs
    src/main.cpp
    src/MobileConstants.cpp
    src/MobileCore.cpp
    src/MobileXmltvHandler.cpp

    qmlapplicationviewer/qmlapplicationviewer.cpp

    ../src/container/xmltv/XmltvChannel.cpp
    ../src/container/xmltv/XmltvCrewMember.cpp
    ../src/container/xmltv/XmltvList.cpp
    ../src/container/xmltv/XmltvProgramme.cpp

    ../src/core/Common.cpp
    ../src/core/Enums.cpp
    ../src/core/ListModel.cpp
    ../src/core/LocaleManager.cpp
    ../src/core/Settings.cpp

    ../src/epg/XmltvCommon.cpp
    ../src/epg/XmltvChannelsFilterModel.cpp
    ../src/epg/XmltvChannelsModel.cpp
    ../src/epg/XmltvCrewFilterModel.cpp
    ../src/epg/XmltvCrewModel.cpp
    ../src/epg/XmltvEnums.cpp
    ../src/epg/XmltvHandler.cpp
    ../src/epg/XmltvManager.cpp
    ../src/epg/XmltvProgrammeFilterModel.cpp
    ../src/epg/XmltvProgrammeModel.cpp
)

SET(Tano_Mobile_Resrcs

)

################
# Qt4 settings #
################
# This line adds the QtDECLARATIVE libraries to the Qt libraries
# loaded when linking Tano Mobile
SET(QT_USE_QTDECLARATIVE True)

# This scripts runs the resource compiler rcc.
QT4_ADD_RESOURCES(Tano_Mobile_Resrcs_Srcs ${Tano_Mobile_Resrcs})

# This script runs the meta object compiler moc.
QT4_WRAP_CPP(Tano_Mobile_MOC_Srcs ${Tano_Mobile_MOCs})


#########################
# Compiler Define flags #
#########################
# Define we are compiling mobile application
ADD_DEFINITIONS(-DMOBILE=1)

# Define default data dir, if under POSIX systems.
IF(NOT MINGW)
    IF (NOT DEFAULT_DATA_DIR)
        ADD_DEFINITIONS(-DDEFAULT_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
    ELSE (NOT DEFAULT_DATA_DIR)
        ADD_DEFINITIONS(-DDEFAULT_DATA_DIR="${DEFAULT_DATA_DIR}")
    ENDIF (NOT DEFAULT_DATA_DIR)
ENDIF(NOT MINGW)

# Define default plugin dir, if under POSIX systems.
IF(NOT MINGW)
    IF (NOT DEFAULT_PLUGIN_DIR)
        ADD_DEFINITIONS(-DDEFAULT_PLUGIN_DIR="${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_PLUGIN_DIR}")
    ELSE (NOT DEFAULT_PLUGIN_DIR)
        ADD_DEFINITIONS(-DDEFAULT_PLUGIN_DIR="${DEFAULT_PLUGIN_DIR}")
    ENDIF (NOT DEFAULT_PLUGIN_DIR)
ENDIF(NOT MINGW)


####################################
# Include paths and set up targets #
####################################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} "${CMAKE_BINARY_DIR}/src" "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/mobile")

# This line adds the include path of the Qt
# directory to the compiler include path
INCLUDE(${QT_USE_FILE})

# This line tells cmake to create the Tano Mobile program.
ADD_EXECUTABLE(tano-mobile ${Tano_Mobile_Srcs} ${Tano_Mobile_MOC_Srcs} ${Tano_Mobile_Resrcs_Srcs})

# Here the needed Qt Libraries are added to the link command.
TARGET_LINK_LIBRARIES(tano-mobile ${QT_LIBRARIES})


################
# Installation #
################
# Install tano-mobile exe
#INSTALL(TARGETS tano-mobile
#    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
#)
