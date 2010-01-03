PROJECT(QVlc)

IF(MINGW)
add_definitions(-DCMAKE_TOOLCHAIN=../../Toolchain-win32.cmake)
ENDIF(MINGW)

# Set your list of sources here.
SET(QVLC_SRCS
    VlcControl.cpp
    VlcInstance.cpp
    VlcSeekWidget.cpp
    VlcVideoWidget.cpp
    VlcVolumeSlider.cpp
)

SET(QVLC_MOCS
    VlcControl.h
    VlcInstance.h
    VlcSeekWidget.h
    VlcVideoWidget.h
    VlcVolumeSlider.h
)

# Search for Qt4. We need the latest version of Qt
SET(QT_MIN_VERSION "4.4.0")
# this line includes FindQt4.cmake, which searches the Qt library and headers
FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})
QT4_WRAP_CPP(QVLC_MOCS_SRCS ${QVLC_MOCS})

find_package(VLC REQUIRED)
if (VLC_FOUND)
	include_directories(${VLC_INCLUDE_DIRS})
endif (VLC_FOUND)

# Create the library.
ADD_LIBRARY(${PROJECT_NAME} STATIC ${QVLC_SRCS} ${QVLC_MOCS_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QT_LIBRARIES} ${VLC_LIBRARIES})