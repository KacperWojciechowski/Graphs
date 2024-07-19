set(SOURCES Graph_list.cpp
            Graph_matrix.cpp
            Pixel_map.cpp
            Benchmark.cpp)

add_library(Sources ${SOURCES})
target_include_directories(Sources PUBLIC ${PROJECT_SOURCE_DIR}/inc)