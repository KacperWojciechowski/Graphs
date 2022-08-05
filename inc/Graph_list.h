#pragma once

#include <string>

#include "PixelMap.h"
#include "Graph_matrix.h"
#include "Graph.h"

namespace Graph
{
	// forward declaration
	class Matrix;

	/**
	 * \brief Class representing a graph in the adjacency list form.
	 * 
	 * The representation is based on the STL list and vector classes. It does provide a
	 * standard graph interface stated by the Graph virtual class.
	 */
	class List : public Graph
	{
	public:
		/* Constructors */
		List(std::string file_path);

		List(Matrix& matrix);
		List(Data::PixelMap& map);

		List(List& l);

		List(List&& l) = delete;

		void print();
		void add_edge(std::size_t source, std::size_t destination, uint32_t weight);
		void add_node();

		void remove_edge(std::size_t source, std::size_t destination);
		void remove_node(std::size_t node_id);

		const std::size_t get_nodes_amount();
		const std::size_t get_node_degree(std::size_t node_id);
		const int32_t get_edge(std::size_t source, std::size_t destination);

		void save_graphml(std::string output_file_path);

	private:

		void load_graphml_file(std::fstream& file);
		void calculate_degrees();

	};
}
