#pragma once

#include <iostream>
#include <cstdint>
#include <string>

namespace Graph
{
	/**
	 * \brief Enum class stating the type of the graph.
	 */
	enum class [[nodiscard]] Type
	{
		undirected = 0, /**< The graph is undirected, which means that the weights of the connections are automatically 
							 mirrored */
		directed,		/**< The graph is directed, which means that the weights are not automatically mirrored */
	};


	/**
	 * \brief Structure containing the degree information of given vertex.
	 *
	 * \note User should access the right set of member fields based on the type of the graph.
	 *		 In case of accessing eg. deg, while having an undirected graph, the result might be incorrect,
	 *		 hence should not be taken into an account.
	 */
	struct [[nodiscard]] Degree
	{
		std::size_t in_deg; /**< Indegree for the directed graphs */
		std::size_t out_deg; /**< Outdegree for the directed graphs */
		std::size_t deg; /**< Degree for the undirected graphs */
	};

	/**
	 * \brief Abstract class stating the common interface for each of the graph representation.
	 */
	class GraphBase
	{
	public:

		// basic operations interface -------------------------------------------------------------------------------------------

		/**
		 * \brief Common function definition for adding an edge to the graph object.
		 *
		 * \param Source source vertex of an edge.
		 * \param Destination end vertex of an edge.
		 * \param Weight weight of the connection.
		 */
		virtual auto addEdge(std::size_t source, std::size_t destination, int32_t weight) -> void = 0;




		/**
		 * \brief Common function definition for adding a vertex to the graph object.
		 *
		 */
		virtual auto addNode() -> void = 0;




		/**
		 * \brief Common function definition for removing an edge from the graph object.
		 *
		 * \param Source source vertex of an edge.
		 * \param Destionation end vertex of an edge.
		 */
		virtual auto removeEdge(std::size_t source, std::size_t destionation) -> void = 0;




		/**
		 * \brief Common function definition for removing a vertex from the graph object.
		 *
		 * \param node_id ID of the vertex (counting from 0) to remove.
		 */
		virtual auto removeNode(std::size_t node_id) -> void = 0;




		/**
		 * \brief Common function definition for acquiring the vertices amount in the graph object.
		 *
		 * \return The amount of the vertices in a graph object.
		 */
		[[nodiscard]] virtual auto getNodesAmount() const -> std::size_t = 0;




		/**
		 * \brief Common function definition for acquiring the degree of given vertex.
		 *
		 * \param node_id ID of a vertex (counting from 0) to get the degree of.
		 * \return The degree of a given vertex.
		 */
		[[nodiscard]] virtual auto getNodeDegree(std::size_t node_id) const -> Degree = 0;




		/**
		 * \brief Common function definition for acquiring the weight of an edge between two vertices.
		 *
		 * \param source Source vertex of an edge.
		 * \param destination End vertex of an edge.
		 * \return Weight value of the connection.
		 */
		[[nodiscard]] virtual auto getEdge(std::size_t source, std::size_t destination) const -> int32_t = 0;




		[[nodiscard]] virtual auto getType() const -> Type = 0;




		// output operations interface ------------------------------------------------------------------------------------------

		/**
		 * \brief Common function definintion for saving the graph into a .GRAPHML format file.
		 *
		 * \param output_file_path Path of the destination file.
		 */
		virtual auto saveToGraphml(std::ostream& stream, std::string name) const -> void = 0;



	protected:
		// input operations interface -------------------------------------------------------------------------------------------

		/**
		 * \brief Common function definition for loading the graph from a .GRAPHML format file.
		 *
		 * \param file Reference to the std::fstream object of the graphml file.
		 */
		virtual auto loadGraphmlFile(std::istream& file) -> void = 0;




		// internal calculations ------------------------------------------------------------------------------------------------

		/**
		 * \brief Common function definition for calculating the degrees of each vertex inside the graph object.
		 *
		 */
		virtual auto calculateDegrees() -> void = 0;
	};
}