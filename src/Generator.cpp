#include "../inc/Generator.h"

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
 * \note Density prescaler does not guarantee any specific density, but manipulate the chance of generating an edge for each field.
 *		 The chance of generating an edge when using a prescaler is expressed as 1/x * (max - min) / (max - min + 1),
 *		 where x denotes the prescaler value, max denotes the upper weight limit, and min denotes the lower weight limit.
 * 
 * \param stream Output stream to save generated matrix to.
 * \param vertex_amount Desired amount of vertices within the graph.
 * \param limits Upper and lower limits for the weight values.
 * \param type Type of the graph from Graph::Type enum.
 * \param density_psc Prescaler for graph density manipulation. Allows user to decrease the chance 
 *					  an edge will be generated, by increasing the prescaler. Assumes value of 1 by default,
 *					  which does not cause any modification.
 * 
 * \ref matrix_generator.cpp "Example of generating a randomized matrix"
 */
auto Data::Generator::make_matrix(std::ostream& stream, uint32_t vertex_amount, const Limits& limits, Graph::Type type, std::uint8_t density_psc) const -> void
{
	std::vector<std::vector<std::size_t>> matrix;
	std::size_t val;

	srand(static_cast<uint32_t>(time(NULL)));

	// calculate the divider for modulo division
	std::size_t divider = limits.max - limits.min + 1;

	// create an empty matrix
	for (std::size_t i = 0; i < vertex_amount; i++)
	{
		matrix.emplace_back(vertex_amount);
	}

	// fill matrix with random weights
	if (type == Graph::Type::directed)
	{
		for (auto& row : matrix)
		{
			for (auto& element : row)
			{
				val = rand() % density_psc == 0 ? rand() % divider : 0;
				if (val != 0)
				{
					val += limits.min;
				}
				element = val;
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
 * Function generating a random throughtput matrix based on an existing adjacency matrix.
 * 
 * The function first reads the input file for adjacency matrix, to later analyze it and create throughtput
 * values where there are edges in the original adjacency matrix.
 * 
 * \note Limits are expressed as a left open interval (limits.min; limits.max>.
 * 
 * \warning The source of the adjacency matrix must be an input file stream. The function does not support imput
 *			from standard input, however it does support outputing the result to the standard output.
 * \warning The function does not verify whether source matrix graph type and graph type provided by the user match.
 *			In case of missmatching the source matrix type, function will result in incorrect throughtput matrix generation.
 * 
 * \warning Exception to guard against:
 *			- std::runtime_error - The adjacency matrix file contains negative weights, or the matrix is not square.
 * 
 * \warning The matrix must consist of only integer values. Otherwise, this funcion might result in an undefined behaviour.
 * 
 * \param stream Output stream to save generated list to
 * \param matrix_src Input stream where to read the adjacency matrix from
 * \param limits Upper and lower limits for throughtput values
 * \param type Type of the source graph
 * 
 * \ref thr_matrix_generator.cpp "Example of generating a throughtput matrix"
 */
auto Data::Generator::make_throughtput_matrix(std::ostream& stream, std::ifstream& matrix_src, const Limits& limits, Graph::Type type) const -> void
{
	std::vector<std::vector<int32_t>> src_matrix;
	std::vector<std::vector<int32_t>> dest_matrix;
	
	srand(static_cast<uint32_t>(time(NULL)));

	// calculate the divider for modulo division
	std::int32_t divider = limits.max - limits.min + 1;
	
	std::string line;
	std::size_t pos;

	std::size_t index = 0;

	// lambda function extracting the values from read row of the source matrix
	auto insert_val = [&line, &pos, &src_matrix](std::size_t index) -> void {

		std::int32_t val = std::stoi(line);
		pos = line.find(' ');
		
		if (pos != std::string::npos)
		{
			line = line.substr(pos + 1);
		}
		
		if (val < 0)
		{
			throw std::runtime_error("Weight less than zero");
		}

		src_matrix[index].emplace_back(val);
	};

	// parse the source matrix file
	while (std::getline(matrix_src, line))
	{
		if (line != "")
		{
			src_matrix.emplace_back(0);

			pos = 0;

			while (pos != std::string::npos)
			{
				insert_val(index);
			}
			insert_val(index);
			index++;
		}
	}

	// verify whether the source matrix is square and create destination matrix
	for (std::size_t index = 0; auto& row : src_matrix)
	{
		if (src_matrix.size() != row.size())
		{
			throw std::runtime_error("The source matrix is not square");
		}
		else
		{
			dest_matrix.emplace_back(row.size());
			index++;
		}
	}

	std::int32_t val;
	std::size_t size = src_matrix.size();

	// create the destination matrix
	if (type == Graph::Type::directed)
	{
		for (std::size_t i = 0; i < size; i++)
		{
			for (std::size_t j = 0; j < size; j++)
			{
				if (src_matrix[i][j] != 0)
				{
					dest_matrix[i][j] = rand() % divider + limits.min;
				}
			}
		}
	}
	else
	{
		for (std::size_t i = 0; i < size; i++)
		{
			for (std::size_t j = i; j < size; j++)
			{
				if (src_matrix[i][j] != 0)
				{
					val = rand() % divider + limits.min;
					dest_matrix[i][j] = val;
					dest_matrix[j][i] = val;
				}
			}
		}
	}

	// output the created throughtput matrix
	for (std::size_t index1 = 0; auto& row : dest_matrix)
	{
		for (std::size_t index2 = 0; auto& element : row)
		{
			stream << element;
			if (index2 < row.size() - 1)
			{
				stream << ' ';
			}
			index2++;
		}
		if (index1 < dest_matrix.size() - 1)
		{
			stream << '\n';
		}
		index1++;
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
 * \note Density prescaler does not guarantee any specific density, but manipulate the chance of generating an edge for each field.
 *		 The chance of generating an edge when using a prescaler is expressed as 1/x * (va - 1) / va,
 *		 where x denotes the prescaler value, and va denotes the vertices amount.
 * 
 * \param stream Output stream to save generated list to
 * \param vertex_amount Desired amount of vertices within the graph
 * \param type Type of the graph from Graph::Type enum
 * \param density_psc Prescaler for graph density manipulation. Allows user to decrease the chance 
 *					  an edge will be generated, by increasing the prescaler. Assumes value of 1 by default,
 *					  which does not cause any modification.
 * 
 * \ref list_generator.cpp "Example of generating a randomized list"
 */
auto Data::Generator::make_list(std::ostream& stream, std::size_t vertex_amount, Graph::Type type, std::uint8_t density_psc) const -> void
{
	std::vector<std::vector<std::size_t>> list;
	std::size_t count;
	std::size_t val;

	srand(static_cast<uint32_t>(time(NULL)));

	// generate empty list
	for (std::size_t i = 0; i < vertex_amount; i++)
	{
		list.emplace_back(0);
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
				list[i].emplace_back(val);

				if (i != val && type == Graph::Type::undirected)
				{
					list[val].emplace_back(i);
				}
			}
		}
	}

	// sort the list
	for (auto& row : list)
	{
		std::sort(row.begin(), row.end());
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
auto Data::Generator::make_pixel_map(std::ostream& stream, uint32_t length, uint32_t width) const -> void
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
