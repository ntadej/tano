#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2015 Tadej Novak <tadej@tano.si>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>./
#############################################################################
# Find the architecture
SET(BITS 0)
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(BITS 1)
ENDIF()

# Automatically run moc, uic and/or rcc, if required
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)
# Verbose makefile
SET(CMAKE_VERBOSE_MAKEFILE ON)

# C++11 support
IF (NOT MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF()

# Show all warnings
IF(NOT MSVC)
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
ENDIF()

# Make final release build smaller
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
ENDIF()

IF(MSVC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nologo -Zc:wchar_t")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nologo -Zc:wchar_t -w34100 -w34189")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NOLOGO /DYNAMICBASE /NXCOMPAT")
ENDIF()
