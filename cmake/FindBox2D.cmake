# - Try to find the Box2D library
#
#  This module defines the following variables
#
#  BOX2D_FOUND - Was Box2D found
#  BOX2D_INCLUDE_DIRS - the Box2D include directories
#  BOX2D_LIBRARY - Link to this
#
#  This module accepts the following variables
#
#  BOX2D_ROOT - Can be set to Box2D install path or Windows build path
#
#=============================================================================
#  FindBox2D.cmake, adapted from FindLogog.cmake which has the following
#  copyright -
#  FindLogog.cmake, adapted from FindBullet.cmake which has the following
#  copyright -
#-----------------------------------------------------------------------------
# Copyright 2009 Kitware, Inc.
# Copyright 2009 Philip Lowman <philip@yhbt.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

if (NOT DEFINED BOX2D_ROOT)
    set (BOX2D_ROOT /usr /usr/local)
endif (NOT DEFINED BOX2D_ROOT)

if(MSVC)
set(LIB_PATHS ${BOX2D_ROOT} ${BOX2D_ROOT}/Release)
else(MSVC)
set (LIB_PATHS ${BOX2D_ROOT} ${BOX2D_ROOT}/lib)
endif(MSVC)

macro(_FIND_BOX2D_LIBRARY _var)
  find_library(${_var}
     NAMES 
        ${ARGN}
     PATHS
		${LIB_PATHS}
     PATH_SUFFIXES lib
  )
  mark_as_advanced(${_var})
endmacro()

if(MSVC)
    find_path(BOX2D_INCLUDE_DIR NAMES Box2D.h
    	PATH_SUFFIXES src/windows src/windows/Box2D
        PATHS ${BOX2D_ROOT})
else(MSVC)
	# Linux/OS X builds
    find_path(BOX2D_INCLUDE_DIR NAMES Box2D/Box2D.h
    	PATH_SUFFIXES include
        PATHS ${BOX2D_ROOT})
endif(MSVC)

# Find the libraries
if(MSVC)
    _FIND_BOX2D_LIBRARY(BOX2D_LIBRARY                   libBox2D.lib)
else(MSVC)
	# Linux/OS X builds
    _FIND_BOX2D_LIBRARY(BOX2D_LIBRARY                   libBox2D.so)
endif(MSVC)

# handle the QUIETLY and REQUIRED arguments and set BOX2D_FOUND to TRUE if 
# all listed variables are TRUE
include("${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake")
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Box2D DEFAULT_MSG
    BOX2D_LIBRARY)

if(MSVC)
    string(REGEX REPLACE "/Box2D$" "" VAR_WITHOUT ${BOX2D_INCLUDE_DIR})
    string(REGEX REPLACE "/windows$" "" VAR_WITHOUT ${VAR_WITHOUT})
    set(BOX2D_INCLUDE_DIRS ${BOX2D_INCLUDE_DIRS} "${VAR_WITHOUT}")
    string(REGEX REPLACE "/libBox2D.lib" "" BOX2D_LIBRARY_DIR ${LOGOG_LIBRARY})
else(MSVC)
	# Linux/OS X builds
    set(BOX2D_INCLUDE_DIRS ${BOX2D_INCLUDE_DIR})
    string(REGEX REPLACE "/libBox2D.so" "" BOX2D_LIBRARY_DIR ${LOGOG_LIBRARY})
endif(MSVC)
