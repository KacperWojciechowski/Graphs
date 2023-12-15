#pragma once

#include "Graph_matrix.h"
#include "Pixel_map.h"

#include <string>
#include <map>

namespace Graph
{
	class Matrix;

	uint32_t find_index(std::vector<Data::coord>& nodes, uint32_t x, uint32_t y);

	class List
	{
	public:
		List(std::string file_path);

		List(Matrix& matrix);
		List(Data::Pixel_map& map);

		List(List& l) = delete;
		List(List&& l) = delete;

		void print();
		void print_deg();

		uint32_t get_nodes_amount();
		uint32_t get_node_degree(uint32_t node);
		uint32_t get_neighbour(uint32_t x, uint32_t y);

		void add_edge(uint32_t source, uint32_t destination);
		void remove_edge(uint32_t source, uint32_t destination);

		void add_node();

		void remove_node(uint32_t node_id);

        int32_t greedy_coloring_core(std::map<int, int>* map, bool log);
        int32_t greedy_coloring(bool log);
        int32_t lf_coloring(bool log);
        int32_t sl_coloring(bool log);
        void shuffle(std::vector<int>& v, bool log);

		~List();
	private:
		struct Node
		{
			uint32_t neighbour;
			Node* next;
		};

		std::map<int, int> node_map;

		Node** nodes;
		uint32_t* degrees;
	};
}