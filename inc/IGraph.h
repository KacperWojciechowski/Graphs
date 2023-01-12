#pragma once

#include <iostream>
#include <cstdint>
#include <string>

namespace Graph
{
	enum class [[nodiscard]] GraphType
	{
		undirected = 0,
		directed,
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

	class IGraph
	{
	public:

		virtual auto addEdge(std::size_t source, std::size_t destination, int32_t weight) -> void = 0;
		virtual auto addNode() -> void = 0;
		virtual auto removeEdge(std::size_t source, std::size_t destionation) -> void = 0;
		virtual auto removeNode(std::size_t node_id) -> void = 0;

		[[nodiscard]] virtual auto getNodesAmount() const -> std::size_t = 0;
		[[nodiscard]] virtual auto getNodeDegree(std::size_t node_id) const -> Degree = 0;
		[[nodiscard]] virtual auto getEdge(std::size_t source, std::size_t destination) const -> int32_t = 0;
		[[nodiscard]] virtual auto getType() const -> GraphType = 0;

	protected:
		virtual auto calculateDegrees() -> void = 0;

		auto [[nodiscard]] getGraphType() const noexcept -> GraphType
		{
			return type;
		}

		GraphType type;
	};
}