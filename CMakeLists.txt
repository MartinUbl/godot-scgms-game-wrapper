CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(godot-scgms-game-wrapper)

SET(CMAKE_CXX_STANDARD 20)
ADD_COMPILE_OPTIONS($<$<CXX_COMPILER_ID:MSVC>:/MP>)

SET(SMARTCGMS_COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scgms-common/" CACHE PATH "SmartCGMS 'common' directory location")

INCLUDE("${SMARTCGMS_COMMON_DIR}/cmake/Functions.cmake")

DISCOVER_DEPENDENCIES()

INCLUDE_DIRECTORIES("${SMARTCGMS_COMMON_DIR}/")

ADD_SUBDIRECTORY(scgms-common)
ADD_SUBDIRECTORY(godot-cpp)

FILE(GLOB_RECURSE src src/*.cpp src/*.h)
FILE(GLOB wrapper_src scgms-wrappers/game-wrapper/src/*.cpp scgms-wrappers/game-wrapper/src/*.h)

SOURCE_GROUP(src FILES ${src})
SOURCE_GROUP(wrapper_src FILES ${wrapper_src})

ADD_LIBRARY(godot-scgms-game-wrapper SHARED ${src} ${wrapper_src})

TARGET_LINK_LIBRARIES(godot-scgms-game-wrapper godot-cpp scgms-common)