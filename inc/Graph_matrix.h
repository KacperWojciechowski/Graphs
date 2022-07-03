#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#include "Graph.h"

namespace Graph
{
	class Matrix : public Graph
	{
	public:

		/* Constructors */
		Matrix(std::string file_path, std::string name, Type type);
		Matrix(Matrix& m);
		Matrix(Matrix&& m) = delete;

		/* Interface */
		void print();

		void add_edge(uint32_t source, uint32_t destination);
		void add_node();

		void remove_edge(uint32_t source, uint32_t destination);
		void remove_node(uint32_t node_id);

		const std::size_t get_nodes_amount();
		const std::size_t get_node_degreee();
		const uint8_t get_edge(uint32_t source, uint32_t destionation);

		void change_to_line_graph();

		void load_throughtput(std::string file_path);

	private:
		/* Load functions for specific file formats */
		void load_mat_file(std::fstream& file);
		void load_graphml_file(std::fstream& file);

		/* Objects containing the graph */
		std::vector<std::vector<uint8_t>> matrix;
		std::vector<std::vector<uint32_t>> throughtput;

		/* GraphML format information */
		std::string name;
		Type type;
	};
}