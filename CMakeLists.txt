cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(troen)

#################################################
# PROJECT DESCRIPTION
#################################################

set(META_PROJECT_NAME        "Troen")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "1")
set(META_VERSION_PATCH       "0")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_AUTHOR_ORGANIZATION "GameProgramming")
set(META_AUTHOR_DOMAIN       "https://github.com/philippotto/GP2013")

#################################################
# OPTIONS
#################################################

option(OPTION_LIMIT_CONFIGS  "Generate limited configs (Release; Debug)" ON)
option(OPTION_LOCAL_INSTALL  "Install to a local directory instead of the system" OFF)
option(OPTION_ERRORS_AS_EXCEPTION "Throw exceptions" OFF)

#################################################
# CMAKE CONFIGURATION
#################################################

set(TROEN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Include cmake modules from ./cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Find the WinSDK libs
if (WIN32)
	set(CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64" "C:\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\winv6.3\\um\\x64")
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

#################################################
# PLATFORM AND ARCHITECTURE
#################################################

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


#################################################
# INSTALL
#################################################


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


#################################################
# DEPENDENCIES
#################################################

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
find_package(OpenSceneGraph REQUIRED)

# Bullet
set(BULLET_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/BULLET_PHYSICS")
find_package(BULLET REQUIRED)

# FMOD
find_package(FMOD REQUIRED)

# HIDAPI
find_package(HIDAPI REQUIRED)

#################################################
# SOURCES
#################################################

#possibly: add subdirectories here
#add_subdirectory(source)

set(sources
	source/main.cpp
	source/forwarddeclarations.h
	source/mainwindow.h
	source/mainwindow.cpp
	source/troengame.h
	source/troengame.cpp
	source/sampleosgviewer.h
	source/sampleosgviewer.cpp
	source/gameeventhandler.h
	source/globals.h
	source/constants.h
	source/constants.cpp
	source/gamelogic.h
	source/gamelogic.cpp

	source/resourcepool.h
	source/resourcepool.cpp

	source/input/keyboard.h
	source/input/keyboard.cpp
	source/input/gamepad.h
	source/input/gamepad.cpp
	source/input/gamepadps4.h
	source/input/gamepadps4.cpp
	source/input/bikeinputstate.h
	source/input/bikeinputstate.cpp
	source/input/ai.h
	source/input/ai.cpp
	source/input/pollingdevice.h
	source/input/pollingdevice.cpp

	source/sound/audiomanager.h
	source/sound/audiomanager.cpp

	source/model/bikemodel.h
	source/model/bikemodel.cpp
	source/model/auto_levelmodel.cpp
	source/model/levelmodel.h
	source/model/levelmodel.cpp
	source/model/fencemodel.h
	source/model/fencemodel.cpp
	source/model/abstractmodel.h
	source/model/abstractmodel.cpp
	source/model/itemmodel.h
	source/model/itemmodel.cpp
	source/model/physicsworld.h
	source/model/physicsworld.cpp
	source/model/bikemotionstate.h
	source/model/objectinfo.h

	source/controller/bikecontroller.h
	source/controller/bikecontroller.cpp
	source/controller/levelcontroller.h
	source/controller/levelcontroller.cpp
	source/controller/fencecontroller.h
	source/controller/fencecontroller.cpp
	source/controller/abstractcontroller.h
	source/controller/abstractcontroller.cpp
	source/controller/hudcontroller.h
	source/controller/hudcontroller.cpp
	source/controller/itemcontroller.h
	source/controller/itemcontroller.cpp


	source/view/bikeview.h
	source/view/bikeview.cpp
	source/view/levelview.h
	source/view/levelview.cpp
	source/view/fenceview.h
	source/view/fenceview.cpp
	source/view/abstractview.h
	source/view/abstractview.cpp
	source/view/shaders.h
	source/view/shaders.cpp
	source/view/hudview.h
	source/view/hudview.cpp
	source/view/itemview.h
	source/view/itemview.cpp
	source/view/skydome.h
	source/view/skydome.cpp
	source/view/postprocessing.cpp
	source/view/postprocessing.h
	source/view/nearfarcallback.h
	source/view/timeupdate.h
	source/view/nodefollowcameramanipulator.h
	source/view/nodefollowcameramanipulator.cpp
	source/view/playermarker.h
	source/view/playermarker.cpp
	source/view/easemotion.h
	source/view/reflection.h
	source/view/reflection.cpp

	source/util/chronotimer.h
	source/util/chronotimer.cpp

	source/util/gldebugdrawer.h
	source/util/gldebugdrawer.cpp


)

source_group_by_path("${CMAKE_CURRENT_SOURCE_DIR}/source"
	"\\\\.h$|\\\\.hpp$|\\\\.cpp$|\\\\.c$|\\\\.ui$|\\\\.qrc$" "Source Files" ${sources})

#################################################
# TARGET
#################################################

# Set target name
set(target troen)

add_executable(${target} ${sources})

qt5_use_modules(${target} Core OpenGL Gui Widgets)

include_directories(
	${CMAKE_SOURCE_DIR}
	${OPENSCENEGRAPH_INCLUDE_DIRS}
	${BULLET_INCLUDE_DIRS}
	${FMOD_INCLUDE_DIRS}
	${HIDAPI_INCLUDE_DIRS}
)

target_link_libraries( ${target}
    ${OPENGL_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${FMOD_LIBRARIES}
	${HIDAPI_LIBRARIES}
)

if (WIN32)
target_link_libraries( ${target}
	${CMAKE_PREFIX_PATH}/Xinput9_1_0.lib
)
endif (WIN32)
