#pragma once

#include <cstdint>
#include <vector>

namespace Data
{
	/**
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
	};

	/**
	* \brief Function finding the index of given coordinate pair in coordinates vector.
	*
	* Can be used to find the node index during creation of a line graph, or creation
	 * of a graph based on pixel map.
	*
	* \note In case the vector does not contain the given coord structure, the
	* maximum numeric limit of std::size_t is returned.
	*
	* \param nodes reference to the vector of coordinates
	 * \param _coord searched coordinates structure
	 * \return index of the coord structure within the coordinates vector.
	*/
	std::size_t find_index(std::vector<coord>& nodes, coord _coord);
}