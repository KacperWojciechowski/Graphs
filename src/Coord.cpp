#include "../inc/Coord.h"

/*
	Function finding the index of given coordinate pair in coordinates vector. Can
	be used to find the node index during creation of a line graph, or creation of
	a graph based on pixel map. 
	
	If searched coordinates were not found, uint32_t upper range limit is returned;

	Params:
	nodes	- reference to the vector of coordinates'
	_coord	- structure containing the searched coordinates 

	Return:
	return  - the index of searched coordinates in given vector. If not found,
			  upper numeric limit of unsigned int is returned.
*/

uint32_t Data::find_index(std::vector<Data::coord>& nodes, Data::coord _coord)
{
	size_t size = nodes.size();

	uint32_t ret = std::numeric_limits<uint32_t>::max();

	for (auto itr = nodes.begin(); itr != nodes.end(); itr++)
	{
		if (itr->x == _coord.x && itr->y == _coord.y)
		{
			ret = std::distance(nodes.begin(), itr);
			break;
		}
	}

	return ret;
}