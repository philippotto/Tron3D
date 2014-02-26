# - try to find Scriptzeug library
#
# Cache Variables: (probably not for direct use in your scripts)
#  SCRIPTZEUG_INCLUDE_DIR
#  SCRIPTZEUG_LIBRARY
#
# Non-cache variables you might use in your CMakeLists.txt:
#  SCRIPTZEUG_FOUND
#  SCRIPTZEUG_INCLUDE_DIRS
#  SCRIPTZEUG_LIBRARIES


set(SCRIPTZEUG_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/include/scriptzeug/include
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/include/reflectionzeug/
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/include/signalzeug/include
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/include/propertyzeug/include
)

FIND_LIBRARY(SCRIPTZEUG_SCRIPTZEUG_LIBRARY
  NAMES scriptzeug
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/lib
    /usr/local/lib
)

FIND_LIBRARY(SCRIPTZEUG_PROPERTYZEUG_LIBRARY
  NAMES propertyzeug
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/lib
    /usr/local/lib
)

FIND_LIBRARY(SCRIPTZEUG_SIGNALZEUG_LIBRARY
  NAMES signalzeug
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/lib
    /usr/local/lib
)

FIND_LIBRARY(SCRIPTZEUG_REFLECTIONZEUG_LIBRARY
  NAMES reflectionzeug
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/lib
    /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Scriptzeug
	DEFAULT_MSG
	SCRIPTZEUG_SCRIPTZEUG_LIBRARY
  # SCRIPTZEUG_PROPERTYZEUG_LIBRARY
  # SCRIPTZEUG_SIGNALZEUG_LIBRARY
	SCRIPTZEUG_INCLUDE_DIR)

if(SCRIPTZEUG_FOUND)
	set(SCRIPTZEUG_LIBRARIES
    ${SCRIPTZEUG_SCRIPTZEUG_LIBRARY}
    ${SCRIPTZEUG_PROPERTYZEUG_LIBRARY}
    ${SCRIPTZEUG_SIGNALZEUG_LIBRARY}
    ${SCRIPTZEUG_REFLECTIONZEUG_LIBRARY}
  )

  set(SCRIPTZEUG_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/include/scriptzeug/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/include/reflectionzeug/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/include/signalzeug/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/scriptzeug/include/propertyzeug/include
  )

endif()