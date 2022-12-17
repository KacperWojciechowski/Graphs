#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "GraphBase.h"
#include "Algorithms.h"

namespace 
{
	enum class MatrixFileType
	{
		MAT = 0,
		GRAPHML,
		UNSUPPORTED
	};

	struct Source 
	{
		std::ifstream& stream;
		MatrixFileType type;
	};
}

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

		/* Factory functions */
		using DynamicMatrix = std::vector<std::vector<int32_t>>;

		[[nodiscard]] static auto constructFromFile(const std::string& path, Type graphType) -> Matrix;
		[[nodiscard]] static auto constructFromDynamicMatrix(DynamicMatrix&& mat, Type graphType) -> Matrix;

		/* Constructors */
		Matrix(const GraphBase& l) noexcept;
		Matrix(const Matrix& m) noexcept;
		Matrix(Matrix&& m) noexcept;

		/* Operators */
		bool operator==(const Matrix& mat) noexcept;
		friend auto operator<< (std::ostream& out, const Matrix& mat) noexcept -> std::ostream&;

		/* Getters */
		[[nodiscard]] auto getType() const noexcept -> Type
		{
			return type;
		}








		/* Common interface */
		auto print() const noexcept -> void;

		auto addEdge(std::size_t source, std::size_t destination, int32_t weight) -> void;
		auto addNode() noexcept -> void;

		auto removeEdge(std::size_t source, std::size_t destination) -> void;
		auto removeNode(std::size_t node_id) -> void;

		[[nodiscard]] auto getNodesAmount() const noexcept -> std::size_t
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
		auto getNodeDegree(std::size_t node_id) const -> Degree
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
		[[nodiscard]] auto getEdge(std::size_t source, std::size_t destination) const -> int32_t
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

		

		auto saveToGraphml(std::ostream& stream, std::string name) const noexcept -> void;

		[[nodiscard]] auto changeToLineGraph() const noexcept -> Matrix;

		/* Extended interface */
		auto loadThroughtput(const std::string& file_path) -> void;
		auto printThroughtput() const -> void;

		/* Algorithmic interface*/
		auto greedy_coloring(bool permutate = false, std::ostream* log_stream = nullptr) const noexcept -> Coloring;
		auto lf_coloring(bool permutate = false, std::ostream* log_stream = nullptr) const noexcept -> Coloring;
		auto sl_coloring(bool permutate = false, std::ostream* log_stream = nullptr) const noexcept -> Coloring;

		auto bellman_ford(std::size_t start, const std::ostream* log_stream = nullptr) const noexcept -> Roadmap;

		/* Extended algorithmic interface */
		auto throughtput_bellman_ford(uint32_t start_vertex, uint32_t minimal_throughtput, std::ostream* log_stream) const noexcept -> Roadmap;

	private:
		Matrix(Source source, Type type);
		Matrix(DynamicMatrix&& mat, Type type) noexcept;

		/* Print formatting functions */
		auto formatType() const -> std::string;
		auto formatLine(const std::vector<int32_t>& row, std::size_t rowIndex) const -> std::string;
		auto formatRow(const std::vector<int32_t>& row) const -> std::string;
		auto formatDegree(std::size_t rowIndex) const -> std::string;




		/* Load functions for specific file formats */
		auto loadMatFile(std::istream& file) -> void;
		auto loadGraphmlFile(std::istream& file) -> void;
		auto calculateDegrees() noexcept -> void;

		/* Private algorithmic functions */
		auto greedy_coloring_core(const std::map<std::size_t, std::size_t>& m, std::ostream* log_stream = nullptr) const noexcept -> Coloring;

		/* Objects containing the graph */
		
		DynamicMatrix matrix;		/**< Structure containing the adjacency matrix. Consists of std::vector of std::vector objects.*/
		DynamicMatrix throughtput;  /**< Structure containing the throughtput matrix corresponding to the adjacency matrix. This matrix
															 has to be of the same dimentions as the adjacency matrix. This feature is exclusive to the matrix 
															 representation. */
		std::vector<Degree> degrees;					/**< Vector of structures storing the degrees of each vertex in Graph::Degree structures. The type of
															 the graph decides which members of the structure should be used. */
		Type type;										/**< Enum signifying the type of the graph. */
	};
}