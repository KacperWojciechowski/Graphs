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
		/* Constructors */
		AdjList(std::string);

		AdjList(Graph&);
		AdjList(Data::Pixel_map&);

		AdjList(AdjList&) = delete;
		AdjList(AdjList&&) = delete;

		/* Basic interface */
		void print_deg();

		uint32_t get_neighbour(uint32_t x, uint32_t y);

		void show() const override;
		uint32_t nodesAmount() const override;
		uint32_t nodeDegree(NodeId) const override;
		uint32_t weightOf(const EdgeInfo&) const override;
		
		void setEdge(const EdgeInfo&) override;
		void addNode() override;
		void removeNode(NodeId) override;
		void removeEdge(const EdgeInfo&) override;


		/* Coloring algorithms with permutations */
		int32_t greedy_coloring(bool log = false);
		int32_t lf_coloring(bool log = false);
		int32_t sl_coloring(bool log = false);

		/* Destructor */
		virtual ~AdjList();
	private:
		/* Adjacency list node structore */
		struct Node
		{
			uint32_t neighbour;
			Node* next;
		};
		
		/* Node map to prevent renumeration during remove operations */
		std::map<int, int> node_map;

		/* Private variables */
		Node** nodes;
		uint32_t* degrees;

		/* Private functions */
		/* Permutations generation */
		void shuffle(std::vector<int>& v, bool log = false);
		/* Coloring algorithms core */
		int32_t greedy_coloring_core(std::map<int, int>* map, bool log = false);
	};
}