#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

PROJECT(${TANO_CORE})

# Add definition for exporting symbols
ADD_DEFINITIONS(-DTANO_CORE_LIBRARY)

# Add definitions for shared library
ADD_DEFINITIONS(-DQT_SHARED -D_REENTRANT)


###################
# List of sources #
###################
# Define the QRC source files used by Tano
SET(Tano_Core_Resrcs
    ../../info.qrc
    ../../data/logo/logo.qrc
    ../../data/misc/misc.qrc
)

# Define the C++ source files used by Tano Core
SET(Tano_Core_Network_Srcs
    network/NetworkDownload.cpp
    network/NetworkRequest.cpp
    network/NetworkUdpxy.cpp
)
SET(Tano_Core_Playlist_Srcs
    playlist/PlaylistFilterModel.cpp
    playlist/PlaylistModel.cpp
    playlist/PlaylistOpen.cpp
    playlist/PlaylistSave.cpp
    playlist/PlaylistUpdate.cpp
    playlist/containers/Channel.cpp
    playlist/generators/CSVGenerator.cpp
    playlist/generators/JsGenerator.cpp
    playlist/generators/M3UGenerator.cpp
    playlist/generators/TvheadendGenerator.cpp
    playlist/generators/XmltvIdGenerator.cpp
    playlist/handlers/CSVHandler.cpp
    playlist/handlers/JsHandler.cpp
    playlist/handlers/M3UHandler.cpp
)
SET(Tano_Core_Settings_Srcs
    settings/Settings.cpp
    settings/SettingsChannel.cpp
    settings/SettingsShortcuts.cpp
)
SET(Tano_Core_Timers_Srcs
    timers/TimersGenerator.cpp
    timers/TimersHandler.cpp
    timers/TimersTimeManager.cpp
    timers/containers/Timer.cpp
    timers/models/TimersFilterModel.cpp
    timers/models/TimersModel.cpp
)
SET(Tano_Core_Xmltv_Srcs
    xmltv/XmltvCommon.cpp
    xmltv/XmltvCommonCategories.cpp
#    xmltv/XmltvGenerator.cpp
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
    SET(Tano_Core_Platform_Srcs
        ../platform/linux/Locations.cpp
    )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(MINGW OR MSVC)
    SET(Tano_Core_Platform_Srcs
        ../platform/windows/Console.cpp
    )
ENDIF(MINGW OR MSVC)

SET(Tano_Core_Srcs
    Arguments.cpp
    Common.cpp
    File.h
    Out.cpp
    ListItem.h
    ListModel.cpp
    LocaleManager.cpp
    Log.cpp
    Resources.cpp
    UpdateHandler.cpp

    CoreSharedExport.h

    ${Tano_Core_Network_Srcs}
    ${Tano_Core_Playlist_Srcs}
    ${Tano_Core_Settings_Srcs}
    ${Tano_Core_Timers_Srcs}
    ${Tano_Core_Xmltv_Srcs}

    ${Tano_Core_Platform_Srcs}
)

IF(CUSTOM_SETTINGS)
    SET(Tano_Core_Srcs
        ${Tano_Core_Srcs}

        ../custom/SettingsDefaults.cpp
        ../custom/SettingsShortcutsDefaults.cpp
    )
ELSE(CUSTOM_SETTINGS)
    SET(Tano_Core_Srcs
        ${Tano_Core_Srcs}

        settings/SettingsDefaults.cpp
        settings/SettingsShortcutsDefaults.cpp
    )
ENDIF(CUSTOM_SETTINGS)


###############
# Qt settings #
###############
# This script runs the resource compiler rcc
IF(QT5)
    QT5_ADD_RESOURCES(Tano_Core_Resrcs_Srcs ${Tano_Core_Resrcs})
ELSE(QT5)
    QT4_ADD_RESOURCES(Tano_Core_Resrcs_Srcs ${Tano_Core_Resrcs})
ENDIF(QT5)


#####################
# Compiler settings #
#####################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_BINARY_DIR}/src
)

# Compile resources with windres
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/platform/windows/tano-core.obj
        COMMAND ${CMAKE_RC_COMPILER}
        -i ${CMAKE_SOURCE_DIR}/src/platform/windows/tano-core.rc
        -o ${CMAKE_BINARY_DIR}/src/platform/windows/tano-core.obj
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/platform/windows"
    )
    SET(Tano_Core_Srcs ${Tano_Core_Srcs} ${CMAKE_BINARY_DIR}/src/platform/windows/tano-core.obj)
ENDIF(MINGW)

IF(MSVC)
    SET(Tano_Core_Srcs ${Tano_Core_Srcs} ${CMAKE_SOURCE_DIR}/src/platform/windows/tano-core.rc)
ENDIF(MSVC)

# This line tells cmake to create the Tano program.
ADD_LIBRARY(${TANO_CORE} SHARED ${Tano_Core_Srcs} ${Tano_Core_Resrcs_Srcs})

# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF(MSVC)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES VERSION ${TANO_VERSION_MAJOR}
                                       SOVERSION "${TANO_VERSION_MAJOR}.${TANO_VERSION_MINOR}"
    )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Here the needed Libraries are added to the link command.
IF(QT5)
    QT5_USE_MODULES(${TANO_CORE} Concurrent Gui Network Sql Xml)
ELSE(QT5)
    TARGET_LINK_LIBRARIES(${TANO_CORE} ${QT_LIBRARIES})
ENDIF(QT5)


################
# Installation #
################
INSTALL(TARGETS ${TANO_CORE} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
)
