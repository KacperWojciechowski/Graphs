#include "../inc/Generator.h"
#include "../inc/Coord.h"

#include <time.h>
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Function generating a random graph in a list representation, of a given type. 
 * 
 * The generated format is compliant with .lst format provided by House of Graphs, and
 * as such, uses vertex indexes starting with 1. Depending of the graph type, this function
 * generates either undirected or directed connections. This function generates mostly
 * dense graphs.
 * 
 * \param stream Output stream to save generated list to
 * \param vertex_amount Desired amount of vertices within the graph
 * \param type Type of the graph from Graph::Type enum
 * 
 * \ref list_generator.cpp "Example of generating a randomized list"
 */
void Data::Generator::make_list(std::ostream& stream, std::size_t vertex_amount, Graph::Type type)
{
	std::vector<std::vector<std::size_t>> list;
	std::size_t count;
	std::size_t val;

	srand(static_cast<uint32_t>(time(NULL)));

	// generate empty list
	for (std::size_t i = 0; i < vertex_amount; i++)
	{
		list.push_back({});
	}
	
	// generate row of the data source file
	for (std::size_t i = 0; i < vertex_amount; i++)
	{
		count = rand() % vertex_amount + 1;

		// generate indexes of each adjacent vertex
		for (std::size_t j = 0; j < count; j++)
		{
			val = rand() % vertex_amount;
			
			// prevent duplicates from appearing
			if (std::find(list[i].begin(), list[i].end(), val) == list[i].end())
			{
				list[i].push_back(val);

				if (i != val && type == Graph::Type::undirected)
				{
					list[val].push_back(i);
				}
			}
		}
	}

	// sort the list
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		std::sort(itr->begin(), itr->end());
	}

	// save the structure
	for (std::size_t index = 0; auto row : list)
	{
		stream << index + 1 << ": ";

		for (std::size_t index2 = 0; auto element : row)
		{
			stream << element + 1;

			if (index2 < row.size() - 1)
			{
				stream << ' ';
			}
			index2++;
		}
		stream << '\n';
		index++;
	}
	stream << std::flush;
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
 * 
 * \ref pixel_map_generator.cpp "Example of generating the pixel map data source"
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
	stream << std::flush;
}
