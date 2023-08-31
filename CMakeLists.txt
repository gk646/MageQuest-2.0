cmake_minimum_required(VERSION 3.26)
project(DungeonM CXX)
set(CMAKE_CXX_STANDARD 23)

# Adding Raylib
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(OPENGL_VERSION "4.3" CACHE STRING "OpenGL Version to build raylib with")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
# Fetch a specific version of raylib
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 4.5.0)
FetchContent_MakeAvailable(raylib)
FetchContent_Declare(cxstructs GIT_REPOSITORY https://github.com/gk646/CXStructures.git)
FetchContent_MakeAvailable(cxstructs)
include_directories(C:/Users/gk646/Documents/Libaries/raygui-3.6/src)
include_directories(C:/Users/gk646/Documents/Libaries/SFML-2.6.0/include)
file(GLOB_RECURSE PROJECT_SOURCES
        "src/*.cpp"
        "src/*.h"
        "src/*.hpp")

# Adding our source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/sources/")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_directories(${PROJECT_NAME} PRIVATE C:/Users/gk646/Documents/Libaries/raygui-3.6/src)
target_link_directories(${PROJECT_NAME} PRIVATE C:/Users/gk646/Documents/Libaries/SFML-2.6.0/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE cxstructs PRIVATE raylib sfml-system  sfml-network)
# Setting ASSETS_PATH
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH= "${CMAKE_CURRENT_SOURCE_DIR}/res")