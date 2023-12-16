#pragma once

#include <Graph.hpp>

//#include "Pixel_map.h"

#include <string>
#include <string_view>
#include <stdexcept>
#include <map>

namespace graph
{
	class Matrix;
	//uint32_t find_index(std::vector<Data::coord>& nodes, uint32_t x, uint32_t y);

	class AdjList : public Graph
	{
	public:
		AdjList(std::string_view file_path);

		AdjList(Matrix& matrix);
		//AdjList(Data::Pixel_map& map);

		AdjList(const AdjList& l) = default;
		AdjList(AdjList&& l) = default;

		void print(std::ostream& out) const noexcept;

		Degree getNodeDeg(std::size_t node) const noexcept
		{
			return adjList[nodeMap.at(node)].first;
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

		/*
        int32_t greedy_coloring_core(std::map<int, int>* map, bool log);
        int32_t greedy_coloring(bool log);
        int32_t lf_coloring(bool log);
        int32_t sl_coloring(bool log);
        void shuffle(std::vector<int>& v, bool log);*/

		~AdjList() = default;
	private:
		using NodeInformation = std::pair<Degree, std::vector<Edge>>;

		std::map<std::size_t, std::size_t> nodeMap;

		std::vector<NodeInformation> adjList;
	};
}