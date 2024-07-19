#pragma once

// libraries
#include <stdint.h>
#include <string>
#include <vector>

// Graph
#include "Graph.h"
//#include "Graph_list.h"

namespace Data
{
	void generate_weighted_graph(std::string file_path, uint32_t nodes_amount);
	void generate_throughtput(std::string input_file_path, std::string output_file_path, uint32_t nodes_amount);
}

namespace Graph
{
	/* Forward declaration */
	class List;

	/*
		Class representing a graph in adjacency matrix form. Does not support
		multigraphs. Partially supports digraphs. Fully supports standard graphs
		without loops.
	*/
	class Matrix //: public Graph
	{
	public:
		/* Constructors */
		Matrix(std::string file_path, std::string throughtput_file_path, std::string name, std::string type);
		Matrix(List& list);

		Matrix(Matrix& m) = delete;
		Matrix(Matrix&& m) = delete;

		/* Interface */
		void print();
		void print_throughtput();
		void saveGraphML(std::string file_path);

		float estrada_index();

		void add_edge(uint32_t source, uint32_t destination);
		void add_node();

		void remove_edge(uint32_t source, uint32_t destination);
		void remove_node(uint32_t node_id);

		const uint32_t get_nodes_amount();
		const uint32_t get_value(uint32_t x, uint32_t y);

		void change_to_line_graph();

		int32_t belman_ford(uint32_t vertex, bool log);
		int32_t throughtput_belman_ford(uint32_t searched_throughtput, uint32_t vertex, bool log);

		/* Destructor */
		~Matrix();

	private:
		/* Custom data types */
		struct coord
		{
			uint32_t x;
			uint32_t y;
		};

		/* Private functions */
		uint32_t find_index(std::vector<coord>& edges, uint32_t x, uint32_t y);

		/* Private variables */
		uint32_t nodes_amount;
		uint32_t** matrix;
		std::string graph_name;
		std::string graph_type;

		uint32_t* distance;
		std::vector<uint32_t>* prev_node;
		uint32_t** throughtput;
	};
} // Graph