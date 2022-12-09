cmake_minimum_required(VERSION 3.16)
project(graphs CXX)

include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

file(GLOB Sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_subdirectory(lib)
add_library(Graph_lib ${Sources})

target_include_directories(Graph_lib PUBLIC inc)

file(GLOB TestSources "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

add_executable(Tests ${TestSources})

target_link_libraries(Tests
	INTERFACE RapidXML
	gtest
	gmock
	gtest_main
	Graph_lib
)

set_target_properties(Tests PROPERTIES CXX_STANDARD 20)
target_compile_features(Tests PUBLIC cxx_std_20)
target_include_directories(Tests PUBLIC inc)

target_compile_options(Tests PUBLIC -std=c++2a -Wall -Wextra -pedantic -Werror -std=c++2a)

add_custom_target(run_tests COMMAND ./tests DEPENDS Tests)
enable_testing()

add_test(
	NAME allTests
	COMMAND tests
)
