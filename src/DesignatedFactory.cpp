#include <DesignatedFactory.h>

#include <type_traits>

template<typename T>
auto DesignatedFactory::createFromFile(std::string path, Graph::Type graphType) -> T
{
    static_assert(std::is_base_of<GraphBase, T>::value, "Given class is not a graph type");

    return T::constructFromFile(path, graphType);

}

template<typename T, typename O>
auto DesignatedFactory::createFromOther(O other) -> T
{

}