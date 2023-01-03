#pragma once

#include <Factory.h>
#include <FileParser.h>

namespace Graph
{
    template<typename T>
    class DesignatedFactory : public Factory<T>
    {
        public:
        
        [[nodiscard]] virtual auto createFromFile(const std::string& path, Graph::Type graphType) -> T override;

        [[nodiscard]] virtual auto createFromOther(const Graph::GraphBase& other) -> T override;

        protected:
        FileParser fileParser;
    };
}