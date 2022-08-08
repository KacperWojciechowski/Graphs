#pragma once

#include <string>
#include <vector>
#include <list>

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
		List(std::string file_path, std::string name, Type type);

		List(Matrix& matrix);
		List(Data::PixelMap& map);

		List(List& l);
		List(List&& l) noexcept;

		/* Interface */
		void print();																	// checked
		void add_edge(std::size_t source, std::size_t destination, uint32_t weight);	// checked
		void add_node();

		void remove_edge(std::size_t source, std::size_t destination);
		void remove_node(std::size_t node_id);											// checked

		const std::size_t get_nodes_amount();
		const Degree get_node_degree(std::size_t node_id);
		const uint32_t get_edge(std::size_t source, std::size_t destination);
		const std::string get_name();
		const Type get_type();

		void save_graphml(std::string output_file_path);

		List change_to_line_graph();

	private:

		/* Adjacent vertex structure */

		/**
		 * \brief Structure containing the information regarding the connection
		 * between a given vertex and its adjacent vertex.
		 */
		struct Node
		{
			std::size_t ID; /**< Adjacent vertex ID */
			uint32_t weight; /**< Weight of the connection */
		};

		/* Load functions for specific file formats */
		void load_lst_file(std::fstream& file);
		void load_graphml_file(std::fstream& file);			// checked
		void calculate_degrees();							// checked

		/* Objects containing the graph */
		std::vector<std::list<Node>> list;
		std::vector<Degree> degrees;

		/* GRAPHML format information */
		std::string name;
		Type type;
	};
}
