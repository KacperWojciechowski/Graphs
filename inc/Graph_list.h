#pragma once

// libraries
#include <string>
#include <map>

// Graph
#include "Graph_matrix.h"
#include "Pixel_map.h"
#include "Graph.h"

namespace Graph
{
	/* Forward declaration */
	class Matrix;

	/*
		Class representing a graph in the adjacency list form. To create an
		object, it is required to load a graph from a .lst file.
	*/
	class List //: public Graph
	{
	public:
		/* Constructors */
		List(std::string file_path);

		List(Matrix& matrix);
		List(Data::Pixel_map& map);

		List(List& l) = delete;
		List(List&& l) = delete;

		/* Basic interface */
		void print();
		void print_deg();
		
		void add_edge(uint32_t source, uint32_t destination);
		void add_node();

		void remove_edge(uint32_t source, uint32_t destination);
		void remove_node(uint32_t node_id);

		uint32_t get_nodes_amount();
		uint32_t get_node_degree(uint32_t node);
		uint32_t get_neighbour(uint32_t x, uint32_t y);

		/* Coloring algorithms with permutations */
		int32_t greedy_coloring(bool log = false);
		int32_t lf_coloring(bool log = false);
		int32_t sl_coloring(bool log = false);

		/* Destructor */
		~List();
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