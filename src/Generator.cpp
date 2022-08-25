#include "../inc/Generator.h"
#include "../inc/Coord.h"

#include <time.h>
#include <iostream>
#include <vector>

/**
 * Function generating a random graph in a list representation, of a given type.
 * 
 * \param stream Output stream to save generated list to
 * \param vertex_amount Desired amount of vertices within the graph
 * \param type Type of the graph from Graph::Type enum
 */
void Data::Generator::make_list(std::ostream& stream, uint32_t vertex_amount, Graph::Type type)
{
	std::vector<Data::coord> edges;

	if (type == Graph::Type::undefined)
	{
		std::cerr << "Please select either directed or undirected graph type\n";
	}
	else
	{

	}
}




/**
 * \brief Function generating a random pixel map within given stream, with given length and width.
 * 
 * Pixel map is represented by a matrix of binary values, where 0 represents a corridor, and 1 represents
 * a wall. This map can be used with List class to create a graph of connected corridors, which can be later
 * used with a path-searching algorithm to find the shortest paths from given place to all other places.
 * 
 * \param stream Output stream to save generated pixel map in.
 * \param length Vertical length of the map.
 * \param width Horizontal width of the map.
 */
void Data::Generator::make_pixel_map(std::ostream& stream, uint32_t length, uint32_t width)
{
	srand(static_cast<unsigned int>(time(NULL)));

	uint32_t sector;

	// generate subsequent pixels in each row
	for (uint32_t i = 0; i < length; i++)
	{
		// iterate through row positions
		for (uint32_t j = 0; j < width; j++)
		{
			sector = rand() % 2;
			stream << sector;
			if (j < width - 1)
			{
				stream << ' ';
			}
		}

		if (i < length - 1)
		{
			stream << '\n';
		}
	}
}
