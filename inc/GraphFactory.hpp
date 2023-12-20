#pragma once

#include <optional>
#include <string_view>
#include <traits>
#include <filesystem>

#include <NullGraph.hpp>

namespace graph
{
class GraphFactory
{
public:
    /*template<typename T, std::enable_if<std::is_same<T, Graph<T>>::value, bool> = true>
    Graph<T> createFromFile(std::string_view fileName)
    {
        namespace fs = std::filesystem;

        fs::path extension = fs::path(fileName).extension();

        if (not isSupportedExtension(extension))
        {
            return NullGraph;
        }


    }

private:

    bool parseFile(std::string_view, std::filesystem::path)
    {
    }*/
};
} // namespace graph