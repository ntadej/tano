#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

SET(Tano_Srcs
    main.cpp
)

#####################
# Compiler settings #
#####################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib/qtsingleapplication/src
)

# Compile resources with windres
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/windows.obj
        COMMAND ${CMAKE_RC_COMPILER}
        -i ${CMAKE_SOURCE_DIR}/src/windows.rc
        -o ${CMAKE_BINARY_DIR}/src/windows.obj
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src"
    )
    SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_BINARY_DIR}/src/windows.obj)
ENDIF(MINGW)

# This line tells cmake to create the Tano program.
ADD_EXECUTABLE(tano ${Tano_Srcs})
ADD_DEPENDENCIES(tano qtsingleapplication ${TANO_CORE} ${TANO_WIDGETS})

# Here the needed Libraries are added to the link command.
IF(WITH_QT5)
    TARGET_LINK_LIBRARIES(tano ${QT_SINGLE_APPLICATION} ${TANO_CORE_LIB} ${TANO_WIDGETS_LIB})
ELSE(WITH_QT5)
    TARGET_LINK_LIBRARIES(tano
                         ${QT_SINGLE_APPLICATION}
                         ${QT_LIBRARIES}
                         ${TANO_CORE_LIB} ${TANO_WIDGETS_LIB}
    )
ENDIF(WITH_QT5)


################
# Installation #
################
# Install tano exe
INSTALL(TARGETS tano
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
)
