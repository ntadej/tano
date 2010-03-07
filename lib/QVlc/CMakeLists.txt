PROJECT(qvlc)
cmake_minimum_required(VERSION 2.6)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

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
SET(QVLC_VERSION "\"${QVLC_VERSION}\"")
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
	SET(QVLC_WC_REVISION 0)
ENDIF(Subversion_FOUND)

OPTION(VLC_TRUNK "Use LibVLC trunk features")
MESSAGE("Libvlc: ${VLC_TRUNK}")

#------------------------------------------------------------------------------

IF(${VLC_TRUNK} MATCHES "ON")
SET(VLC_1_1_INT 1)
SET(VLC_1_0_INT 0)
ELSE(${VLC_TRUNK} MATCHES "ON")
SET(VLC_1_1_INT 0)
SET(VLC_1_0_INT 1)
ENDIF(${VLC_TRUNK} MATCHES "ON")

CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/Config.h"
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# Set your list of sources here.
SET(QVLC_SRCS
	AudioControl.cpp
	Instance.cpp
	SeekWidget.cpp
	VideoControl.cpp
	VideoWidget.cpp
	VolumeSlider.cpp
)

SET(QVLC_MOCS
	AudioControl.h
	Instance.h
	SeekWidget.h
	VideoControl.h
	VideoWidget.h
	VolumeSlider.h
)

SET(QVLC_HEADERS
	${QVLC_MOCS}

	${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

INCLUDE(${QT_USE_FILE})
QT4_WRAP_CPP(QVLC_MOCS_SRCS ${QVLC_MOCS})

INCLUDE_DIRECTORIES(${LIBVLC_INCLUDE_DIR})

# Create the library.
ADD_LIBRARY(${PROJECT_NAME} SHARED ${QVLC_SRCS} ${QVLC_MOCS_SRCS})

IF(PACKAGE)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION 0.2.1
									  SOVERSION 0.2)
ENDIF(PACKAGE)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QT_LIBRARIES} ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})

# Install files
INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin
					LIBRARY DESTINATION lib${LIB_SUFFIX}
					ARCHIVE DESTINATION lib${LIB_SUFFIX})
INSTALL(FILES ${QVLC_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/QVlc)

