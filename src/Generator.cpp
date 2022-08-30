#include "../inc/Generator.h"
#include "../inc/Coord.h"

#include <time.h>
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Function generating a random graph of given type in a matrix representation.
 * 
 * The generated format is compliant with .mat format provided by House of Graphs. Each
 * number in the data file structure represents a weight of an edge. The value of zero
 * represents no edge between two vertices. Undirected graphs will result in being represented
 * as a symmetric matrix, with the diagonal as a symmetry axis. This function generates graphs
 * only with positive weights.
 * 
 * \note Limits are expressed as a left open interval (limits.min; limits.max>.
 * \note Density prescaller does not guarantee any specific density, but manipulate the chance of generating an edge for each field.
 *		 The chance of generating an edge when using a prescaller is expressed as 1/x * (max - min) / (max - min + 1),
 *		 where x denotes the prescaller value, max denotes the upper weight limit, and min denotes the lower weight limit.
 * 
 * \param stream Output stream to save generated matrix to.
 * \param vertex_amount Desired amount of vertices within the graph.
 * \param limits Upper and lower limits for the weight values.
 * \param type Type of the graph from Graph::Type enum.
 * \param density_psc Prescaller for graph density manipulation. Allows user to decrease the chance 
 *					  an edge will be generated, by increasing the prescaller. Assumes value of 1 by default,
 *					  which does not cause any modification.
 * 
 * \ref matrix_generator.cpp "Example of generating a randomized matrix"
 */
void Data::Generator::make_matrix(std::ostream& stream, uint32_t vertex_amount, const Limits& limits, Graph::Type type, std::uint8_t density_psc)
{
	std::vector<std::vector<std::size_t>> matrix;
	std::size_t val;

	srand(static_cast<uint32_t>(time(NULL)));

	// calculate the divider for modulo division
	std::size_t divider = limits.max - limits.min + 1;

	// create an empty matrix
	for (std::size_t i = 0; i < vertex_amount; i++)
	{
		matrix.push_back({});
		for (std::size_t j = 0; j < vertex_amount; j++)
		{
			matrix[i].push_back(0);
		}
	}

	// fill matrix with random weights
	if (type == Graph::Type::directed)
	{
		for (std::size_t i = 0; i < vertex_amount; i++)
		{
			for (std::size_t j = 0; j < vertex_amount; j++)
			{
				val = rand() % density_psc == 0 ? rand() % divider : 0;
				if (val != 0)
				{
					val += limits.min;
				}
				matrix[i][j] = val;
			}
		}
	}
	else
	{
		for (std::size_t i = 0; i < vertex_amount; i++)
		{
			for (std::size_t j = i; j < vertex_amount; j++)
			{
				val = rand() % density_psc == 0 ? rand() % divider : 0;
				if (val != 0)
				{
					val += limits.min;
				}
				matrix[i][j] = val;
				matrix[j][i] = val;
			}
		}
	}

	// output the resulting matrix to the stream
	for (std::size_t index1 = 0; auto& row : matrix)
	{
		for (std::size_t index2 = 0; auto& element : row)
		{
			stream << element;
			if (index2 < row.size() - 1)
			{
				stream << ' ';
				index2++;
			}
		}
		if (index1 < matrix.size() - 1)
		{
			stream << '\n';
		}
	}
	stream << std::flush;
}




/**
 * Function generating a random graph in a list of given type representation. 
 * 
 * The generated format is compliant with .lst format provided by House of Graphs, and
 * as such, uses vertex indexes starting with 1. Depending of the graph type, this function
 * generates either undirected or directed connections. This function generates mostly
 * dense graphs.
 * 
 * \note Density prescaller does not guarantee any specific density, but manipulate the chance of generating an edge for each field.
 *		 The chance of generating an edge when using a prescaller is expressed as 1/x * (va - 1) / va,
 *		 where x denotes the prescaller value, and va denotes the vertices amount.
 * 
 * \param stream Output stream to save generated list to
 * \param vertex_amount Desired amount of vertices within the graph
 * \param type Type of the graph from Graph::Type enum
 * \param density_psc Prescaller for graph density manipulation. Allows user to decrease the chance 
 *					  an edge will be generated, by increasing the prescaller. Assumes value of 1 by default,
 *					  which does not cause any modification.
 * 
 * \ref list_generator.cpp "Example of generating a randomized list"
 */
void Data::Generator::make_list(std::ostream& stream, std::size_t vertex_amount, Graph::Type type, std::uint8_t density_psc)
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
			val = rand() % density_psc == 0 ? rand() % vertex_amount : 0;
			
			// prevent duplicates from appearing
			if (std::find(list[i].begin(), list[i].end(), val) == list[i].end() && val != 0)
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
