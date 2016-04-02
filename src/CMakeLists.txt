#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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
# Configuration #
#################
CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/src/Config.h.in
    ${CMAKE_BINARY_DIR}/src/Config.h
)

IF(MSVC)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/src/Project.rc.in
        ${CMAKE_BINARY_DIR}/src/Project.rc
    )
ENDIF()

INCLUDE(ProjectI18N)
INCLUDE(ProjectSources)
INCLUDE(ProjectResources)
INCLUDE(ProjectUi)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(Project_Srcs
        ${Project_Srcs}
        ../resources/dsa_pub.pem
        ../resources/style/DefaultTheme.ini
    )
    SET_SOURCE_FILES_PROPERTIES (
        ../resources/dsa_pub.pem
        ../resources/style/DefaultTheme.ini
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )

    SET(Project_Iconset ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_LOWER}/resources/logo/Project.iconset)

    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.app/Contents/Resources)
    FIND_PROGRAM(ICONUTIL_EXECUTABLE NAMES iconutil)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.app/Contents/Resources/${PROJECT_NAME}.icns
        COMMAND ${ICONUTIL_EXECUTABLE} --convert icns --output ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.app/Contents/Resources/${PROJECT_NAME}.icns
        ${Project_Iconset}
    )
    SET(Project_Srcs ${Project_Srcs} ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.app/Contents/Resources/${PROJECT_NAME}.icns)
ENDIF()


#####################
# Compiler settings #
#####################
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET_SOURCE_FILES_PROPERTIES(application/Updates.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
    SET_SOURCE_FILES_PROPERTIES(settings/widgets/SettingsDialog.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/widgets
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
)

IF(MSVC)
    SET(Project_Srcs ${Project_Srcs} ${CMAKE_BINARY_DIR}/src/Project.rc)
ENDIF()

# This line tells cmake to create the Project program.
ADD_EXECUTABLE(${PROJECT_NAME} MACOSX_BUNDLE WIN32 ${Project_UIs} ${Project_Srcs} ${Project_Resources})

# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_BINARY_DIR}/platform/osx/Info.plist")
   TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${APPLICATIONSERVICES_LIBRARY} ${COCOA_LIBRARY} ${SPARKLE_LIBRARY})
   TARGET_LINK_LIBRARIES(${PROJECT_NAME} Qt5::MacExtras)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME_LOWER})
   TARGET_LINK_LIBRARIES(${PROJECT_NAME} Qt5::DBus)
ENDIF()

IF(MSVC)
    INCLUDE_DIRECTORIES(${WINSPARKLE_INCLUDE})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${WINSPARKLE_LIBRARY})
ENDIF()

# Here the needed Libraries are added to the link command.
TARGET_LINK_LIBRARIES(${PROJECT_NAME} VLCQt::Widgets
                      Qt5::Concurrent Qt5::Gui Qt5::Network Qt5::PrintSupport Qt5::Sql Qt5::Widgets Qt5::Xml)


################
# Installation #
################
# Install exe
INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_BIN_DIR}
    BUNDLE DESTINATION .
)
