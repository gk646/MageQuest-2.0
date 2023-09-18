cmake_minimum_required(VERSION 3.26)
project(MageQuest CXX C)
set(CMAKE_CXX_STANDARD 23)


if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -flto")
endif ()

#-DCMAKE_C_FLAGS_RELEASE:STRING="/O2i /DNDEBUG /fp:fast /EHs-c- /GL"

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(OPENGL_VERSION "4.3" CACHE STRING "OpenGL Version to build raylib with")

add_subdirectory(C:/Users/gk646/Documents/Libaries/raylib-4.5.0 raylib)
add_library(sqlite3 STATIC C:/Users/gk646/Documents/Libaries/SQLiteCpp-3.3.1/sqlite3/sqlite3.c)
include_directories(C:/Users/gk646/Documents/Libaries/nbnet-1.1.3/src)
include_directories(C:/Users/gk646/Documents/Libaries/sdk/public)

file(GLOB_RECURSE PROJECT_SOURCES
        "src/*.cpp"
        "src/*.h"
        "src/*.hpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

include_directories(${PROJECT_NAME} PUBLIC
        C:/Users/gk646/Documents/Libaries/raygui-3.6/src
        C:/Users/gk646/Documents/CodingProjects/C++/cxstructs/src
        C:/Users/gk646/Documents/Libaries/SQLiteCpp-3.3.1/sqlite3
)


#target_link_libraries(MageQuest raylib sqlite3 wsock32 ws2_32 opengl32 gdi32 winmm)
target_link_libraries(${PROJECT_NAME} PUBLIC
        C:/Users/gk646/Documents/Libaries/sdk/redistributable_bin/win64/steam_api64.lib
        raylib
        sqlite3
        ws2_32
)