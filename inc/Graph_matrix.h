#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "GraphBase.h"
#include "Algorithms.h"

namespace Graph
{

	/**
	 * @brief This class provides an adjacency matrix graph representation.
	 * 
	 * The representation is based on the STL vector class. It does provide a
	 * standard graph interface stated by the Graph virtual class. 
	 * 
	 * \warning This class requires from graph have only non-negative weights of the edges.
	 */
	class Matrix : public GraphBase
	{
	public:

		/* Constructors */
		Matrix(std::string file_path, Type type);
		Matrix(std::vector<std::vector<uint32_t>>& mat, Type type);
		
		Matrix(GraphBase& l);

		Matrix(Matrix& m);
		Matrix(Matrix&& m) noexcept;

		/* Common interface */
		void print();

		void make_edge(std::size_t source, std::size_t destination, uint32_t weight);
		void add_node();

		void remove_edge(std::size_t source, std::size_t destination);
		void remove_node(std::size_t node_id);

		std::size_t get_nodes_amount();
		Degree get_node_degree(std::size_t node_id);
		uint32_t get_edge(std::size_t source, std::size_t destination);
		Type get_type();

		void save_graphml(std::ostream& stream, std::string name);

		Matrix change_to_line_graph();

		/* Extended interface */
		void load_throughtput(std::string file_path);
		void print_throughtput();

		/* Algorithmic interface*/


		/* Extended algorithmic interface */
		Roadmap throughtput_belman_ford(uint32_t start_vertex, uint32_t minimal_throughtput, bool log);
	
	
	private:
		/* Load functions for specific file formats */
		void load_mat_file(std::istream& file);
		void load_graphml_file(std::istream& file);
		void calculate_degrees();

		/* Objects containing the graph*/
		std::vector<std::vector<uint32_t>> matrix;		/**< Structure containing the adjacency matrix. Consists of std::vector of std::vector objects.*/
		std::vector<std::vector<uint32_t>> throughtput; /**< Structure containing the throughtput matrix corresponding to the adjacency matrix. This matrix
															 has to be of the same dimentions as the adjacency matrix. This feature is exclusive to the matrix 
															 representation. */
		std::vector<Degree> degrees;					/**< Vector of structures storing the degrees of each vertex in Graph::Degree structures. The type of
															 the graph decides which members of the structure should be used. */
		Type type;										/**< Enum signifying the type of the graph. */
	};
}