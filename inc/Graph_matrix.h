#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include <iostream>

#include "Graph.h"
#include "Algorithms.h"

namespace Graph
{
	/**
	 * @brief This class provides an adjacency matrix graph representation.
	 * 
	 * The representation is based on the STL vector class. It does provide a
	 * standard graph interface stated by the Graph virtual class. 
	 */
	class Matrix : public Graph
	{
	public:

		/* Constructors */
		Matrix(std::string file_path, std::string name, Type type);
		Matrix(std::vector<std::vector<uint32_t>>& mat, std::string name, Type type);
		
		Matrix(Matrix& m);
		Matrix(Matrix&& m) noexcept;

		/* Interface */
		void print();

		void add_edge(std::size_t source, std::size_t destination, uint32_t weight);
		void add_node();

		void remove_edge(std::size_t source, std::size_t destination);
		void remove_node(std::size_t node_id);

		const std::size_t get_nodes_amount();
		const Degree get_node_degree(std::size_t node_id);
		const uint32_t get_edge(std::size_t source, std::size_t destination);
		const std::string get_name();
		const Type get_type();

		void save_graphml(std::string output_file_path);

		Matrix change_to_line_graph();

		void load_throughtput(std::string file_path);

		// algorithmic interface


		Roadmap throughtput_belman_ford(uint32_t start_vertex, uint32_t minimal_throughtput, bool log);
	
	
	private:
		/* Load functions for specific file formats */
		void load_mat_file(std::fstream& file);
		void load_graphml_file(std::fstream& file);
		void calculate_degrees();

		/* Objects containing the graph */
		std::vector<std::vector<uint32_t>> matrix;
		std::vector<std::vector<uint32_t>> throughtput;
		std::vector<Degree> degrees;

		/* GraphML format information */
		std::string name;
		Type type;
	};
}