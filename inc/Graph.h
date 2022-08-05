#pragma once

#include <cstdint>
#include <string>

namespace Graph
{
	/**
	 * \brief Enum class stating the type of the graph.
	 */
	enum class Type
	{
		undirected = 0, /**< The graph is undirected, which means that the weights of the connections are automatically 
						mirrored */
		directed, /**< The graph is directed, which means that the weights are not automatically mirrored */
		undefined /**< The graph type is undefined. In majority of cases, operations on such graph are either
						incorrect, or the graph type is assumed as directed */
	};

	/**
	 * \brief Abstract class stating the common interface for each of the graph representation.
	 */
	class Graph
	{
	public:

		// basic operations interface -------------------------------------------------------------------------------------------

		/**
		 * \brief Comomn function definition for printing the representation of a graph.
		 * 
		 */
		virtual void print() = 0;




		/**
		 * \brief Common function definition for adding an edge to the graph object.
		 * 
		 * \param Source source vertex of an edge.
		 * \param Destination end vertex of an edge.
		 * \param Weight weight of the connection.
		 */
		virtual void add_edge(std::size_t source, std::size_t destination, uint32_t weight) = 0;




		/**
		 * \brief Common function definition for adding a vertex to the graph object.
		 * 
		 */
		virtual void add_node() = 0;




		/**
		 * \brief Common function definition for removing an edge from the graph object.
		 * 
		 * \param Source source vertex of an edge.
		 * \param Destionation end vertex of an edge.
		 */
		virtual void remove_edge(std::size_t source, std::size_t destionation) = 0;




		/**
		 * \brief Common function definition for removing a vertex from the graph object.
		 * 
		 * \param node_id ID of the vertex (counting from 0) to remove.
		 */
		virtual void remove_node(std::size_t node_id) = 0;




		/**
		 * \brief Common function definition for acquiring the vertices amount in the graph object.
		 * 
		 * \return The amount of the vertices in a graph object.
		 */
		virtual const std::size_t get_nodes_amount() = 0;




		/**
		 * \brief Common function definition for acquiring the degree of given vertex.
		 * 
		 * \param node_id ID of a vertex (counting from 0) to get the degree of.
		 * \return The degree of a given vertex.
		 */
		virtual const std::size_t get_node_degree(std::size_t node_id) = 0;




		/**
		 * \brief Common function definition for acquiring the weight of an edge between two vertices.
		 * 
		 * \param source Source vertex of an edge.
		 * \param destination End vertex of an edge.
		 * \return Weight value of the connection.
		 */
		virtual const int32_t get_edge(std::size_t source, std::size_t destination) = 0;




		/**
		 * \brief Common function definition for name getter.
		 */
		virtual const std::string get_name() = 0;




		/**
		 * \brief Common function definition for type getter.
		 */
		virtual const Type get_type() = 0;




		// output operations interface ------------------------------------------------------------------------------------------
		
		/**
		 * \brief Common function definintion for saving the graph into a .GRAPHML format file.
		 * 
		 * \param output_file_path Path of the destination file.
		 */
		virtual void save_graphml(std::string output_file_path) = 0;



	protected:
		// input operations interface -------------------------------------------------------------------------------------------

		/**
		 * \brief Common function definition for loading the graph from a .GRAPHML format file.
		 * 
		 * \param file Reference to the std::fstream object of the graphml file.
		 */
		virtual void load_graphml_file(std::fstream& file) = 0;




		// internal calculations ------------------------------------------------------------------------------------------------

		/**
		 * \brief Common function definition for calculating the degrees of each vertex inside the graph object.
		 *  
		 */
		virtual void calculate_degrees() = 0;
	};
}