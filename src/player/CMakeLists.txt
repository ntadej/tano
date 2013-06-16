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

IF(MINGW OR MSVC)
    IF(BRANDING)
        CONFIGURE_FILE(
            ${CMAKE_SOURCE_DIR}/branding/windows/tano.rc.in
            ${CMAKE_BINARY_DIR}/src/player/tano.rc
        )
    ELSE(BRANDING)
        CONFIGURE_FILE(
            ${CMAKE_SOURCE_DIR}/src/player/tano.rc.in
            ${CMAKE_BINARY_DIR}/src/player/tano.rc
        )
    ENDIF(BRANDING)
ENDIF(MINGW OR MSVC)

SET(Tano_Srcs
    main.cpp
)

SET(Tano_SingleApplication_Srcs
    ../../lib/qtsingleapplication/src/qtlocalpeer.cpp
    ../../lib/qtsingleapplication/src/qtsingleapplication.cpp
    ../../lib/qtsingleapplication/src/qtsinglecoreapplication.cpp
)

#####################
# Compiler settings #
#####################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/widgets
    ${CMAKE_SOURCE_DIR}/lib/qtsingleapplication/src
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
)

# Include additional libraries
INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})

# Compile resources with windres
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/player/tano.obj
        COMMAND ${CMAKE_RC_COMPILER}
        -i ${CMAKE_BINARY_DIR}/src/player/tano.rc
        -o ${CMAKE_BINARY_DIR}/src/player/tano.obj
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/player"
    )
    SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_BINARY_DIR}/src/player/tano.obj)
ENDIF(MINGW)

IF(MSVC)
    SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_BINARY_DIR}/src/player/tano.rc)
ENDIF(MSVC)

# This line tells cmake to create the Tano program.
IF(MINGW OR MSVC)
    ADD_EXECUTABLE(tano WIN32 ${Tano_Srcs} ${Tano_SingleApplication_Srcs})
ELSE(MINGW OR MSVC)
    ADD_EXECUTABLE(tano ${Tano_Srcs} ${Tano_SingleApplication_Srcs})
ENDIF(MINGW OR MSVC)
ADD_DEPENDENCIES(tano ${TANO_CORE} ${TANO_WIDGETS})

# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET_TARGET_PROPERTIES(tano PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ".")
    SET_TARGET_PROPERTIES(tano PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET_TARGET_PROPERTIES(tano PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET_TARGET_PROPERTIES(tano PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET_TARGET_PROPERTIES(tano PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET_TARGET_PROPERTIES(tano PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF(MSVC)

# Here the needed Libraries are added to the link command.
IF(QT5)
    QT5_USE_MODULES(tano Network Widgets)
    TARGET_LINK_LIBRARIES(tano ${TANO_CORE_LIB} ${TANO_WIDGETS_LIB})
ELSE(QT5)
    TARGET_LINK_LIBRARIES(tano ${TANO_CORE_LIB} ${TANO_WIDGETS_LIB} ${QT_LIBRARIES})
ENDIF(QT5)


################
# Installation #
################
# Install tano exe
INSTALL(TARGETS tano
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
)
