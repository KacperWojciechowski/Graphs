#include <filesystem>
#include <fstream>
#include <Graphs/Deserializer.hpp>
#include <iostream>
#include <regex>
#include <vector>

namespace
{
enum class FileType
{
    LST,
    MAT,
    GRAPHML
};

template <typename T, FileType ft>
T parseContentIntoGraphRepresentation(const std::string& content)
{
    auto parseLine = [](const std::string& line) {
        std::vector<uint32_t> neighbors;
        std::stringstream stream(line);
        std::string value;

        while (std::getline(stream, value, ' '))
        {
            if (value.empty())
            {
                continue;
            }
            std::cout << value << std::endl;
            neighbors.emplace_back(static_cast<uint32_t>(std::stoi(value) - 1));
        }
        return neighbors;
    };

    using regItr = std::sregex_iterator;
    std::regex nodeRegex("[0-9]:([0-9 ]+)");
    std::vector<std::vector<uint32_t>> nodes;

    for (auto itr = regItr(content.begin(), content.end(), nodeRegex)++; itr != regItr(); ++itr)
    {
        if (itr->size() != 2)
        {
            std::cerr << "[Deserializer] Invalid line format";
            continue;
        }
        std::ssub_match match = (*itr)[1];
        std::cout << match.str() << "\n";
        nodes.emplace_back(parseLine(match.str()));
    }

    T graph;
    graph.addNodes(nodes.size());
    for (uint32_t nodeId = 0; nodeId < nodes.size(); nodeId++)
    {
        for (auto neighbor : nodes[nodeId])
        {
            graph.setEdge({nodeId, neighbor});
        }
    }
    return graph;
}
} // namespace

namespace Graphs
{
template <typename T, typename Guard>
T Deserializer<T, Guard>::deserializeLstFile(const std::string& filePath)
{
    std::filesystem::path path(filePath);
    if (path.extension().string() != ".lst")
    {
        std::cerr << "[Deserializer] Invalid file extension";
        return T();
    }

    std::ifstream file(filePath);

    if (not file.good())
    {
        std::cerr << "[Deserializer] Error opening file";
    }

    auto fileContent = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    return parseContentIntoGraphRepresentation<T, FileType::LST>(fileContent);

    return T();
}
} // namespace Graphs
