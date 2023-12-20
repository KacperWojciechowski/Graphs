#pragma once

#include <Graph.hpp>

#include <iostream>
#include <fstream>

namespace graph
{
class FileParser
{
public:
    template<typename T, typename = std::enable_if<std::is_base_of<Graph<T>, T>::value>>
    static T parseLstFile(std::ifstream& file)
    {
        // TODO: implement
        std::cout << "lst" << std::endl;
        return {};
    }

    template<typename T, typename = std::enable_if<std::is_base_of<Graph<T>, T>::value>>
    static T parseMatFile(std::ifstream& file)
    {
        // TODO: implement
        std::cout << "mat" << std::endl;
        return {};
    }

    template<typename T, typename = std::enable_if<std::is_base_of<Graph<T>, T>::value>>
    static T parseGraphMlFile(std::ifstream& file)
    {
        // TODO: implement
        std::cout << "graphml" << std::endl;
        return {};
    }
};
} // namespace graph