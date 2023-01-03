#pragma once

#include <GraphBase.h>

namespace Graph
{
    template<class T>
    [[nodiscard]] auto constructFromFile(const std::string& path, Graph::Type graphType) -> T
    {
        static_assert(std::is_base_of<GraphBase, T>::value, "Expected type is not a graph representation");

        return T::constructFromFile(path, graphType);
    }
    
    template<class T>
    [[nodiscard]] auto constructFromOther(const GraphBase& other) -> T
    {
        return T(other);
    }
}