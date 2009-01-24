#Mininum required CMake version
cmake_minimum_required(VERSION 2.6)

project(phonon-backend)

#Add options values
include(Options.cmake)

#OWBuild
include(CMakeLists-owbuild.txt)

#Default installation path
#and other platform specific installation instructions
set(CMAKE_INSTALL_PREFIX ${BUILD_DIR})
if (WIN32)
	include(CMakeLists-win32.txt)
endif (WIN32)
if (LINUX)
	include(CMakeLists-linux.txt)
endif (LINUX)
if (APPLE)
	include(CMakeLists-macos.txt)
endif (APPLE)
##

if (KDE4_FOUND)
	#include(KDE4Defaults)
	#include(MacroLibrary)

	add_definitions(${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS})
	include_directories(${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})
else (KDE4_FOUND)
	set(QT_MIN_VERSION 4.4.0)
	find_package(Qt4 REQUIRED)

	#Includes all wanted definitions for Qt
	include(${QT_USE_FILE})

	#Ok, we can set NEW_TITLE_CHAPTER_HANDLING as we have a patched Phonon library
	set(NEW_TITLE_CHAPTER_HANDLING 1)

	message(STATUS "KDE4 support disabled")
endif (KDE4_FOUND)


#Optimization: remove all Qt modules from include path
reduce_qt_include_dirs()
##

#3rdparty libraries with Qt and Phonon dependency
add_subdirectory(3rdparty/phonon)

#Do not use Phonon from Qt or KDE4
#Use Phonon from this repository instead (patched version)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(QT_PHONON_LIBRARY phonon)

#set(PHONON_LIBS phonon)
#add_subdirectory(3rdparty/ds9)

add_subdirectory(phonon-mplayer)
##

#CPack instructions
include(CPackConfig.txt)
