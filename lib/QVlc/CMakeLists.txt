PROJECT(QVlc)

IF(MINGW)
add_definitions(-DCMAKE_TOOLCHAIN=../../Toolchain-win32.cmake)
ENDIF(MINGW)

IF(${VLC_TRUNK} MATCHES "ON")
ADD_DEFINITIONS(-DVLC_TRUNK=1)
ENDIF(${VLC_TRUNK} MATCHES "ON")

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

INCLUDE(${QT_USE_FILE})
QT4_WRAP_CPP(QVLC_MOCS_SRCS ${QVLC_MOCS})

INCLUDE_DIRECTORIES(${LIBVLC_INCLUDE_DIR})

# Create the library.
ADD_LIBRARY(${PROJECT_NAME} STATIC ${QVLC_SRCS} ${QVLC_MOCS_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QT_LIBRARIES} ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})