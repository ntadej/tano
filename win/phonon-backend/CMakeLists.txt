#Mininum required CMake version
cmake_minimum_required(VERSION 2.6)

project(phonon-backend)

#OWBuild
include(CMakeLists-owbuild.txt)

#CMake declarations order matters!

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

set(QT_MIN_VERSION 4.4.0)
find_package(Qt4 REQUIRED)

#Includes all wanted definitions for Qt
include(${QT_USE_FILE})

#Optimization: remove all Qt modules from include path
reduce_qt_include_dirs()
##

#3rdparty libraries with Qt and Phonon dependency
add_subdirectory(3rdparty/phonon)

#Do not use Phonon from Qt or KDE4
#Use Phonon from this repository instead (patched version)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(QT_PHONON_LIBRARY phonon)

add_subdirectory(vlc)
##
