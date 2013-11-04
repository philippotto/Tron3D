# - Try to find OpenSceneGraph
#
#  This module defines the following variables
#
#  OPENSCENEGRAPH_FOUND - Was OpenSceneGraph found
#  OPENSCENEGRAPH_INCLUDE_DIRS - the OpenSceneGraph include directories
#  OPENSCENEGRAPH_LIBRARIES - Link to this, by default it includes
#                     all OpenSceneGraph components
#
#  This module accepts the following variables
#
#  OSG_DIR - Can be set to OpenSceneGraph install path or Windows build path
#

# Copyright (c) 2013, Johannes Deselaers <deselaers (dot) johannes (at) gmail (dot) com>
#
# Feel free to share and edit

if(!OSG_DIR)
  set(OSG_DIR $ENV{OSG_DIR})
endif(!OSG_DIR)

macro(_FIND_OSG_LIBRARY _var)
  find_library(${_var}
     NAMES
        ${ARGN}
     PATHS
        ${OSG_DIR}
        ${OSG_DIR}/libs
     PATH_SUFFIXES lib
  )
  # mark_as_advanced(${_var})
endmacro()

macro(_OSG_APPEND_LIBRARIES _list _release)
   set(_debug ${_release}_DEBUG)
   if(${_debug})
      set(${_list} ${${_list}} optimized ${${_release}} debug ${${_debug}})
   else()
      set(${_list} ${${_list}} ${${_release}})
   endif()
endmacro()

# Find the include directories

FIND_PATH(OPENSCENEGRAPH_INCLUDE_DIR osg/PositionAttitudeTransform
  HINTS
  $ENV{OSG_DIR}
  $ENV{OSGDIR}
  PATH_SUFFIXES include
  PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
)

# Find the libraries

_FIND_OSG_LIBRARY(OPENTHREADS_LIBRARY           OpenThreads)
_FIND_OSG_LIBRARY(OPENTHREADS_LIBRARY_DEBUG     OpenThreadsd)

_FIND_OSG_LIBRARY(OSG_LIBRARY                   osg)
_FIND_OSG_LIBRARY(OSG_LIBRARY_DEBUG             osgd)

_FIND_OSG_LIBRARY(OSGANIMATION_LIBRARY          osgAnimation)
_FIND_OSG_LIBRARY(OSGANIMATION_LIBRARY_DEBUG    osgAnimationd)

_FIND_OSG_LIBRARY(OSGDB_LIBRARY                 osgDB)
_FIND_OSG_LIBRARY(OSGDB_LIBRARY_DEBUG           osgDBd)

_FIND_OSG_LIBRARY(OSGFX_LIBRARY                 osgFX)
_FIND_OSG_LIBRARY(OSGFX_LIBRARY_DEBUG           osgFXd)

_FIND_OSG_LIBRARY(OSGGA_LIBRARY                 osgGA)
_FIND_OSG_LIBRARY(OSGGA_LIBRARY_DEBUG           osgGAd)

_FIND_OSG_LIBRARY(OSGPARTICLE_LIBRARY           osgParticle)
_FIND_OSG_LIBRARY(OSGPARTICLE_LIBRARY_DEBUG     osgParticled)

_FIND_OSG_LIBRARY(OSGSHADOW_LIBRARY             osgShadow)
_FIND_OSG_LIBRARY(OSGSHADOW_LIBRARY_DEBUG       osgShadowd)

_FIND_OSG_LIBRARY(OSGTEXT_LIBRARY               osgText)
_FIND_OSG_LIBRARY(OSGTEXT_LIBRARY_DEBUG         osgTextd)

_FIND_OSG_LIBRARY(OSGUTIL_LIBRARY               osgUtil)
_FIND_OSG_LIBRARY(OSGUTIL_LIBRARY_DEBUG         osgUtild)

_FIND_OSG_LIBRARY(OSGVIEWER_LIBRARY             osgViewer)
_FIND_OSG_LIBRARY(OSGVIEWER_LIBRARY_DEBUG       osgViewerd)

_FIND_OSG_LIBRARY(OSGQT_LIBRARY                 osgQt)
_FIND_OSG_LIBRARY(OSGQT_LIBRARY_DEBUG           osgQtd)

# handle the QUIETLY and REQUIRED arguments and set OPENSCENEGRAPH_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenSceneGraph DEFAULT_MSG
    OPENTHREADS_LIBRARY
    OSG_LIBRARY
    OSGANIMATION_LIBRARY
    OSGDB_LIBRARY
    OSGFX_LIBRARY
    OSGGA_LIBRARY
    OSGPARTICLE_LIBRARY
    OSGSHADOW_LIBRARY
    OSGTEXT_LIBRARY
    OSGUTIL_LIBRARY
    OSGVIEWER_LIBRARY
    OSGQT_LIBRARY
    OPENSCENEGRAPH_INCLUDE_DIR
)

set(OPENSCENEGRAPH_INCLUDE_DIRS ${OPENSCENEGRAPH_INCLUDE_DIR})

if(OPENSCENEGRAPH_FOUND)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OPENTHREADS_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSG_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGANIMATION_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGDB_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGFX_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGGA_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGPARTICLE_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGSHADOW_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGTEXT_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGUTIL_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGVIEWER_LIBRARY)
   _OSG_APPEND_LIBRARIES(OPENSCENEGRAPH_LIBRARIES OSGQT_LIBRARY)
endif()