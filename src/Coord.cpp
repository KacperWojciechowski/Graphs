#include "../inc/Coord.h"

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
std::size_t Data::find_index(std::vector<Data::coord>& nodes, Data::coord _coord)
{
	size_t size = nodes.size();

	// set the default value
	std::size_t ret = std::numeric_limits<std::size_t>::max();

	// search for the index
	for (auto itr = nodes.begin(); itr != nodes.end(); itr++)
	{
		if (itr->x == _coord.x && itr->y == _coord.y)
		{
			ret = std::distance(nodes.begin(), itr);
			break;
		}
	}

	// return found index
	return ret;
}