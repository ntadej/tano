# Toolchain for cross-compiling Tano using MinGW i686
#
# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)
 
# which compilers to use for C and C++
SET(CMAKE_C_COMPILER i686-pc-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-pc-mingw32-g++)
 
# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  /usr/i686-pc-mingw32 ~/vlc-sdk-win32)
 
# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
 
# FindQt4.cmake querys qmake to get information, this doesn't work when crosscompiling
set(MINGW_PREFIX    /usr/i686-pc-mingw32)
set(QT_BINARY_DIR   /usr/bin)
set(QT_LIBRARY_DIR  ${MINGW_PREFIX}/lib)
set(QT_LIBRARYB_DIR  ${MINGW_PREFIX}/bin)
set(QT_INCLUDES ${MINGW_PREFIX}/include/qt4)
set(QT_QTCORE_LIBRARY   ${MINGW_PREFIX}/lib/libQtCore4.a)
set(QT_QTCORE_INCLUDE_DIR ${MINGW_PREFIX}/include/qt4/QtCore)
set(QT_QTGUI_INCLUDE_DIR ${MINGW_PREFIX}/include/qt4/QtGui)
set(QT_QTNETWORK_INCLUDE_DIR ${MINGW_PREFIX}/include/qt4/QtNetwork)
set(QT_QTXML_INCLUDE_DIR ${MINGW_PREFIX}/include/qt4/QtXml)
set(QT_MKSPECS_DIR  ${MINGW_PREFIX}/mkspecs)
set(QT_MOC_EXECUTABLE  ${QT_BINARY_DIR}/moc)
set(QT_QMAKE_EXECUTABLE  ${QT_BINARY_DIR}/qmake)
set(QT_UIC_EXECUTABLE  ${QT_BINARY_DIR}/uic)

set(WINDRES_PROGRAM i686-pc-mingw32-windres)