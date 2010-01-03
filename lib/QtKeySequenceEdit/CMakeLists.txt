PROJECT(QtKeySequenceEdit)

IF(MINGW)
add_definitions(-DCMAKE_TOOLCHAIN=../../Toolchain-win32.cmake)
ENDIF(MINGW)

# Set your list of sources here.
SET(QKEY_EDITOR_SRCS
    QtKeySequenceEdit.cpp
)

SET(QKEY_EDITOR_MOCS
    QtKeySequenceEdit.h
)

# Search for Qt4. We need the latest version of Qt
SET(QT_MIN_VERSION "4.4.0")
# this line includes FindQt4.cmake, which searches the Qt library and headers
FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})
QT4_WRAP_CPP(QKEY_EDITOR_MOCS_SRCS ${QKEY_EDITOR_MOCS})

# Create the library.
ADD_LIBRARY(${PROJECT_NAME} STATIC ${QKEY_EDITOR_SRCS} ${QKEY_EDITOR_MOCS_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QT_LIBRARIES})