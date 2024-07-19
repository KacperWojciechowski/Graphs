#pragma once

#include <string>
#include <map>
#include <Graphs/Graph.hpp>
#include <Graphs/Pixel_map.hpp>

namespace Graphs
{
	class AdjList : public Graph
	{
	public:
		AdjList(std::string);

		AdjList(const Graph&);
		AdjList(const Data::Pixel_map&);

		AdjList(AdjList&) = delete;
		AdjList(AdjList&&) = delete;

		uint32_t nodesAmount() const override;
		uint32_t nodeDegree(NodeId) const override;
		EdgeInfo findEdge(const EdgeInfo&) const override;
		
		void setEdge(const EdgeInfo&) override;
		void addNode() override;
		void removeNode(NodeId) override;
		void removeEdge(const EdgeInfo&) override;


		/* Coloring algorithms with permutations */
		int32_t greedy_coloring(bool log = false);
		int32_t lf_coloring(bool log = false);
		int32_t sl_coloring(bool log = false);

		/* Destructor */
		virtual ~AdjList() = default;
	private:
		std::string show() const override;

		using Neighbors = std::vector<uint32_t>;

		void removeNeighborFromRange(Neighbors& range, NodeId tgtNeighbor);
		void addNeighborAndSortRange(Neighbors& range, NodeId tgtNeighbor);
		void shuffle(std::vector<int>& v, bool log = false);
		int32_t greedy_coloring_core(std::map<int, int>* map, bool log = false);
		
		std::vector<Neighbors> nodes;
		std::map<uint32_t, uint32_t> nodeMap;
	};
}