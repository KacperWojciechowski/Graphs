#pragma once

#include <cstdint>
#include <vector>

namespace Data
{
	/**
	* 
	 * \brief Structure containing coordinates of the vertex or edge.
	 * 
	 * \note If the structure stores an edge coordinates, the first coordinate states
	 * the starting vertex, while the second coordinate states the ending
	 * vertex.
	 */
	struct coord
	{
		std::size_t x; /**< x coordinate of a vertex / starting vertex of an edge */
		std::size_t y; /**< y coordinate of a vertex / ending vertex of an edge */

		std::size_t& row = x; /**< alias for the row coordinate of a matrix structure */
		std::size_t& col = y; /**< alias for the column coordinate of a matrix structure */
	};

	std::size_t find_index(std::vector<coord>& nodes, coord _coord);
}