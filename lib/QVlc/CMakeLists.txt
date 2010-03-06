PROJECT(qvlc)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake
)

# Set MingW bin dir, if MingW present
IF(MINGW)
	STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" MINGW_BIN_DIR_TMP "${CMAKE_CXX_COMPILER}")
	STRING(REGEX REPLACE "\\\\" "/" MINGW_BIN_DIR "${MINGW_BIN_DIR_TMP}") # Replace back slashes to slashes
ENDIF(MINGW)

# Add required libraries
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(LIBVLC REQUIRED)

# libQVlc version number
FILE(READ VERSION QVLC_VERSION)
STRING(REGEX REPLACE "\n" "" QVLC_VERSION "${QVLC_VERSION}") # get rid of the newline at the end
MESSAGE("You are compiling libQVlc ${QVLC_VERSION}")

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
	Subversion_GET_REVISION(${CMAKE_CURRENT_SOURCE_DIR} QVLC_WC_REVISION)
	MESSAGE("Current revision is ${QVLC_WC_REVISION}")
ELSE(Subversion_FOUND)
	SET(QVLC_WC_REVISION "")
ENDIF(Subversion_FOUND)

OPTION(VLC_TRUNK "Use LibVLC trunk features")
MESSAGE("Libvlc: ${VLC_TRUNK}")

OPTION(UNIX_PACKAGE "UNIX lib.so.version features")

#------------------------------------------------------------------------------

IF(${VLC_TRUNK} MATCHES "ON")
ADD_DEFINITIONS(-DVLC_TRUNK=1)
SET(VLC_TRUNK_INT 1)
ELSE(${VLC_TRUNK} MATCHES "ON")
SET(VLC_TRUNK_INT 0)
ENDIF(${VLC_TRUNK} MATCHES "ON")

CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/Version.cpp.in"
	"${CMAKE_CURRENT_BINARY_DIR}/Version.cpp"
)
CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/VlcConfig.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/VlcConfig.h"
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# Set your list of sources here.
SET(QVLC_SRCS
	VlcControl.cpp
	VlcInstance.cpp
	VlcSeekWidget.cpp
	VlcVideoWidget.cpp
	VlcVolumeSlider.cpp

	${CMAKE_CURRENT_BINARY_DIR}/Version.cpp
)

SET(QVLC_MOCS
	VlcControl.h
	VlcInstance.h
	VlcSeekWidget.h
	VlcVideoWidget.h
	VlcVolumeSlider.h
)

SET(QVLC_HEADERS
	VlcControl.h
	VlcInstance.h
	VlcSeekWidget.h
	VlcVideoWidget.h
	VlcVolumeSlider.h

	${CMAKE_CURRENT_BINARY_DIR}/VlcConfig.h
)

INCLUDE(${QT_USE_FILE})
QT4_WRAP_CPP(QVLC_MOCS_SRCS ${QVLC_MOCS})

INCLUDE_DIRECTORIES(${LIBVLC_INCLUDE_DIR})

# Create the library.
ADD_LIBRARY(${PROJECT_NAME} SHARED ${QVLC_SRCS} ${QVLC_MOCS_SRCS})

IF(UNIX_PACKAGE)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION 0.2.1
									  SOVERSION 0.2)
ENDIF(UNIX_PACKAGE)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QT_LIBRARIES} ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})

# Install files
INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin
					LIBRARY DESTINATION lib${LIB_SUFFIX}
					ARCHIVE DESTINATION lib${LIB_SUFFIX})
INSTALL(FILES ${QVLC_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/QVlc)

