set(SOURCES AdjList.cpp
            AdjMatrix.cpp
            Pixel_map.cpp
            Benchmark.cpp
            ColoringAlgorithms.cpp
            Deserializer.cpp)

add_library(Sources ${SOURCES})
target_include_directories(Sources PUBLIC ${PROJECT_SOURCE_DIR}/inc)