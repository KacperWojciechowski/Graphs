#pragma once

#include <Graphs/Graph.hpp>
#include <Graphs/Pixel_map.hpp>
#include <map>
#include <string>

namespace Graphs
{
class AdjList : public Graph
{
public:
    AdjList() = default;
    AdjList(const Graph&);

    AdjList(std::string);
    // AdjList(const Data::Pixel_map&);

    AdjList(AdjList&) = delete;
    AdjList(AdjList&&) = delete;

    uint32_t nodesAmount() const override;
    uint32_t nodeDegree(NodeId) const override;
    EdgeInfo findEdge(const EdgeInfo&) const override;

    void setEdge(const EdgeInfo&) override;
    void addNodes(uint32_t) override;
    void removeNode(NodeId) override;
    void removeEdge(const EdgeInfo&) override;

    virtual std::vector<NodeId> getNodeIds() const override;
    virtual std::vector<NodeId> getNeighborsOf(NodeId) const override;

    virtual ~AdjList() = default;

private:
    std::string show() const override;

    using Neighbors = std::vector<uint32_t>;

    void buildFromLstFile(const std::string&);
    void removeNeighborFromRange(Neighbors&, NodeId);
    void addNeighborAndSortRange(Neighbors&, NodeId);

    std::vector<Neighbors> nodes;
    std::map<NodeId, uint32_t> nodeMap;
};
} // namespace Graphs
