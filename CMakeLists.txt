cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")

project(graphs CXX)

include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

set(Sources src/AdjList.cpp)
            #src/Graph_matrix.cpp
            #src/Pixel_map.cpp)

add_library(Graphs ${Sources})
target_include_directories(Graphs PUBLIC inc)

add_executable(Test src/main.cpp)
target_link_libraries(Test PUBLIC Graphs)
