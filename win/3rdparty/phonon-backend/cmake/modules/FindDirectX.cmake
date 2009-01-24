# - Try to find DirectX
# Once done this will define
#
#  DIRECTX9_FOUND - system has DirectX 9
#  DIRECTX10_FOUND - system has DirectX 10
#  DIRECTX10_1_FOUND - system has DirectX 10.1
#  DIRECTX_INCLUDE_DIRS - the DirectX include directory
#  DIRECTX9_LIBRARIES - Link these to use DirectX 9
#  DIRECTX10_LIBRARIES - Link these to use DirectX 10
#  DIRECTX10_1_LIBRARIES - Link these to use DirectX 10.1

# Copyright (c) 2008 Tanguy Krotoff <tkrotoff@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if (DIRECTX_INCLUDE_DIRS)
  # in cache already
  set(DIRECTX_FOUND TRUE)
else (DIRECTX_INCLUDE_DIRS)

  find_path(DIRECTX_INCLUDE_DIR
    NAMES
      d3d9.h
    PATHS
      "$ENV{DXSDK_DIR}Include"
  )

  set(DIRECTX_LIBRARY_DIR
    "$ENV{DXSDK_DIR}/Lib/x86"
  )

  macro (_DIRECTX_ADJUST_LIB_VARS basename)
    # if only the release version was found, set the debug variable also to the release version
    if (${basename}_LIBRARY_RELEASE AND NOT ${basename}_LIBRARY_DEBUG)
      set(${basename}_LIBRARY_DEBUG ${${basename}_LIBRARY_RELEASE})
      set(${basename}_LIBRARY       ${${basename}_LIBRARY_RELEASE})
    endif (${basename}_LIBRARY_RELEASE AND NOT ${basename}_LIBRARY_DEBUG)

    # if only the debug version was found, set the release variable also to the debug version
    if (${basename}_LIBRARY_DEBUG AND NOT ${basename}_LIBRARY_RELEASE)
      set(${basename}_LIBRARY_RELEASE ${${basename}_LIBRARY_DEBUG})
      set(${basename}_LIBRARY         ${${basename}_LIBRARY_DEBUG})
    endif (${basename}_LIBRARY_DEBUG AND NOT ${basename}_LIBRARY_RELEASE)

    if (${basename}_LIBRARY_DEBUG AND ${basename}_LIBRARY_RELEASE)
        # if the generator supports configuration types then set
        # optimized and debug libraries, or if the CMAKE_BUILD_TYPE has a value
        if (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
          set(${basename}_LIBRARY       optimized ${${basename}_LIBRARY_RELEASE} debug ${${basename}_LIBRARY_DEBUG})
        else(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
          # if there are no configuration types and CMAKE_BUILD_TYPE has no value
          # then just use the release libraries
          set(${basename}_LIBRARY       ${${basename}_LIBRARY_RELEASE} )
        endif(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
    endif (${basename}_LIBRARY_DEBUG AND ${basename}_LIBRARY_RELEASE)
  endmacro (_DIRECTX_ADJUST_LIB_VARS)

  # Set D3D9_LIBRARY
  find_library(D3D9_LIBRARY_RELEASE NAMES d3d9 PATHS ${DIRECTX_LIBRARY_DIR})
  find_library(D3D9_LIBRARY_DEBUG NAMES d3d9d PATHS ${DIRECTX_LIBRARY_DIR})
  _DIRECTX_ADJUST_LIB_VARS(D3D9)

  # Set D3DX9_LIBRARY
  find_library(D3DX9_LIBRARY_RELEASE NAMES d3dx9 PATHS ${DIRECTX_LIBRARY_DIR})
  find_library(D3DX9_LIBRARY_DEBUG NAMES d3dx9d PATHS ${DIRECTX_LIBRARY_DIR})
  _DIRECTX_ADJUST_LIB_VARS(D3DX9)

  # Set D3D10_LIBRARY
  find_library(D3D10_LIBRARY_RELEASE NAMES d3d10 PATHS ${DIRECTX_LIBRARY_DIR})
  find_library(D3D10_LIBRARY_DEBUG NAMES d3d10d PATHS ${DIRECTX_LIBRARY_DIR})
  _DIRECTX_ADJUST_LIB_VARS(D3D10)

  # Set D3DX10_LIBRARY
  find_library(D3DX10_LIBRARY_RELEASE NAMES d3dx10 PATHS ${DIRECTX_LIBRARY_DIR})
  find_library(D3DX10_LIBRARY_DEBUG NAMES d3dx10d PATHS ${DIRECTX_LIBRARY_DIR})
  _DIRECTX_ADJUST_LIB_VARS(D3DX10)

  # Set D3D10_1_LIBRARY
  find_library(D3D10_1_LIBRARY_RELEASE NAMES d3d10_1 PATHS ${DIRECTX_LIBRARY_DIR})
  find_library(D3D10_1_LIBRARY_DEBUG NAMES d3d10_1d PATHS ${DIRECTX_LIBRARY_DIR})
  _DIRECTX_ADJUST_LIB_VARS(D3D10_1)

  set(DIRECTX_INCLUDE_DIRS
    ${DIRECTX_INCLUDE_DIR}
  )
  set(DIRECTX9_LIBRARIES
    "${DIRECTX_LIBRARY_DIR}/ddraw.lib"
    "${DIRECTX_LIBRARY_DIR}/dinput8.lib"
    "${DIRECTX_LIBRARY_DIR}/dsetup.lib"
    "${DIRECTX_LIBRARY_DIR}/dsound.lib"
    "${DIRECTX_LIBRARY_DIR}/DxErr.lib"
    "${DIRECTX_LIBRARY_DIR}/DxErr9.lib"
    "${DIRECTX_LIBRARY_DIR}/dxgi.lib"
    "${DIRECTX_LIBRARY_DIR}/dxguid.lib"
    "${DIRECTX_LIBRARY_DIR}/X3DAudio.lib"
    "${DIRECTX_LIBRARY_DIR}/XInput.lib"
    strmiids dmoguids msdmo ole32 oleaut32 uuid gdi32
    ${D3D9_LIBRARY}
    ${D3DX9_LIBRARY}
  )

  set(DIRECTX10_LIBRARIES
    ${D3D10_LIBRARY}
    ${D3DX10_LIBRARY}
  )
  set(DIRECTX10_1_LIBRARIES
    ${D3D10_1_LIBRARY}
  )

  if (DIRECTX_INCLUDE_DIRS AND DIRECTX9_LIBRARIES)
    set(DIRECTX9_FOUND TRUE)
  endif (DIRECTX_INCLUDE_DIRS AND DIRECTX9_LIBRARIES)

  if (DIRECTX_INCLUDE_DIRS AND DIRECTX10_LIBRARIES)
    set(DIRECTX10_FOUND TRUE)
  endif (DIRECTX_INCLUDE_DIRS AND DIRECTX10_LIBRARIES)

  if (DIRECTX_INCLUDE_DIRS AND DIRECTX10_1_LIBRARIES)
    set(DIRECTX10_1_FOUND TRUE)
  endif (DIRECTX_INCLUDE_DIRS AND DIRECTX10_1_LIBRARIES)

  if (DIRECTX9_FOUND)
    if (NOT DirectX_FIND_QUIETLY)
      message(STATUS "Found DirectX 9")
    endif (NOT DirectX_FIND_QUIETLY)
  endif (DIRECTX9_FOUND)

  if (DIRECTX10_FOUND)
    if (NOT DirectX_FIND_QUIETLY)
      message(STATUS "Found DirectX 10")
    endif (NOT DirectX_FIND_QUIETLY)
  endif (DIRECTX10_FOUND)

  if (DIRECTX10_1_FOUND)
    if (NOT DirectX_FIND_QUIETLY)
      message(STATUS "Found DirectX 10.1")
    endif (NOT DirectX_FIND_QUIETLY)
  endif (DIRECTX10_1_FOUND)

  if (NOT DIRECTX9_FOUND AND NOT DIRECTX10_FOUND AND NOT DIRECTX10_1_FOUND)
    if (DirectX_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find DirectX")
    endif (DirectX_FIND_REQUIRED)
  endif (NOT DIRECTX9_FOUND AND NOT DIRECTX10_FOUND AND NOT DIRECTX10_1_FOUND)


  # show the DIRECTX_INCLUDE_DIRS and DIRECTX9_LIBRARIES variables only in the advanced view
  mark_as_advanced(DIRECTX_INCLUDE_DIRS DIRECTX9_LIBRARIES)

  # show the DIRECTX_INCLUDE_DIRS and DIRECTX10_LIBRARIES variables only in the advanced view
  mark_as_advanced(DIRECTX_INCLUDE_DIRS DIRECTX10_LIBRARIES)

  # show the DIRECTX_INCLUDE_DIRS and DIRECTX10_1_LIBRARIES variables only in the advanced view
  mark_as_advanced(DIRECTX_INCLUDE_DIRS DIRECTX10_1_LIBRARIES)

endif (DIRECTX_INCLUDE_DIRS)
