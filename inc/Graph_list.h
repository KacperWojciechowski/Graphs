#pragma once

#include <string>
#include <vector>
#include <list>

#include "..\inc\GraphBase.h"

// forward declaration of PixelMap class for convertion constructor
namespace Data
{
	class PixelMap;
}

namespace Graph
{
	/**
	 * \brief Class representing a graph in the adjacency list form.
	 * 
	 * The representation is based on the STL list and vector classes. It does provide a
	 * standard graph interface stated by the Graph virtual class.
	 * 
	 * \warning This class requires from the graph to have only non-negative weights of the edges.
	 */
	class List : public GraphBase
	{
	public:
		/* Constructors */
		List(const std::string& file_path, Type type);

		List(GraphBase& matrix);
		List(Data::PixelMap& map);

		List(List& l);
		List(List&& l) noexcept;

		/* Interface */
		void print();
		void make_edge(std::size_t source, std::size_t destination, int32_t weight);
		void add_node();

		void remove_edge(std::size_t source, std::size_t destination);
		void remove_node(std::size_t node_id);

		std::size_t get_nodes_amount();
		Degree get_node_degree(std::size_t node_id);
		int32_t get_edge(std::size_t source, std::size_t destination);
		Type get_type();

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
			std::size_t ID;  /**< Adjacent vertex ID */
			int32_t weight; /**< Weight of the connection */
		};

		/* Load functions for specific file formats */
		void load_lst_file(std::istream& file);
		void load_graphml_file(std::istream& file);
		void calculate_degrees();

		/* Objects containing the graph information */

		std::vector<std::list<Node>> list; /**< List structure of the graph. Consists of the std::vector 
												containing std::lists of neighbour vertices for a given vertex. */
		std::vector<Degree> degrees;	   /**< Vector containing degrees of each vertex. Degrees are stored within the Graph::Degree
												structure, and the type field decides which fields of the structure should be used. */
		Type type;						   /**< Enum signifying the type of the graph */
	};
}





