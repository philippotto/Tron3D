# - Try to find FMOD EX
#
#  This module defines the following variables
#
#  FMOD_FOUND - Was FMOD found
#  FMOD_INCLUDE_DIRS - the FMOD include directories
#  FMOD_LIBRARIES - Link to this, by default it includes
#                     all FMOD components
#
#  This module accepts the following variables
#
#  FMOD_DIR - Can be set to FMOD install path or Windows build path
#

# Copyright (c) 2013, Johannes Deselaers <deselaers (dot) johannes (at) gmail (dot) com>
#
# Feel free to share and edit


if(!FMOD_DIR)
  set(FMOD_DIR $ENV{FMOD_DIR})
endif(!FMOD_DIR)


# Find FMOD Ex first 
FIND_PATH(FMOD_INCLUDE_DIR fmod.h
  HINTS
  $ENV{FMOD_DIR}
  $ENV{FMODDIR}
  PATH_SUFFIXES api/inc
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/fmod/api/inc
)

FIND_LIBRARY(FMOD_LIBRARY
  NAMES fmodex64_vc fmodex
  HINTS
  $ENV{FMOD_DIR}
  $ENV{FMODDIR}
  PATH_SUFFIXES api/lib
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/fmod/api/lib
)

FIND_LIBRARY(FMOD_LIBRARY_DEBUG 
  NAMES fmodexL64_vc fmodexL
  HINTS
  $ENV{FMOD_DIR}
  $ENV{FMODDIR}
  PATH_SUFFIXES api/lib
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/fmod/api/lib
)


# Find FMOD Event next 
FIND_PATH(FMOD_EVENT_INCLUDE_DIR fmod_event.h
  HINTS
  $ENV{FMOD_DIR}
  $ENV{FMODDIR}
  PATH_SUFFIXES fmoddesignerapi/api
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/fmod/fmoddesignerapi/api/inc
)

FIND_LIBRARY(FMOD_EVENT_LIBRARY
  NAMES fmod_event64 fmodevent
  HINTS
  $ENV{FMOD_DIR}
  $ENV{FMODDIR}
  PATH_SUFFIXES fmoddesignerapi/lib
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/fmod/fmoddesignerapi/api/lib
)

FIND_LIBRARY(FMOD_EVENT_LIBRARY_DEBUG 
  NAMES fmod_eventL64 fmodeventL
  HINTS
  $ENV{FMOD_DIR}
  $ENV{FMODDIR}
  PATH_SUFFIXES fmoddesignerapi/lib
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/fmod/fmoddesignerapi/api/lib
)

FIND_LIBRARY(FMOD_EVENT_NET_LIBRARY
  NAMES fmod_event_net64 fmodeventnet
  HINTS
  $ENV{FMOD_DIR}
  $ENV{FMODDIR}
  PATH_SUFFIXES fmoddesignerapi/lib
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/fmod/fmoddesignerapi/api/lib
)

FIND_LIBRARY(FMOD_EVENT_NET_LIBRARY_DEBUG 
  NAMES fmod_event_netL64 fmodeventnetL
  HINTS
  $ENV{FMOD_DIR}
  $ENV{FMODDIR}
  PATH_SUFFIXES fmoddesignerapi/lib
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/fmod/fmoddesignerapi/api/lib
)

# handle the QUIETLY and REQUIRED arguments and set FMOD_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FMOD DEFAULT_MSG
    FMOD_LIBRARY
    FMOD_INCLUDE_DIR
    FMOD_EVENT_LIBRARY
    FMOD_EVENT_INCLUDE_DIR
    FMOD_EVENT_NET_LIBRARY
)

IF (FMOD_FOUND)
    MESSAGE(STATUS "FMOD found!")
ELSE (FMOD_FOUND)
    MESSAGE(ERROR "FMOD not (entirely) found!")
ENDIF (FMOD_FOUND)

set(FMOD_INCLUDE_DIRS
    ${FMOD_INCLUDE_DIR}
    ${FMOD_EVENT_INCLUDE_DIR}
)


macro(_FMOD_APPEND_LIBRARIES _list _release)
   set(_debug ${_release}_DEBUG)
   if(${_debug})
      set(${_list} ${${_list}} optimized ${${_release}} debug ${${_debug}})
   else()
      set(${_list} ${${_list}} ${${_release}})
   endif()
endmacro()

if(FMOD_FOUND)
   _FMOD_APPEND_LIBRARIES(FMOD_LIBRARIES FMOD_LIBRARY)
   _FMOD_APPEND_LIBRARIES(FMOD_LIBRARIES FMOD_EVENT_LIBRARY)
   _FMOD_APPEND_LIBRARIES(FMOD_LIBRARIES FMOD_EVENT_NET_LIBRARY)
endif()
