cmake_minimum_required(VERSION 3.20)
project(Graphs)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

add_executable(Graphs main.cpp)
target_include_directories(Graphs PUBLIC ${PROJECT_SOURCE_DIR}/inc)

target_link_libraries(Graphs PRIVATE Sources)