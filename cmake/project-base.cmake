# Common base cmake file for eris example scripts.  ${myproj} must be set
# before including this file.  This is typically some shorter version of the
# directory name (which will be used as the full project name).  For example,
# circular-city/ might have ccity for myproj.
#
# It is also assumed that the subproject's object code lives entirely within
# the project's ${myproj} directory (e.g. circular-city/ccity/); any cpp files
# inside the project top-level directory (e.g. circular-city/) will be compiled
# to executables that link aginst liberis and the project's compiled object
# files.
#
# Object headers should then be included via '#include "ccity/whatever.hpp"'.
#
# It is also recommended, though not required, that you use the ${myprog} (e.g.
# "ccity") C++ namespace for subproject object code.
#
# Typically this requires a project CMakeLists.txt consisting of just:
#
# cmake_minimum_required(VERSION 2.8)
# set(myproj blah)
# include(../cmake/project-base.cmake)
#

cmake_minimum_required(VERSION 2.8)

if(myproj)
    message(STATUS "Creating ${myproj} build infrastructure (via ../cmake/project-base.cmake)")
else()
    message(FATAL_ERROR "Variable \${myproj} must be set before including ../cmake/project-base.cmake")
endif()

get_filename_component(myproject "${CMAKE_SOURCE_DIR}" NAME)

project("${myproject}" CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake/Modules)

# No in-source building
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out-of-source build.  Create a build directory and run 'cmake ${CMAKE_SOURCE_DIR} [options]'.")

include_directories(.)

file(GLOB_RECURSE proj_object_src ${myproj}/*.cpp)
file(GLOB proj_executables *.cpp)

# Add eris
# If we're building in "foo", look for ../eris/foo for eris build dir, then
# fall back to ../eris/build if that doesn't work.  ../eris should be a symlink
# to the eris project dir.
get_filename_component(BUILD_BASENAME "${CMAKE_BINARY_DIR}" NAME)

include_directories(../eris/include)

set(ERIS_BUILD_HINTS "../eris/build")
if(NOT BUILD_BASENAME STREQUAL "build")
    set(ERIS_BUILD_HINTS "../eris/${BUILD_BASENAME}" "${ERIS_BUILD_HINTS}")
endif()

find_library(
    ERIS_LIB
    NAMES eris
    HINTS ${ERIS_BUILD_HINTS})

if (ERIS_LIB STREQUAL "ERIS_LIB-NOTFOUND")
    message(FATAL_ERROR "Could not find compiled eris (looked in ../eris/${BUILD_BASENAME} ../eris/build)")
else()
    message(STATUS "Found eris (${ERIS_LIB})")
endif()

add_definitions(-std=c++11 -Wall -Werror)

add_library(${myproj} ${proj_object_src})

foreach(proj_src ${proj_executables})
    get_filename_component(exec "${proj_src}" NAME_WE)
    add_executable("${exec}" "${proj_src}")
    target_link_libraries("${exec}" ${ERIS_LIB})
    target_link_libraries("${exec}" ${myproj})
endforeach()

