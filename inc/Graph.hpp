#pragma once

#include <vector>

namespace graph
{
enum class GraphType
{
    undirected = 0,
    directed
};

struct EdgeCoord
{
    std::size_t source;
    std::size_t target;
};

struct DirectedDegree
{
    std::size_t inDeg;
    std::size_t outDeg;
}

using UndirectedDegree = std::size_t;
using Degree = std::variant<UndirectedDegree, DirectedDegree>;

template<typename T>
friend std::ostream& operator<<(std::ostream& out, const Graph<T>& graph) const noexcept
{
    out << static_cast<T&>(graph);
    return out;
}

template<typename T>
class Graph {

public:
    using Edge = std::pair<EdgeCoord, int>;

    static constexpr std::size_t none = 0;

    Degree getNodeDeg(std::size_t node) const
    {
        return static_cast<T&>(*this).getNodeDeg(node);
    }

    std::size_t getGrapDeg() const
    {
        return static_cast<T&>(*this).graphDeg;
    }

    std::vector<std::size_t> getNodeIds() const noexcept
    {
        return static_cast<T&>(*this).getNodeIds();
    }

    std::vector<std::size_t> getNeighboursOf(std::size_t node) const
    {
        return static_cast<T&>(*this).getNeighboursOf(node);
    }

    int getEdgeWeight(const EdgeCoord& edge) const
    {
        return static_cast<T&>(*this).getEdgeWeight(edge);
    }

    void setEdge(const Edge& edge)
    {
        static_cast<T&>(*this).setEdge(edge);
    }

    void setEdgeSeries(const std::vector<Edge>& edges)
    {
        std::ranges::for_each(edges, [](const auto& edge) {
            static_cast<T&>(*this).setEdge(edge);
        });
    }

    std::size_t addNode(std::size_t currMaxNodeIndex = none)
    {
        return static_cast<T&>(*this).addNode(currMaxNodeIndex);
    }

    void addNodeSeries(std::size_t count)
    {
        std::size_t currMaxNodeIndex = none;
        for (std::size_t i = 0; i < count; i++)
        {
            currMaxNodeIndex = static_cast<T&>(*this).addNode(currMaxNodeIndex);
        }
    }

    void removeEdge(const EdgeCoord& edge)
    {
        static_cast<T&>(*this).removeEdge(edge);
    }

    void removeEdgeSeries(const std::vector<EdgeCoord>& edges)
    {
        std::ranges::for_each(edges, [](const auto& edge) {
            static_cast<T&>(*this).removeEdge(edge);
        });
    }

    void removeNode(std::size_t node)
    {
        static_cast<T&>(*this).removeNode(node);
    }

    void removeNodeSeries(const std::vector<std::size_t>& nodes)
    {
        std::ranges::for_each(nodes, [](const auto& node) {
            static_cast<T&>(*this).removeNode(node);
        });
    }

private:
    std::size_t graphDeg;
    std::vector<Degree> degrees;

    friend T;
    Graph() = default;
};
} // namespace graph