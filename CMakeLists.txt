
project(circular-city)

cmake_minimum_required(VERSION 2.8)

# ccity package version
set(CCITY_VERSION_MAJOR "0")
set(CCITY_VERSION_MINOR "0")
set(CCITY_VERSION_PATCH "1")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

# No in-source building
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out-of-source build.  Create a build directory and run 'cmake ${CMAKE_SOURCE_DIR} [options]'.")

include_directories(include)
file(GLOB_RECURSE ccity_include include/*.h include/*.hpp)
file(GLOB_RECURSE ccity_src src/ccity/*.cpp)
file(GLOB ccity_scripts src/*.cpp)
#file(GLOB ccity_R src/*.R)

#file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/results)

# Add eris
# If we're building in "foo", look for ../eris/foo for eris build dir, then
# fall back to ../eris/build if that doesn't work.
get_filename_component(BUILD_BASENAME "${CMAKE_BINARY_DIR}" NAME)
set(ERIS_INCLUDE eris/include)
include_directories(${ERIS_INCLUDE})
find_library(
    ERIS_LIB
    NAMES eris
    HINTS "eris/${BUILD_BASENAME}" "eris/build")

add_definitions(-std=c++11 -Wall -Werror)

add_library(ccity ${ccity_src})

foreach(cc_src ${ccity_scripts})
    get_filename_component(v "${cc_src}" NAME_WE)
    add_executable("${cc}" "${cc_src}")
    target_link_libraries("${cc}" ${ERIS_LIB})
    target_link_libraries("${cc}" ccity)
endforeach()

#foreach(cc_r ${ccity_R})
#    file(COPY "${cc_r}" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#endforeach()

#add_subdirectory(doc EXCLUDE_FROM_ALL)
