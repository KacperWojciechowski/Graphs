#pragma once

#include <vector>
#include <variant>
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>

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

using Edge = std::pair<EdgeCoord, int>;

struct DirectedDegree
{
    std::size_t inDeg = 0;
    std::size_t outDeg = 0;
};

using UndirectedDegree = std::size_t;
using Degree = std::variant<UndirectedDegree, DirectedDegree>;

template<typename T>
class Graph
{
public:
    static constexpr std::size_t none = 0;

    template<typename T1>
    friend std::ostream& operator<<(std::ostream& out, const graph::Graph<T1>& graph) noexcept;

    Degree getNodeDeg(std::size_t node) const
    {
        return static_cast<T&>(*this).getNodeDeg(node);
    }

    std::size_t getGraphDeg() const
    {
        return graphDeg;
    }

    GraphType getGraphType() const
    {
        return graphType;
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
        std::ranges::for_each(edges, [this](const auto& edge) {
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
        std::ranges::for_each(edges, [this](const auto& edge) {
            static_cast<T&>(*this).removeEdge(edge);
        });
    }

    void removeNode(std::size_t node)
    {
        static_cast<T&>(*this).removeNode(node);
    }

    void removeNodeSeries(const std::vector<std::size_t>& nodes)
    {
        std::ranges::for_each(nodes, [this](const auto& node) {
            static_cast<T&>(*this).removeNode(node);
        });
    }


protected:
    GraphType graphType;
    std::size_t graphDeg;
    std::vector<Degree> degrees;
    std::map<std::size_t, std::size_t> nodeMap;

    friend T;
    Graph(GraphType graphType) : graphType(graphType) { };

    void calculageGraphDeg()
    {
        auto comparison = getDegCompFunction();
        auto deg = std::ranges::max(degrees, comparison);
        if (std::holds_alternative<UndirectedDegree>(degrees[0]))
        {
            graphDeg = std::get<UndirectedDegree>(deg);
            return;
        }
        graphDeg = std::get<DirectedDegree>(deg).outDeg;
    }

    void calculateDegrees(const EdgeCoord& edge, int modifier)
    {
        auto& sourceDeg = degrees[nodeMap.at(edge.source)];
        auto& targetDeg = degrees[nodeMap.at(edge.target)];
        outDegModifyFunc(sourceDeg, modifier);
        inDegModifyFunc(targetDeg, modifier);
        calculageGraphDeg();
    }

private:
    std::function<bool(Degree, Degree)> getDegCompFunction()
    {
        if (graphType == GraphType::undirected)
        {
            return [this](const auto& lhs, const auto& rhs) {
                return std::get<UndirectedDegree>(lhs) < std::get<UndirectedDegree>(rhs);
            };
        }
        return [this](const auto& lhs, const auto& rhs) {
                return std::get<DirectedDegree>(lhs).outDeg < std::get<DirectedDegree>(rhs).outDeg;
            };
    }

    void inDegModifyFunc(Degree& degree, int modifier)
    {
        if (std::holds_alternative<UndirectedDegree>(degree))
        {
            std::get<UndirectedDegree>(degree) += modifier;
        }
        else
        {
            std::get<DirectedDegree>(degree).inDeg += modifier;
        }
    }

    void outDegModifyFunc(Degree& degree, int modifier)
    {
        if (std::holds_alternative<UndirectedDegree>(degree))
        {
            std::get<UndirectedDegree>(degree) += modifier;
        }
        else
        {
            std::get<DirectedDegree>(degree).outDeg += modifier;
        }
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const graph::Graph<T>& graph) noexcept
{
    out << static_cast<const T&>(graph) << "\n";
    return out;
}
} // namespace graph
