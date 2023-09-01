cmake_minimum_required(VERSION 3.26)
project(DungeonM CXX)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(OPENGL_VERSION "4.3" CACHE STRING "OpenGL Version to build raylib with")

add_subdirectory(C:/Users/gk646/Documents/Libaries/raylib-4.5.0 ${PROJECT_SOURCE_DIR}/raylib)

file(GLOB_RECURSE PROJECT_SOURCES
        "src/*.cpp"
        "src/*.h"
        "src/*.hpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

include_directories(${PROJECT_NAME} PUBLIC
        C:/Users/gk646/Documents/Libaries/raygui-3.6/src
        C:/Users/gk646/Documents/Libaries/raylib-4.5.0/src
        C:/Users/gk646/Documents/Libaries/SFML-2.6.0/include
        C:/Users/gk646/Documents/CodingProjects/C++/cxstructs/src
)
target_link_directories(${PROJECT_NAME} PRIVATE
        C:/Users/gk646/Documents/Libaries/raylib-4.5.0/src
        C:/Users/gk646/Documents/Libaries/raygui-3.6/src
        C:/Users/gk646/Documents/Libaries/SFML-2.6.0/lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
        sfml-system
        sfml-network
        winmm  # if needed for Windows Multimedia API
)