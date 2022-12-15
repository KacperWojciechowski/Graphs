cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-10")
set(CMAKE_C_COMPILER "/usr/bin/gcc-10")

project(graphs CXX)

include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

file(GLOB Sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(Sources src/Algorithms.cpp
			src/Coord.cpp
			src/Generator.cpp
			src/GraphList.cpp
			src/GraphMatrix.cpp
			src/PixelMap.cpp)

add_library(Graph_lib ${Sources})

target_include_directories(Graph_lib PUBLIC inc
									 PUBLIC "third-party/rapidxml/include")


file(GLOB TestSources "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

add_executable(Tests ${TestSources})

target_link_libraries(Tests
	gtest
	gmock
	gtest_main
	Graph_lib
)

set_target_properties(Tests PROPERTIES CXX_STANDARD 20)
target_compile_features(Tests PUBLIC cxx_std_20)
target_include_directories(Tests PUBLIC inc)

target_compile_options(Tests PUBLIC -Wall -Wextra -pedantic -Werror)

add_custom_target(run_tests COMMAND ./Tests DEPENDS Tests)
enable_testing()

add_test(
	NAME allTests
	COMMAND tests
)
