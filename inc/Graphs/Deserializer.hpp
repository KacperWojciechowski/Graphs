#pragma once

#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
#include <string>
#include <type_traits>

namespace Graphs
{
class Graph;

template <typename T, typename Guard = std::enable_if_t<std::is_base_of_v<Graph, T>>>
class Deserializer
{
public:
    static T deserializeLstFile(const std::string& filePath);
    static T deserializeMatFile(const std::string& filePath);
    static T deserializeGraphMlFile(const std::string& filePath);
};

template class Deserializer<AdjList>;
template class Deserializer<AdjMatrix>;
} // namespace Graphs
