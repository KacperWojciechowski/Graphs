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
		List(std::string file_path, Type type);

		List(Matrix& matrix);
		List(Data::PixelMap& map);

		List(List& l);
		List(List&& l) noexcept;

		/* Interface */
		void print();
		void add_edge(std::size_t source, std::size_t destination, uint32_t weight);
		void add_node();

		void remove_edge(std::size_t source, std::size_t destination);
		void remove_node(std::size_t node_id);

		const std::size_t get_nodes_amount();
		const Degree get_node_degree(std::size_t node_id);
		const uint32_t get_edge(std::size_t source, std::size_t destination);
		const Type get_type();

		void save_graphml(std::ostream& stream, std::string name);

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
		void load_graphml_file(std::fstream& file);
		void calculate_degrees();

		/* Objects containing the graph */
		std::vector<std::list<Node>> list;
		std::vector<Degree> degrees;

		/* Graph type information */
		Type type;
	};
}
