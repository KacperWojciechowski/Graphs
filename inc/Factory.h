#pragma once

#include <GraphBase.h>

namespace Graph
{
    template<typename T>
    class Factory
    {
        public:    
        [[nodiscard]] virtual auto createFromFile(const std::string& path, Graph::Type graphType) -> T = 0;

        [[nodiscard]] virtual auto createFromOther(const GraphBase& other) -> T = 0;

    };
}