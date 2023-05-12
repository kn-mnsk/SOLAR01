## Windows Desktop Application

cmake_minimum_required (VERSION 3.2)
#cmake_policy(VERSION 3.1)
#cmake_gui(--preset="${CMakePresets.json}")
#set(CMAKE_MAKE_PROGRAM ninja)
#set(CMAKE_GENERATOR ninja)

get_filename_component(ProjectID ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectID ${ProjectID})
project (${ProjectID})

set(CMAKE_DEBUG_POSTFIX d)

##### From CmakeLists of freeglut
# for multiarch LIBDIR support (requires cmake>=2.8.8)
INCLUDE(GNUInstallDirs)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "..." FORCE)
endif()

### Folder Structures
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

### Define directories -Configuration types are either Debug or Release only!
set(ConfigDir $<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
set(PROGRAMS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(CURRENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
# NOTE: On Windows and Cygwin, the dll's are placed in the
# CMAKE_RUNTIME_OUTPUT_DIRECTORY, while their corresponding import
# libraries end up in CMAKE_ARCHIVE_OUTPUT_DIRECTORY. On other
# platforms, such as Linux, the shared libraries are put in
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY instead.
# Static libraries end up in CMAKE_ARCHIVE_OUTPUT_DIRECTORY on all
# platforms.
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
##### From CmakeLists of freeglut

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CURRENT_DIR}/cmake/modules/")

include_directories(
    "${PROGRAMS_DIR}/assimp/assimp/include" 
    "${PROGRAMS_DIR}/glad/glad-4.6-compatibility/include" 
    "${PROGRAMS_DIR}/glm/glm-0.9.9.7" 
    "${PROGRAMS_DIR}/stb/stb_image.h_2.26/include" 
    "${PROGRAMS_DIR}/glimg/include" 
    "${PROGRAMS_DIR}/glimg/source" 
    "${PROGRAMS_DIR}/boost/boost_1_79_0" 
    "${PROGRAMS_DIR}/libgltf/include" 
    "${PROGRAMS_DIR}/tinygltf" 
    "${PROGRAMS_DIR}/FreeType/FreeType-2.10.4/FreeType/include" 
    "${CURRENT_DIR}/mylibrary" 
    "${CURRENT_DIR}/source"
    )

add_definitions(-D_CRT_SECURE_NO_WARNINGS)  # to disable deprication

configure_file("${CURRENT_DIR}/configuration/root_directory.h.in" "${CURRENT_DIR}/configuration/local_configuration/root_directory.h")
include_directories("${CURRENT_DIR}/configuration")

## libraries organization
# OpenGL
find_package(OpenGL REQUIRED)
set(LIBS ${OPENGL_LIBRARIES} comctl32.lib)# gdiplus.lib)

file(GLOB_RECURSE GladFile 
"${PROGRAMS_DIR}/glad/glad-4.6-compatibility/include/glad/*.h"
"${PROGRAMS_DIR}/glad/glad-4.6-compatibility/src/*.c" 
    )
add_library(staticGLAD STATIC "${GladFile}")

#Then, other libs
set(LIBS "${LIBS}" 
    staticGLAD
    "${PROGRAMS_DIR}/stb/stb_image.h_2.26/libStatic/.build/lib/${ConfigDir}/staticSTBIMAGE.lib" 
    "${PROGRAMS_DIR}/assimp/assimp/lib/${ConfigDir}/assimp-vc143-mt$<$<CONFIG:Debug>:d>.lib"
    "${PROGRAMS_DIR}/glimg/.build/lib/${ConfigDir}/GLIMG.lib" 
    "${PROGRAMS_DIR}/libgltf/source/libgltf/${ConfigDir}/libgltf$<$<CONFIG:Debug>:d>.lib"
    "${PROGRAMS_DIR}/FreeType/FreeType-2.10.4/FreeType/release dll/win64/freetype.lib" 
    )

### then create a project file
file(GLOB_RECURSE SourceHeader
    "${CURRENT_DIR}/source/source/main/*.h" 
    "${CURRENT_DIR}/source/source/main/*.hpp" 
)  
file(GLOB_RECURSE SourceMain
    "${CURRENT_DIR}/source/source/main/*.cpp" 
)  
file(GLOB_RECURSE SourceResource 
    "${CURRENT_DIR}/source/source/main/*.ico"
    "${CURRENT_DIR}/source/source/main/*.rc"
)
file(GLOB_RECURSE SourceSub
    "${CURRENT_DIR}/source/source/sub/*.*" 
    "${CURRENT_DIR}/mylibrary/mylibrary/SolarSys.*"
)
file(GLOB_RECURSE WinOpenGL
    "${CURRENT_DIR}/source/source/winOpenGL/*.*"  
)
file(GLOB_RECURSE WinControls
    "${CURRENT_DIR}/source/source/winControls/*.*"  
)
file(GLOB_RECURSE WinLogs
    "${CURRENT_DIR}/source/source/winLogs/*.*"  
)

file(GLOB_RECURSE Shader 
    "${CURRENT_DIR}/shader/*.vert"
    "${CURRENT_DIR}/shader/*.frag"
    "${CURRENT_DIR}/shader/*.geom"
    "${CURRENT_DIR}/shader/*.tesc"
    "${CURRENT_DIR}/shader/*.tese"
    "${CURRENT_DIR}/shader/*.glsl" 
)

add_executable(${ProjectID} ${SourceHeader} ${SourceMain} ${SourceResource} ${SourceSub} ${WinOpenGL} ${WinControls} ${WinLogs} ${Shader})
set_target_properties(${ProjectID} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_compile_definitions(${ProjectID} PRIVATE "UNICODE;_UNICODE" )
target_link_options(${ProjectID} PRIVATE "/SUBSYSTEM:WINDOWS" )

source_group(Header FILES ${SourceHeader})
source_group(Main FILES ${SourceMain})
source_group(Resource FILES ${SourceResource})
source_group(MainSub FILES ${SourceSub})
source_group(winControls FILES ${WinControls})
source_group(winOpenGL FILES ${WinOpenGL})
source_group(winLogs FILES ${WinLogs})
source_group(Shader FILES ${Shader})
target_link_libraries(${ProjectID} PUBLIC "${LIBS}" )

unset(SourceHeader)
unset(SourceMain)
unset(SourceResource)
unset(SourceSub)
unset(WinControls)
unset(WinOpenGL)
unset(WinLogs)
unset(Shader)

### copy dlls    
add_custom_command(TARGET ${ProjectID} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy "${PROGRAMS_DIR}/assimp/assimp/bin/${ConfigDir}/assimp-vc143-mt$<$<CONFIG:Debug>:d>.dll" $<TARGET_FILE_DIR:${ProjectID}> 
    COMMAND ${CMAKE_COMMAND} -E copy "${PROGRAMS_DIR}/FreeType/FreeType-2.10.4/FreeType/release dll/win64/freetype.dll" $<TARGET_FILE_DIR:${ProjectID}> 
    )

### if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
if(MSVC)
    configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${ProjectID}.vcxproj.user @ONLY)
    target_compile_options(${ProjectID} PUBLIC "/ZI" )
    target_link_options(${ProjectID} PUBLIC "/INCREMENTAL" )

endif(MSVC)
