cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)
project(troen)

# PROJECT DESCRIPTION
set(META_PROJECT_NAME        "Troen")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "1")
set(META_VERSION_PATCH       "0")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_AUTHOR_ORGANIZATION "GameProgramming")
set(META_AUTHOR_DOMAIN       "https://github.com/philippotto/GP2013")


# OPTIONS
option(OPTION_LIMIT_CONFIGS  "Generate limited configs (Release; Debug)" ON)
option(OPTION_LOCAL_INSTALL  "Install to a local directory instead of the system" OFF)
option(OPTION_BUILD_STATIC   "Build static libraries" OFF)
option(OPTION_BUILD_EXAMPLES "Build examples" ON)
option(OPTION_ERRORS_AS_EXCEPTION "Throw exceptions" OFF)


# CMAKE CONFIGURATION
set(TROEN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Include cmake modules from ./cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Find the WinSDK libs
if (WIN32)
    set(CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64")
    cmake_policy(SET CMP0020 NEW)
endif (WIN32)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set configuration types
if(OPTION_LIMIT_CONFIGS)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)
endif()

# Project
project(${META_PROJECT_NAME} C CXX)

# Generate folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include custom cmake functions
include(cmake/Custom.cmake)


# PLATFORM AND ARCHITECTURE

# Architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(X64 ON)
endif()

# Check for linux
if(UNIX AND NOT APPLE)
    set(LINUX 1)
endif()

# Setup platform specifics (compile flags, etc., ...)
if(MSVC)
    message(STATUS "Configuring for platform Windows/MSVC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Windows/GCC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(LINUX AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Linux/GCC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinuxGCC.cmake)
elseif(APPLE)
    message(STATUS "Configuring for platform MacOS.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
else()
    # Unsupported system/compiler
    message(WARNING "Unsupported platform/compiler combination")
endif()


# INSTALL

# if(X64)
#     # http://public.kitware.com/Bug/view.php?id=9094
#     set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
# endif()

# set(project ${META_PROJECT_NAME})
# if(WIN32)
#     set(INSTALL_BIN           ".")                      # C:\Programme\<project>
#     set(INSTALL_SHARED        ".")                      # C:\Programme\<project>
#     set(INSTALL_LIB           "lib")                    # C:\Programme\<project>\lib
#     set(INSTALL_INCLUDE       "include")                # C:\Programme\<project>\include
#     set(INSTALL_DOC           "doc")                    # C:\Programme\<project>\doc
#     set(INSTALL_APPLICATIONS  "")                       # Not available under Windows
#     set(INSTALL_ICONS         "")                       # Not available under Windows
#     set(INSTALL_INIT          "")                       # Not available under Windows
#     set(INSTALL_DATA          ".")                      # C:\Programme\<project>
#     set(INSTALL_EXAMPLES      ".")                      # C:\Programme\<project>
# else()
#     set(INSTALL_BIN           "bin")                    # /usr/[local]/bin
#     set(INSTALL_SHARED        "lib")                    # /usr/[local]/lib
#     set(INSTALL_LIB           "lib")                    # /usr/[local]/lib
#     set(INSTALL_INCLUDE       "include")                # /usr/[local]/include
#     set(INSTALL_DOC           "share/doc/${project}")   # /usr/[local]/share/doc/<project>
#     set(INSTALL_APPLICATIONS  "share/applications")     # /usr/[local]/share/applications
#     set(INSTALL_ICONS         "share/pixmaps")          # /usr/[local]/share/pixmaps
#     set(INSTALL_INIT          "/etc/init")              # /etc/init (upstart init scripts)
#     set(INSTALL_DATA          "share/${project}")       # /usr/[local]/share/<project>
#     set(INSTALL_EXAMPLES      "share/${project}")       # /usr/[local]/share/<project>

#     # Adjust target paths for Local installs
#     if(OPTION_LOCAL_INSTALL)
#         # Put binaries in root directory and keep data directory name
#         set(INSTALL_BIN                       ".")           # /<INSTALL_PREFIX>
#         set(INSTALL_DATA                      ".")           # /<INSTALL_PREFIX>

#         # We have to change the RPATH of binaries to achieve a usable local install.
#         # [TODO] For binaries, "$ORIGIN/lib" is right, so that libraries are found in ./lib.
#         #        However, I have not yet tested what happens when libraries use other libraries.
#         #        In that case, they might need the rpath $ORIGIN instead ...
#         set(CMAKE_SKIP_BUILD_RPATH            FALSE)         # Use automatic rpath for build
#         set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE)         # Use specific rpath for INSTALL
#         set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)         # NO automatic rpath for INSTALL
#         set(CMAKE_INSTALL_RPATH               "$ORIGIN/lib") # Libraries are relative to binary
#     endif()
# endif()

# install(DIRECTORY ${GLOW_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE})


# DEPENDENCIES

# Qt5 - taken from cg2sandbox
set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
# Probably works in the next cmake release -> http://www.cmake.org/Bug/view.php?id=13788
# What we do not want is automocs beside the project -> http://www.cmake.org/Bug/view.php?id=13688
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)
# try to find either qt4 or qt5, favoring qt5
# good resource: http://www.kdab.com/using-cmake-with-qt-5/
# http://qt-project.org/forums/viewthread/30006/
if (MSVC)
    cmake_policy(SET CMP0020 NEW)
endif()

set(QT_PACKAGES
    Qt5Core
    Qt5Gui
    Qt5Widgets
    Qt5OpenGL
)
foreach(package ${QT_PACKAGES})
    find_package(${package} 5.1 REQUIRED)
endforeach(package)

# OpenGL
find_package(OpenGL REQUIRED)

# OpenSceneGraph
set(OSG_PACKAGES
    OpenThreads
    osg
    osgAnimation
    osgDB
    osgFX
    osgGA
    osgParticle
    osgShadow
    osgText
    osgUtil
    osgViewer
    osgQt
)
foreach(package ${OSG_PACKAGES})
    find_package(${package} REQUIRED)
endforeach(package)

set(OSG_INCLUDE_DIRECTORIES
    ${OPENTHREADS_INCLUDE_DIR}
    ${OSG_INCLUDE_DIR}
    ${OSGANIMATION_INCLUDE_DIR}
    ${OSGDB_INCLUDE_DIR}
    ${OSGFX_INCLUDE_DIR}
    ${OSGGA_INCLUDE_DIR}
    ${OSGPARTICLE_INCLUDE_DIR}
    ${OSGSHADOW_INCLUDE_DIR}
    ${OSGTEXT_INCLUDE_DIR}
    ${OSGUTIL_INCLUDE_DIR}
    ${OSGVIEWER_INCLUDE_DIR}
    $(OSGQT_INCLUDE_DIR)
)

set(OSG_LIBRARY
    ${OPENTHREADS_LIBRARY}
    ${OSG_LIBRARY}
    ${OSGANIMATION_LIBRARY}
    ${OSGDB_LIBRARY}
    ${OSGFX_LIBRARY}
    ${OSGGA_LIBRARY}
    ${OSGPARTICLE_LIBRARY}
    ${OSGSHADOW_LIBRARY}
    ${OSGTEXT_LIBRARY}
    ${OSGUTIL_LIBRARY}
    ${OSGVIEWER_LIBRARY}
    $(OSGQT_LIBRARY)
)

### TODO: Bullet, FMOD here!

set(BULLET_LIBARY_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/BulletCollision_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/BulletDynamics_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/BulletFileLoader_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/BulletMultiThreaded_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/BulletSoftBody_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/BulletSoftBodySolvers_OpenCL_Mini_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/BulletWorldImporter_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/BulletXmlWorldImporter_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/ConvexDecomposition_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/GIMPACTUtils_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/LinearMath_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/MiniCL_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Debug/OpenGLSupport_Debug.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/BulletCollision.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/BulletDynamics.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/BulletFileLoader.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/BulletMultiThreaded.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/BulletSoftBody.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/BulletSoftBodySolvers_OpenCL_Mini.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/BulletWorldImporter.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/BulletXmlWorldImporter.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/ConvexDecomposition.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/GIMPACTUtils.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/LinearMath.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/MiniCL.lib
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/lib/Release/OpenGLSupport.lib
	)
set(BULLET_INCLUDE_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}/bullet/include
	)


set(headers
    source/mainwindow.h
)	

# SOURCES
set(sources
    source/main.cpp
    source/mainwindow.cpp

)

#possibly: add subdirectories here
add_subdirectory(source)


# TARGET
# Set target name
set(target troen)

add_executable(${target} ${headers} ${sources})

qt5_use_modules(${target} Core OpenGL Gui Widgets)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${OSG_INCLUDE_DIRECTORIES}
	${BULLET_INCLUDE_DIRECTORIES}
    # ${FMOD_INCLUDE_DIR}
)

target_link_libraries(${target}
    ${OPENGL_LIBRARIES}
    ${OSG_LIBRARY}
    # ${FMOD_LIBRARIES}
	${BULLET_LIBARY_DIRECTORIES}
)