cmake_minimum_required (VERSION 2.8.12)
project(maplesdl)
#
# Project options
#
include(CheckCCompilerFlag)
include(CheckFunctionExists)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "Setting G++ Complier options")
  set(CMAKE_CXX_FLAGS_DEBUG "-std=c++14 -Wall -pedantic -g -pg ${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_RELEASE "-std=c++14 -Wall -pedantic ${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	message(STATUS "Setting Clang Complier options)")
	set(CMAKE_CXX_FLAGS_DEBUG "--std=c++14 -Wall -g -pg ${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_RELEASE "--std=c++14 -Wall ${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	message(STATUS "Setting VS Complier options (NOT SET)")
else()
	message(STATUS "Unable to determine compiler")
endif()
#sudo apt install libtinyxml2-dev
#Setting local modules path
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")
#
# Find packages
#
find_package(PkgConfig REQUIRED)
foreach(dependencies zlib sdl2 SDL2_mixer SDL2_image SDL2_ttf box2d)
  pkg_search_module(${dependencies} REQUIRED ${dependencies})
  list(APPEND GAME_LIBRARIES ${${dependencies}_LIBRARIES})
  include_directories(${${dependencies}_INCLUDE_DIRS})
  message(STATUS "Adding include ${${dependencies}_INCLUDE_DIRS}") 
endforeach()
find_package(TinyXML2 REQUIRED)

include_directories(MapleSDL)
aux_source_directory(MapleSDL MAPLE_SRC)
aux_source_directory(MapleSDL/MISC MAPLE_SRC)

add_executable(${PROJECT_NAME}.bin ${MAPLE_SRC})
#add_dependencies(${PROJECT_NAME}.bin
target_link_libraries (
  ${PROJECT_NAME}.bin
  ${GAME_LIBRARIES}
  ${TINYXML2_LIBRARIES}
  )
