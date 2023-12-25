#pragma once

#include <Graph.hpp>

#include <iostream>
#include <fstream>
#include <regex>

namespace graph
{
class FileParser
{
public:
    template<typename T, typename = std::enable_if<std::is_base_of<Graph<T>, T>::value>>
    static T parseLstFile(std::ifstream& file, graph::GraphType graphType)
    {
        std::vector<Edge> edges = {};
        std::size_t nodesCount = 0;
        std::regex linePattern("[0-9]+");
        while (!file.eof())
        {
            std::string line;
            std::getline(file, line);
            if (line.empty())
            {
                break;
            }
            nodesCount++;
            std::smatch matched;
            std::regex_search(line, matched, linePattern);
            std::size_t origin = std::stoul(std::string(matched[0]));
            for(auto start = matched.suffix().first; std::regex_search(start, line.cend(), matched, linePattern); start = matched.suffix().first)
            {
                int defaultWeight = 1;
                edges.emplace_back(EdgeCoord{origin, std::stoul(std::string(matched[0]))}, defaultWeight);
            }
        }
        return fillGraph<T>(graphType, nodesCount, edges);
    }

    template<typename T, typename = std::enable_if<std::is_base_of<Graph<T>, T>::value>>
    static T parseMatFile(std::ifstream& file, graph::GraphType graphType)
    {
        // TODO: implement
        std::cout << "mat" << std::endl;
        return {graphType};
    }

    template<typename T, typename = std::enable_if<std::is_base_of<Graph<T>, T>::value>>
    static T parseGraphMlFile(std::ifstream& file, graph::GraphType graphType)
    {
        // TODO: implement
        std::cout << "graphml" << std::endl;
        return {graphType};
    }

private:
    template<typename T>
    static T fillGraph(graph::GraphType graphType, std::size_t nodesCount, std::vector<graph::Edge> edges)
    {
        T graph = {graphType};
        graph.addNodeSeries(nodesCount);
        graph.setEdgeSeries(edges);
        return graph;
    }
};
} // namespace graph