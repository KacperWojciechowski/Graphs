#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\inc\GraphBase.h"
#include "..\inc\Algorithms.h"

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
		Matrix(std::vector<std::vector<int32_t>>& mat, Type type);
		
		Matrix(GraphBase& l);

		Matrix(Matrix& m);
		Matrix(Matrix&& m) noexcept;

		/* Common interface */
		auto print() const -> void;

		auto make_edge(std::size_t source, std::size_t destination, int32_t weight) -> void;
		auto add_node() -> void;

		auto remove_edge(std::size_t source, std::size_t destination) -> void;
		auto remove_node(std::size_t node_id) -> void;

		auto get_nodes_amount() const -> std::size_t;
		auto get_node_degree(std::size_t node_id) const -> Degree;
		auto get_edge(std::size_t source, std::size_t destination) const -> int32_t;
		auto get_type() const -> Type;

		auto save_graphml(std::ostream& stream, std::string name) const -> void;

		auto change_to_line_graph() const -> Matrix;

		/* Extended interface */
		auto load_throughtput(std::string file_path) -> void;
		auto print_throughtput() const -> void;

		/* Algorithmic interface*/

		/* Extended algorithmic interface */
		auto throughtput_bellman_ford(uint32_t start_vertex, uint32_t minimal_throughtput, bool log) -> Roadmap;
	
	
	private:
		/* Load functions for specific file formats */
		auto load_mat_file(std::istream& file) -> void;
		auto load_graphml_file(std::istream& file) -> void;
		auto calculate_degrees() -> void;

		/* Objects containing the graph*/
		std::vector<std::vector<int32_t>> matrix;		/**< Structure containing the adjacency matrix. Consists of std::vector of std::vector objects.*/
		std::vector<std::vector<int32_t>> throughtput; /**< Structure containing the throughtput matrix corresponding to the adjacency matrix. This matrix
															 has to be of the same dimentions as the adjacency matrix. This feature is exclusive to the matrix 
															 representation. */
		std::vector<Degree> degrees;					/**< Vector of structures storing the degrees of each vertex in Graph::Degree structures. The type of
															 the graph decides which members of the structure should be used. */
		Type type;										/**< Enum signifying the type of the graph. */
	};
}