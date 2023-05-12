# Subdirectory STBIMAGE libDynamic

# This library is compiled as a dynamic library

### Define directories -Configuration types are either Debug or Release only!
set(ConfigDir $<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
set(COMMONSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../")
set(CURRENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
# for multiarch LIBDIR support (requires cmake>=2.8.8)
INCLUDE(GNUInstallDirs)

# NOTE: On Windows and Cygwin, the dll's are placed in the
# CMAKE_RUNTIME_OUTPUT_DIRECTORY, while their corresponding import
# libraries end up in CMAKE_ARCHIVE_OUTPUT_DIRECTORY. On other
# platforms, such as Linux, the shared libraries are put in
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY instead.
# Static libraries end up in CMAKE_ARCHIVE_OUTPUT_DIRECTORY on all
# platforms.
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CURRENT_DIR}/.build/bin")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CURRENT_DIR}/.build/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CURRENT_DIR}/.build/lib")


if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CURRENT_DIR}" CACHE PATH "..." FORCE)
endif()


IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CURRENT_DIR}/.build/cmake/modules/")

include_directories(
    "${CURRENT_DIR}/../include" )

add_definitions(-D_CRT_SECURE_NO_WARNINGS)  # to disable deprication

## libraries organization
# OpenGL
#find_package(OpenGL REQUIRED)

#List all library sources
file(GLOB_RECURSE stbimageFile 
  "${CURRENT_DIR}/../include/stb_image/*.h"
  "${CURRENT_DIR}/../src/*.cpp" )

add_library(dynamicSTBIMAGE SHARED "${stbimageFile}")
set_target_properties(dynamicSTBIMAGE 
  PROPERTIES C_VISIBILITY_PRESET hidden 
  CXX_VISIBILITY_PRESET hidden  
  DEFINE_SYMBOL "${dynamicSTBIMAGE}_EXPORTS" 
  WINDOWS_EXPORT_ALL_SYMBOLS TRUE )
#target_compile_definitions(OGLDEV PUBLIC COMPILING_DLL=1)
#Export the library's public include path to dependent targets
#target_include_directories(OGLDEV  INTERFACE  "${COMMONSDK_DIR}/.Tutorial/tutorial/OGLDEV"   "${CURRENT_DIR}/.local_sdk/OGLDEV/OGLDEV_local" )
unset(stbimageFile)

#target_link_libraries(dynamicSTBIMAGE    PUBLIC   ${OPENGL_LIBRARIES} )

