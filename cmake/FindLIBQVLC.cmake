# CMake module to search for QVLC (Qt and libVLC connector library)
# Authors: Rohit Yadav <rohityadav89@gmail.com>
#          Tadej Novak <ntadej@users.sourceforge.net>
#
# If it's found it sets LIBQVLC_FOUND to TRUE
# and following variables are set:
#    LIBQVLC_INCLUDE_DIR
#    LIBQVLC_LIBRARY


# FIND_PATH and FIND_LIBRARY normally search standard locations
# before the specified paths. To search non-standard paths first,
# FIND_* is invoked first with specified paths and NO_DEFAULT_PATH
# and then again with no specified paths to search the default
# locations. When an earlier FIND_* succeeds, subsequent FIND_*s
# searching for the same item do nothing.

#Put here path to custom location
#example: /home/user/QVlc/include etc..
FIND_PATH(LIBQVLC_INCLUDE_DIR QVlc/VlcInstance.h
  "$ENV{LIBQVLC_INCLUDE_PATH}"
  "$ENV{LIB_DIR}/include"
  "$ENV{LIB_DIR}/include/QVlc"
  "/usr/include"
  "/usr/include/QVlc"
  "/usr/local/include"
  "/usr/local/include/QVlc"
  #mingw
  c:/msys/local/include
  NO_DEFAULT_PATH
  )
FIND_PATH(LIBQVLC_INCLUDE_DIR VlcInstance.h)

#Put here path to custom location
#example: /home/user/vlc/lib etc..
FIND_LIBRARY(LIBQVLC_LIBRARY NAMES QVlc PATHS
  "$ENV{LIBQVLC_LIBRARY_PATH}"
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  NO_DEFAULT_PATH
  )
FIND_LIBRARY(LIBQVLC_LIBRARY NAMES QVlc)

IF (LIBQVLC_INCLUDE_DIR AND LIBQVLC_LIBRARY)
   SET(LIBQVLC_FOUND TRUE)
ENDIF (LIBQVLC_INCLUDE_DIR AND LIBQVLC_LIBRARY)

IF (LIBQVLC_FOUND)
   IF (NOT LIBQVLC_FIND_QUIETLY)
      MESSAGE(STATUS "Found libQVlc include-dir path: ${LIBQVLC_INCLUDE_DIR}")
      MESSAGE(STATUS "Found libQVlc library path: ${LIBQVLC_LIBRARY}")
   ENDIF (NOT LIBQVLC_FIND_QUIETLY)
ELSE (LIBQVLC_FOUND)
   IF (LIBQVLC_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find libQVlc")
   ENDIF (LIBQVLC_FIND_REQUIRED)
ENDIF (LIBQVLC_FOUND)
