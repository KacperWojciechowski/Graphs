#pragma once

#include <Graph.hpp>

#include <map>
#include <algorithm>

namespace graph
{
class FileParser;

class AdjList final : public Graph<AdjList>
{
public:
	AdjList(const AdjList& l) = default;
	AdjList(AdjList&& l) = default;

	friend std::ostream& operator<<(std::ostream& out, const graph::AdjList& graph) noexcept;

	Degree getNodeDeg(std::size_t node) const noexcept
	{
		return degrees[nodeMap.at(node)];
	}

	std::vector<std::size_t> getNodeIds() const noexcept
	{
		std::vector<std::size_t> nodeIdx = {};
		std::ranges::for_each(nodeMap, [&nodeIdx](const auto& node) {
			nodeIdx.emplace_back(node.first);
		});
		return nodeIdx;
	}

	std::vector<std::size_t> getNeighboursOf(std::size_t node) const;

	int getEdgeWeight(const EdgeCoord& edge) const;

	void setEdge(const Edge& edge);
	std::size_t addNode(std::size_t currMaxNodeIndex = none);
	void removeEdge(const EdgeCoord& edge);
	void removeNode(std::size_t node);

	~AdjList() = default;

private:
	using NodeInformation = std::vector<Edge>;

	using Graph<AdjList>::graphType;
	using Graph<AdjList>::graphDeg;
	using Graph<AdjList>::degrees;

	friend FileParser;

	AdjList(GraphType graphType) : Graph(graphType) { };

	std::map<std::size_t, std::size_t> nodeMap;
	std::vector<NodeInformation> adjList;
};
} // graph