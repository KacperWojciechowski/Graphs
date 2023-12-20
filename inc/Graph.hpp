#pragma once

#include <cstdint>
#include <vector>
#include <variant>
#include <iostream>
#include <algorithm>

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
};

using UndirectedDegree = std::size_t;
using Degree = std::variant<UndirectedDegree, DirectedDegree>;

class Graph {

public:
    using Edge = std::pair<EdgeCoord, int>;

    static constexpr std::size_t none = 0;

    friend std::ostream& operator<<(std::ostream& out, const Graph& graph) noexcept
    {
        graph.print(out);
        return out;
    }

    virtual void print(std::ostream& out) const = 0;
    virtual Degree getNodeDeg(std::size_t node) const = 0;
    virtual std::size_t getGraphDeg() const
    {
        return graphDeg;
    }

    virtual std::vector<std::size_t> getNodeIds() const noexcept = 0;
    virtual std::vector<std::size_t> getNeighboursOf(std::size_t node) const = 0;
    virtual int getEdgeWeight(const EdgeCoord& edge) const = 0;

    virtual void setEdge(const Edge& edge) = 0;
    virtual void setEdgeSeries(const std::vector<Edge>& edges)
    {
        std::ranges::for_each(edges, [this](const auto& edge) {
            setEdge(edge);
        });
    }

    virtual std::size_t addNode(std::size_t currMaxNodeIndex = none) = 0;
    virtual void addNodeSeries(std::size_t count)
    {
        std::size_t currMaxNodeIndex = none;
        for (std::size_t i = 0; i < count; i++)
        {
            currMaxNodeIndex = addNode(currMaxNodeIndex);
        }
    }

    virtual void removeEdge(const EdgeCoord& edge) = 0;
    virtual void removeEdgeSeries(const std::vector<EdgeCoord>& edges)
    {
        std::ranges::for_each(edges, [this](const auto& edge) {
            removeEdge(edge);
        });
    }

    virtual void removeNode(std::size_t node) = 0;
    virtual void removeNodeSeries(const std::vector<std::size_t>& nodes)
    {
        std::ranges::for_each(nodes, [this](const auto& node) {
            removeNode(node);
        });
    }

protected:
    GraphType graphType;
    std::size_t graphDeg;
};
} // namespace graph