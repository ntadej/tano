# Defines global public variables
#
# CMAKE_BUILD_TYPE = set to Debug by default
# BUILD_TYPE = same as CMAKE_BUILD_TYPE but lowercase (debug, release, minsizerel...)
# BUILD_NAME = build name used by BUILD_DIR
# BUILD_DIR = directory where compiled files will be copied, ${CMAKE_BINARY_DIR}/${BUILD_NAME} by default
# SVN_REVISION = subversion revision number, see ow_get_svn_revision()
# CURRENT_DATE_TIME = current date time: 20070323121316 (e.g 2007-03-23 12:13:16)
#
# Useful CMake variables:
# http://www.cmake.org/Wiki/CMake_Useful_Variables
# All variables defined by CMake:
# http://www.cmake.org/Wiki/CMake_Useful_Variables/Get_Variables_From_CMake_Dashboards
#
# Copyright (C) 2006-2007  Wengo
# Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING file.


# Sets a global variable to test if OWBuild is already included or not in the build system
set(OWBUILD_INCLUDED TRUE)

# Sets the default build type to Debug if not already done
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE
		Debug
		CACHE STRING "Choose the build type, options are: None Debug Release RelWithDebInfo MinSizeRel" FORCE
	)
endif (NOT CMAKE_BUILD_TYPE)

# Directory where compiled files (install target) will be copied
# In lower case
# Use the system name (Linux, Windows, MacOS...) and compiler name (MSVC, GCC...)
set(BUILD_NAME ${SYSTEM_NAME}-${COMPILER_NAME}-${CMAKE_BUILD_TYPE})
string(TOLOWER ${BUILD_NAME} BUILD_NAME)
set(BUILD_DIR
	${CMAKE_BINARY_DIR}/${BUILD_NAME}
	CACHE PATH "Build directory, depends on build type, system name and OS name" FORCE
)

# Gets svn revision
set(SVN_REVISION "")
ow_get_svn_revision(SVN_REVISION)
# Bugfix with SVN revision number that can integrate a : and
# this does not work under Windows for the installer name, replace it by -
if (SVN_REVISION)
	string(REPLACE ":" "-" SVN_REVISION ${SVN_REVISION})
endif (SVN_REVISION)

# Gets current date time
set(CMAKE_BUILD_DATE "0")
ow_get_current_date_time(CMAKE_BUILD_DATE)
