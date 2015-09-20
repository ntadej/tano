#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2014 Tadej Novak <tadej@tano.si>
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

IF(MSVC)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/src/core/tano-core.rc.in
        ${CMAKE_BINARY_DIR}/src/core/tano-core.rc
    )
ENDIF()


###################
# List of sources #
###################
# Define the C++ source files used by Tano Core
SET(Tano_Core_Application_Srcs
    application/Arguments.cpp
    application/LocaleManager.cpp
    application/LocalServer.cpp
    application/Log.cpp
    application/Output.cpp
)
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
    playlist/handlers/GoTVHandler.cpp
    playlist/handlers/JsHandler.cpp
    playlist/handlers/M3UHandler.cpp
)
SET(Tano_Core_Plugins_Srcs
    plugins/ConfigPlugin.h
    plugins/NetworkPlugin.h
    plugins/Plugins.cpp
)
SET(Tano_Core_Settings_Srcs
    settings/Settings.cpp
    settings/SettingsChannel.cpp
    settings/SettingsDefaults.cpp
    settings/SettingsKeys.cpp
    settings/SettingsPassword.cpp
    settings/SettingsShortcuts.cpp
    settings/SettingsShortcutsDefaults.cpp
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
    SET(Tano_Core_Platform_Srcs
        platform/Linux.cpp
    )
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(Tano_Core_Platform_Srcs
        platform/OSX.cpp
    )
ENDIF()

IF(MSVC)
    SET(Tano_Core_Platform_Srcs
        platform/Windows.cpp
    )
ENDIF()

SET(Tano_Core_Srcs
    Common.cpp
    File.h
    ListItem.h
    ListModel.cpp
    Resources.cpp
    UpdateHandler.cpp

    CoreSharedExport.h

    ${Tano_Core_Application_Srcs}
    ${Tano_Core_Network_Srcs}
    ${Tano_Core_Playlist_Srcs}
    ${Tano_Core_Plugins_Srcs}
    ${Tano_Core_Settings_Srcs}
    ${Tano_Core_Timers_Srcs}
    ${Tano_Core_Xmltv_Srcs}

    ${Tano_Core_Platform_Srcs}
)


#####################
# Compiler settings #
#####################
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    FIND_PATH(COREFOUNDATION_INCLUDE_DIR CoreFoundation/CoreFoundation.h)
    SET(COREFOUNDATION_LIBRARY "-weak_framework CoreFoundation")
    FIND_PATH(IOKIT_INCLUDE_DIR IOKit/IOKitLib.h)
    SET(IOKIT_LIBRARY "-weak_framework IOKit")

    INCLUDE_DIRECTORIES(${COREFOUNDATION_INCLUDE_DIR}/Headers)
    SET_SOURCE_FILES_PROPERTIES(platform/OSX.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
ENDIF()

# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_BINARY_DIR}/src/core
)

IF(MSVC)
    SET(Tano_Core_Srcs ${Tano_Core_Srcs} ${CMAKE_BINARY_DIR}/src/core/tano-core.rc)
ENDIF()

# This line tells cmake to create the Tano Core library.
ADD_LIBRARY(${TANO_CORE} SHARED ${Tano_Core_Srcs})

# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET_TARGET_PROPERTIES(${TANO_CORE} PROPERTIES VERSION ${TANO_VERSION_MAJOR}
                                       SOVERSION "${TANO_VERSION_MAJOR}.${TANO_VERSION_MINOR}"
    )
ENDIF()

# Here the needed Libraries are added to the link command.
TARGET_LINK_LIBRARIES(${TANO_CORE} Qt5::Concurrent Qt5::Gui Qt5::Network Qt5::Sql Qt5::Xml)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    TARGET_LINK_LIBRARIES(${TANO_CORE} ${COREFOUNDATION_LIBRARY} ${IOKIT_LIBRARY})
ENDIF()


################
# Installation #
################
INSTALL(TARGETS ${TANO_CORE} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_LIB_DIR}
)
