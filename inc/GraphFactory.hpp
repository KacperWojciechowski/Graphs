#pragma once

#include <optional>
#include <string_view>
#include <type_traits>
#include <filesystem>
#include <fstream>
#include <functional>

#include <FileParser.hpp>

namespace graph
{
class GraphFactory final
{
public:
    template<typename T, typename = std::enable_if_t<std::is_base_of<Graph<T>, T>::value>>
    std::optional<Graph<T>> createFromFile(std::string_view fileName)
    {
        namespace fs = std::filesystem;

        fs::path extension = fs::path(fileName).extension();

        auto parsingHandler = selectFileHandler<T>(extension);
        if (not parsingHandler)
        {
            return std::nullopt;
        }

        std::ifstream file(fileName.data());
        if (not file.good())
        {
            return std::nullopt;
        }

        return parsingHandler(file);
    }

private:

    template<typename T>
    std::function<T(std::ifstream&)> selectFileHandler(std::filesystem::path extension)
    {
        if (extension == ".lst") {
            return FileParser::parseLstFile<T>;
        } else if (extension == ".mat") {
            return FileParser::parseMatFile<T>;
        } else if (extension == ".graphml" or extension == ".GRAPHML") {
            return FileParser::parseGraphMlFile<T>;
        } else {
            return nullptr;
        }
    }
};
} // namespace graph