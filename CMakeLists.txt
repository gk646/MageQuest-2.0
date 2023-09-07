cmake_minimum_required(VERSION 3.26)
project(MageQuest CXX C)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mtune=native -fno-exceptions -flto")


set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(OPENGL_VERSION "4.3" CACHE STRING "OpenGL Version to build raylib with")

add_subdirectory(C:/Users/gk646/Documents/Libaries/raylib-4.5.0 raylib)
add_library(sqlite3 STATIC C:/Users/gk646/Documents/Libaries/SQLiteCpp-3.3.1/sqlite3/sqlite3.c)
file(GLOB_RECURSE PROJECT_SOURCES
        "src/*.cpp"
        "src/*.h"
        "src/*.hpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

include_directories(${PROJECT_NAME} PUBLIC
        C:/Users/gk646/Documents/Libaries/raygui-3.6/src
        C:/Users/gk646/Documents/CodingProjects/C++/cxstructs/src
        C:/Users/gk646/Documents/Libaries/SQLiteCpp-3.3.1/sqlite3
        #C:/Users/gk646/Documents/Libaries/SFML-2.6.0/include
)
target_link_directories(${PROJECT_NAME} PUBLIC
       # C:/Users/gk646/Documents/Libaries/SFML-2.6.0/lib
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        raylib
        sqlite3
        #pthread
        #sfml-system
        #sfml-network
        #winmm
)