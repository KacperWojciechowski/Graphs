#include <algorithm>
#include <fstream>
#include <Graphs/AdjList.hpp>
#include <Graphs/ColoringAlgorithms.hpp>
#include <iostream>
#include <ranges>

int main() {
    auto result = std::make_shared<Graphs::Algorithm::ColoringResult>();
    auto algorithm = Graphs::Algorithm::GreedyColoring<false>{result};

    auto graph = Graphs::AdjList("../BenchmarkSamples/chrom_num_3/1.lst");

    algorithm(graph);

    auto filter = [](const auto& coloring) {
        return coloring.second;
    };
    auto r = std::ranges::max(*result, {}, filter);
    std::cout << r.second << std::endl;
    return 0;
}
