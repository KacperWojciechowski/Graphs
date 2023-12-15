cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")
set(CMAKE_C_COMPILER "/usr/bin/gcc-11")

project(graphs CXX)

include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

set(Sources src/Graph_list.cpp
            src/Graph_matrix.cpp
            src/Pixel_map.cpp)

add_library(Graph_lib ${Sources})

target_include_directories(Graph_lib PUBLIC inc
									 PUBLIC "third-party/rapidxml/include")
