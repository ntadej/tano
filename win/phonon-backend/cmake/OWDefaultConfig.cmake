# Defines general default CMake configuration options
#
# Copyright (C) 2006-2007  Wengo
# Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING file.


# Mininum required CMake version
cmake_minimum_required(VERSION 2.6)

# This option is OFF for optimization reasons (compilation time)
# Always include srcdir and builddir in include path
# This saves typing ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} in
# about every subdir
# Since CMake 2.4.0
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

# By default CMake appends the directories onto the current list of directories
# This default behavior can be changed by setting CMAKE_INCLUDE_DIRECTORIES_BEFORE to ON
# It puts the include dirs which are in the source or build tree
# before all other include dirs, so the headers in the sources
# are prefered over the already installed ones
# Since CMake 2.4.1
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

# Use colored output
# Since CMake 2.4.0
set(CMAKE_COLOR_MAKEFILE ON)
