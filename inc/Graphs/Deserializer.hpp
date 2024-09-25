#pragma once

#include <fstream>
#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
#include <type_traits>

namespace Graphs
{
class Graph;

template <typename T, typename Guard = std::enable_if_t<std::is_base_of_v<Graph, T>>>
class Deserializer
{
public:
    static T deserializeLstFile(std::ifstream& filePath);
    static T deserializeMatFile(std::ifstream& filePath);
    static T deserializeGraphMlFile(std::ifstream& filePath);
};

template class Deserializer<AdjList>;
template class Deserializer<AdjMatrix>;
} // namespace Graphs
