#pragma once

#include <string>
#include <map>
#include <Graphs/Graph.h>
#include <Graphs/Pixel_map.h>

namespace Graph
{
	class Matrix;
}

namespace Graphs
{
	class AdjList : public Graph
	{
	public:
		/* Constructors */
		AdjList(std::string);

		AdjList(::Graph::Matrix&);
		AdjList(Data::Pixel_map&);

		AdjList(AdjList&) = delete;
		AdjList(AdjList&&) = delete;

		/* Basic interface */
		void print_deg();
		

		uint32_t get_neighbour(uint32_t x, uint32_t y);

		virtual void show() const override;
		virtual unsigned int nodesAmount() const override;
		virtual unsigned int nodeDegree(NodeId) const override;

		virtual void setEdge(EdgeInfo);
		virtual void addNode();
		virtual void removeNode(NodeId);
		virtual void removeEdge(EdgeInfo);


		/* Coloring algorithms with permutations */
		int32_t greedy_coloring(bool log = false);
		int32_t lf_coloring(bool log = false);
		int32_t sl_coloring(bool log = false);

		/* Destructor */
		~AdjList();
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