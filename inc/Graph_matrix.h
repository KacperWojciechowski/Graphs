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
	class Matrix : public GraphBase//, public Algorithms
	{
	public:

		/* Constructors */
		Matrix(const std::string& file_path, Type type);
		Matrix(const std::vector<std::vector<int32_t>>& mat, Type type) noexcept;
		
		Matrix(const GraphBase& l) noexcept;

		Matrix(const Matrix& m) noexcept;
		Matrix(Matrix&& m) noexcept;

		/* Common interface */
		auto print() const noexcept -> void;

		auto make_edge(std::size_t source, std::size_t destination, int32_t weight) -> void;
		auto add_node() noexcept -> void;

		auto remove_edge(std::size_t source, std::size_t destination) -> void;
		auto remove_node(std::size_t node_id) -> void;

		auto [[nodiscard]] get_nodes_amount() const noexcept -> std::size_t
		{
			return matrix.size();
		}

		/**
		 * \brief Getter for the degree of given vertex.
		 * 
		 * \param node_id ID of the vertex which degree should be returned.
		 * \return Degree of the given vertex
		 * 
		 * \warning Exception to guard against:
		 *		- std::out_of_range - when given ID exceeds the count of vertices.
		 */
		auto get_node_degree(std::size_t node_id) const -> Degree
		{
			if (node_id >= degrees.size())
				throw std::out_of_range("ID out of bounds");
			else return degrees[node_id];
		}

		/**
		 * \brief Getter for the weight of a connection between given vertices.
		 *
		 * \param source ID of the source vertex of the edge (counting from 0).
		 * \param destination ID of the end vertex of the edge (counting from 0).
		 * \return Value of the weight of the edge.
		 *
		 * \note Return value of zero means no connection is present.
		 *
		 * \warning Exception to guard against:
		 *		- std::out_of_range - when either of the IDs exceeds the count of vertices.
		 */
		auto [[nodiscard]] get_edge(std::size_t source, std::size_t destination) const -> int32_t
		{
			if (source >= matrix.size() || destination >= matrix.size())
			{
				throw std::out_of_range("Index out of bounds");
			}
			else
			{
				return matrix[source][destination];
			}
		}

		auto [[nodiscard]] get_type() const noexcept -> Type
		{
			return type;
		}

		auto save_graphml(std::ostream& stream, std::string name) const noexcept -> void;

		auto [[nodiscard]] change_to_line_graph() const noexcept -> Matrix;

		/* Extended interface */
		auto load_throughtput(const std::string& file_path) -> void;
		auto print_throughtput() const -> void;

		/* Algorithmic interface*/
		auto greedy_coloring(bool permutate = false, std::ostream* log_stream = nullptr) const noexcept -> Coloring;
		auto lf_coloring(bool permutate = false, std::ostream* log_stream = nullptr) const noexcept -> Coloring;
		auto sl_coloring(bool permutate = false, std::ostream* log_stream = nullptr) const noexcept -> Coloring;

		auto bellman_ford(std::size_t start, const std::ostream* log_stream = nullptr) const noexcept -> Roadmap;

		/* Extended algorithmic interface */
		auto throughtput_bellman_ford(uint32_t start_vertex, uint32_t minimal_throughtput, std::ostream* log_stream) const noexcept -> Roadmap;
	
	
	private:
		/* Load functions for specific file formats */
		auto load_mat_file(std::istream& file) -> void;
		auto load_graphml_file(std::istream& file) -> void;
		auto calculate_degrees() -> void;

		/* Private algorithmic functions */
		auto greedy_coloring_core(const std::map<std::size_t, std::size_t>& m, std::ostream* log_stream = nullptr) const noexcept -> Coloring;

		/* Objects containing the graph */
		std::vector<std::vector<int32_t>> matrix;		/**< Structure containing the adjacency matrix. Consists of std::vector of std::vector objects.*/
		std::vector<std::vector<int32_t>> throughtput;  /**< Structure containing the throughtput matrix corresponding to the adjacency matrix. This matrix
															 has to be of the same dimentions as the adjacency matrix. This feature is exclusive to the matrix 
															 representation. */
		std::vector<Degree> degrees;					/**< Vector of structures storing the degrees of each vertex in Graph::Degree structures. The type of
															 the graph decides which members of the structure should be used. */
		Type type;										/**< Enum signifying the type of the graph. */
	};
}