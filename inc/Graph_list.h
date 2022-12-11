#pragma once

#include <string>
#include <vector>

#include "GraphBase.h"

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
		/**
		 * \brief Structure containing the information regarding the connection
		 * between a given vertex and its adjacent vertex.
		*/
		struct Node
		{
			std::size_t ID;  /**< Adjacent vertex ID */
			int32_t weight; /**< Weight of the connection */
		};


		/* Constructors */
		List(const std::string& file_path, Type type);

		List(const GraphBase& matrix) noexcept;
		List(const Data::PixelMap& map) noexcept;

		List(const List& l) noexcept;
		List(List&& l) noexcept;

		/* Interface */
		auto print() const noexcept -> void;
		auto make_edge(std::size_t source, std::size_t destination, int32_t weight) -> void;
		auto add_node() noexcept -> void;

		auto remove_edge(std::size_t source, std::size_t destination) -> void;
		auto remove_node(std::size_t node_id) -> void;

		[[nodiscard]] auto get_nodes_amount() const noexcept -> std::size_t
		{
			return list.size();
		}

		/**
		 * Getter for a vertex degree.
		 *
		 * \warning Exception to guard against:
		 *		- std::out_of_range - given vertex ID is out of range.
		 *
		 * \param node_id ID of the vertex.
		 * \return
		 */
		auto get_node_degree(std::size_t node_id) const -> Degree
		{
			if (node_id >= degrees.size())
			{
				throw std::out_of_range("Index out of bounds");
			}
			return degrees[node_id];
		}

		/**
		 * \brief Getter for the weight of the edge between two given vertices.
		 *
		 * \note A return value of zero means there is no edge between given vertices.
		 *
		 * \warning Exceptions to guard against:
		 *		- std::out_of_range - One of the given indexes is out of range.
		 *
		 * \param source ID of the beginning vertex of the edge.
		 * \param destination ID of the end vertex of the edge.
		 * \return Weight of the connection.
		 */
		[[nodiscard]] auto get_edge(std::size_t source, std::size_t destination) const -> int32_t
		{
			int32_t ret = 0;
			if (source >= list.size() || destination >= list.size())
			{
				throw std::out_of_range("Index out of bounds");
			}
			for (const auto& neighbour : list[source])
			{
				if (neighbour.ID == destination)
				{
					ret = neighbour.weight;
					break;
				}
			}
			return ret;
		}

		[[nodiscard]] auto get_type() const noexcept -> Type
		{
			return type;
		}

		auto save_graphml(std::ostream& stream, std::string name) const noexcept -> void;

		[[nodiscard]] auto change_to_line_graph() const -> List;

	private:

		/* Load functions for specific file formats */
		auto load_lst_file(std::istream& file) noexcept -> void;
		auto load_graphml_file(std::istream& file) -> void;
		auto calculate_degrees() noexcept -> void;

		/* Objects containing the graph information */

		std::vector<std::vector<Node>> list; /**< List structure of the graph. Consists of the std::vector 
												containing std::lists of neighbour vertices for a given vertex. */
		std::vector<Degree> degrees;	   /**< Vector containing degrees of each vertex. Degrees are stored within the Graph::Degree
												structure, and the type field decides which fields of the structure should be used. */
		Type type;						   /**< Enum signifying the type of the graph */
	};
}