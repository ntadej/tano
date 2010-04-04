###############################
# Tano - An open IP TV player #
###############################
# The name of our project is "Tano". CMakeLists files in this project can
# refer to the root source directory of the project as ${TANO_SOURCE_DIR}
# and to the root binary directory of the project as ${TANO_BINARY_DIR}.

PROJECT(Tano)
cmake_minimum_required(VERSION 2.6)

CONFIGURE_FILE(
  "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
)

# Set MingW bin dir, if MingW present
IF(MINGW)
	STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" MINGW_BIN_DIR_TMP "${CMAKE_CXX_COMPILER}")
	STRING(REGEX REPLACE "\\\\" "/" MINGW_BIN_DIR "${MINGW_BIN_DIR_TMP}") # Replace back slashes to slashes
ENDIF(MINGW)

IF(MINGW)
OPTION(DEBUG_CONSOLE "Compile with debug console")
	IF(${DEBUG_CONSOLE} MATCHES "ON")
		SET(TANO_DEBUG_CONSOLE 1)
	ELSE(${DEBUG_CONSOLE} MATCHES "ON")
		SET(CMAKE_EXE_LINKER_FLAGS "-enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -mthreads -Wl -Wl,-subsystem,windows")
		SET(TANO_DEBUG_CONSOLE 0)
	ENDIF(${DEBUG_CONSOLE} MATCHES "ON")
ELSE(MINGW)
	SET(TANO_DEBUG_CONSOLE 0)
ENDIF(MINGW)

# Search for Qt4 and other libraries
SET(QT_MIN_VERSION "4.5.0")

FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(LIBQVLC REQUIRED)

# Set Qt4 bin dir to find QtCore4.dll and other libs to install
STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" QT_BIN_DIR_TMP "${QT_QMAKE_EXECUTABLE}")
STRING(REGEX REPLACE "\\\\" "/" QT_BIN_DIR "${QT_BIN_DIR_TMP}")  # Replace back slashes to slashes

# Set default install path:
# In *nix environments this is /usr/local 
# Under M$ Windows this is C:\Program files\Tano
# WARNING: Leading slash should be added - this is for easier concatenation of the strings due to slash/backslash issues
IF(MINGW)
	IF (NOT CMAKE_INSTALL_PREFIX)
		SET(CMAKE_INSTALL_PREFIX "C:/Program files/Tano")
	ENDIF (NOT CMAKE_INSTALL_PREFIX)
	SET(TANO_INSTALL_DATA_DIR "")
	SET(TANO_INSTALL_PLUGIN_DIR "plugins")
	SET(TANO_INSTALL_BIN_DIR "")
ELSE(MINGW)
	IF (NOT CMAKE_INSTALL_PREFIX)
		SET(CMAKE_INSTALL_PREFIX "/usr/local")
	ENDIF (NOT CMAKE_INSTALL_PREFIX)
	SET(TANO_INSTALL_DATA_DIR "share/tano")
	SET(TANO_INSTALL_PLUGIN_DIR "lib/tano")
	SET(TANO_INSTALL_BIN_DIR "bin")
ENDIF(MINGW)


###########
# Version #
###########
# Tano version number.
FILE(READ VERSION TANO_VERSION)
STRING(REGEX REPLACE "\n" "" TANO_VERSION "${TANO_VERSION}") # get rid of the newline at the end
MESSAGE("You are compiling Tano ${TANO_VERSION}")

# SVN revision
MACRO(Subversion_GET_REVISION dir variable)
  EXECUTE_PROCESS(COMMAND ${Subversion_SVN_EXECUTABLE} info ${dir}
    OUTPUT_VARIABLE ${variable}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  STRING(REGEX REPLACE "^(.*\n)?Revision: ([^\n]+).*"
    "\\2" ${variable} "${${variable}}")
ENDMACRO(Subversion_GET_REVISION)

FIND_PACKAGE(Subversion)
IF(Subversion_FOUND)
	Subversion_GET_REVISION(${CMAKE_SOURCE_DIR} TANO_WC_REVISION)
    MESSAGE("Current revision is ${TANO_WC_REVISION}")
ELSE(Subversion_FOUND)
    SET(TANO_WC_REVISION "")
ENDIF(Subversion_FOUND)


#################
# Configuration #
#################
OPTION(REFRESH_TRANSLATIONS "Refresh translations on compile")
MESSAGE("Refresh translations on compile: ${REFRESH_TRANSLATIONS}")
OPTION(PORTABLE "Compile as PortableApps application")
MESSAGE("Compile as PortableApps application: ${PORTABLE}")
IF(${PORTABLE} MATCHES "ON")
SET(PORTABLEAPP 1)
ELSE(${PORTABLE} MATCHES "ON")
SET(PORTABLEAPP 0)
ENDIF(${PORTABLE} MATCHES "ON")


###################
# Compile and set #
###################
ADD_SUBDIRECTORY(lang)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(plugins)

SET (Tano_Files
	LICENSE.GPL
	src/ui/images/tano.png
	src/ui/images/tano-small.png
)

FILE (GLOB Tano_Playlists ${CMAKE_SOURCE_DIR}/playlists/*.m3u)


################
# Installation #
################
# Install data
INSTALL( FILES ${Tano_Files} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}")
# Install playlists
INSTALL( FILES ${Tano_Playlists} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/playlists")
