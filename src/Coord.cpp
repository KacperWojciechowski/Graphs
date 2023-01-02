#include "Coord.h"

#include <limits>

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
auto Data::find_index(const std::vector<Data::Coord>& nodes, const Data::Coord& coord) noexcept -> std::size_t
{
	std::size_t size = nodes.size();

	// set the default value
	std::size_t ret = std::numeric_limits<std::size_t>::max();

	// search for the index
	for (std::size_t index = 0; const auto& element : nodes)
	{
		if (element.x == coord.x && element.y == coord.y)
		{
			ret = index;
			break;
		}
		index++;
	}

	// return found index
	return ret;
}
