# Shows general debug informations (OS, processor, compiler, build type...)
#
# Copyright (C) 2006-2007  Wengo
# Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING file.


message(STATUS "System: " ${SYSTEM_NAME})
message(STATUS "Processor: " ${CMAKE_SYSTEM_PROCESSOR})
message(STATUS "Compiler: " ${COMPILER_NAME})
message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})
message(STATUS "Build Tool: " ${CMAKE_BUILD_TOOL})
message(STATUS "Build Directory: " ${BUILD_DIR})
message(STATUS "SVN Revision: " ${SVN_REVISION})
