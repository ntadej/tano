#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2008-2010 Tadej Novak <tadej@tano.si>
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

PROJECT(Tano-Recorder)
cmake_minimum_required(VERSION 2.6)

###################
# List of sources #
###################
# Define the MOC source files used by Tano Recorder
SET(Tano_Recorder_MOCs	# List of classes which Qt needs to produce MOCs for them (in order for events, signals, slots and other macros to work
	RecorderCore.h
	TimeManager.h

)

SET(Tano_Recorder_Resrcs

)

# Define the C++ source files used by Tano
SET(Tano_Recorder_Srcs
	main.cpp

	RecorderCore.cpp
	TimeManager.cpp

	../src/container/Timer.cpp

	../src/core/Common.cpp
	../src/core/Enums.cpp
	../src/core/PluginsLoader.cpp
	../src/core/Settings.cpp
	../src/core/Version.cpp
)

################
# Qt4 settings #
################
# This line adds the QtXML and QtNETWORK libraries to the Qt libraries
# loaded when linking Tano
#SET(QT_USE_QTXML True)
#SET(QT_USE_QTNETWORK True)

# This scripts runs the resource compiler rcc.
QT4_ADD_RESOURCES(Tano_Recorder_Resrcs_Srcs ${Tano_Recorder_Resrcs})

# This script runs the meta object compiler moc.
QT4_WRAP_CPP(Tano_Recorder_MOC_Srcs ${Tano_Recorder_MOCs})


#########################
# Compiler Define flags #
#########################
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
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} "${CMAKE_BINARY_DIR}/src" "${CMAKE_SOURCE_DIR}/src")

# This line adds the include path of the Qt
# directory to the compiler include path
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})

IF(MINGW)
	# CMake currently doesn't support windres yet. Add command manually
	FIND_PROGRAM(
		WINDRES_PROGRAM windres.exe PATHS
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/bin"
		c:/MinGW/bin /MinGW/bin
	)
	ADD_CUSTOM_COMMAND(
		OUTPUT ${CMAKE_BINARY_DIR}/src/tano.obj
		COMMAND ${WINDRES_PROGRAM}
		-i ${CMAKE_SOURCE_DIR}/src/tano.rc
		-o ${CMAKE_BINARY_DIR}/src/tano.obj
		WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src"
	)
	SET(Tano_Recorder_Srcs ${Tano_Recorder_Srcs} ${CMAKE_BINARY_DIR}/src/tano.obj)
ENDIF(MINGW)


# This line tells cmake to create the Tano Recorder program.
ADD_EXECUTABLE(tano-recorder ${Tano_Recorder_Srcs} ${Tano_Recorder_MOC_Srcs} ${Tano_Recorder_Resrcs_Srcs})

# Here the needed Qt Libraries are added to the link command.
TARGET_LINK_LIBRARIES(tano-recorder ${QT_LIBRARIES} ${LIBVLCQT_LIBRARY})


################
# Installation #
################
# Install tano-recorder exe
INSTALL(TARGETS tano-recorder
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
)

###############################
# Compile and install plugins #
###############################
ADD_SUBDIRECTORY(plugins)
