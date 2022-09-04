#include "../inc/Algorithms.h"

#include <algorithm>
#include <iostream>

/**
 * Constructor creating internal vectors of given size.
 * 
 * \param vertex_count Amount of vertices in the graph structure.
 */
Graph::Roadmap::Roadmap(std::size_t vertex_count)
	: prev_node(vertex_count),
	distances(vertex_count, std::numeric_limits<int32_t>::max()),
	throughtputs(vertex_count, std::numeric_limits<int32_t>::max()),
	start(0)
{
}




/**
 * \brief Function displaying the data stored within the Roadmap class.
 * 
 * \note In order to follow the full path, the user needs to recreate the path
 *		 based on the stored data.
 */
auto Graph::Roadmap::print() const -> void
{
	for (std::size_t index = 0; auto element : this->distances)
	{
		// print distance
		std::cout << "Distance: " << element << std::endl;
		std::cout << "Prev node: ";

		// print previous node info
		if (this->prev_node[index].empty())
		{
			std::cout << "None";
		}
		else
		{
			for (std::size_t index2 = 0; auto vertex : this->prev_node[index])
			{
				std::cout << vertex;
				if (index2 < this->prev_node[index].size() - 1)
				{
					std::cout << " / ";
				}
				index2++;
			}
		}
		index++;
		
		// flush the stream and go to new line
		std::cout << '\n';
	}
	std::cout << std::flush;
}




/**
 * Function extracting a path vector from internally saved starting vertex, to a selected destination
 * vertex from a roadmap.
 * 
 * This function returns a vector of Paths due to a fact, that in the graph structure there
 * may exist several paths of equal length. The function does not verify whether they 
 * vary in throughtput.
 * 
 * \param end Destination vertex of the path.
 * \return
 */
auto Graph::Roadmap::paths(std::size_t end) const -> std::vector<Path>
{
	std::vector<Path> ret;
	std::vector<std::size_t> path;

	this->path_search(ret, path, this->throughtputs[end], this->distances[end], end);

	for (auto& log : ret)
	{
		std::reverse(log.path.begin(), log.path.end());
	}
	return ret;
}




/**
 * Recursive function for path building.
 * 
 * \param paths Reference to a vector containing all the found paths.
 * \param path Reference to a vector containing currently explored path.
 * \param thr Throughtput of the currently explored path.
 * \param distance Distance shared by all the found paths.
 * \param v Currently explored vertex.
 * \return 
 */
auto Graph::Roadmap::path_search(std::vector<Path>& paths, std::vector<std::size_t>& path, int32_t const& thr, int32_t const& distance, std::size_t v) const -> void
{
	path.emplace_back(v);

	if (this->prev_node[v].empty())
	{
		paths.emplace_back(path, distance, thr);
		path.pop_back();
	}
	else
	{
		for (auto& vertex : this->prev_node[v])
		{
			path_search(paths, path, thr, distance, vertex);
		}
		path.pop_back();
	}
}




/**
 * Constructor saving the Path information.
 * 
 * \param path Vector containing the indexes of each subsequent vertex in a path, starting
 *			   with the source vertex and ending with a destination vertex.
 * \param distance Summed distance from the source vertex to the destination vertex.
 * \param throughtput The throughtput of the path, which is the minimal throughtput of
 *					  all subsequent stretch.
 */
Graph::Path::Path(std::vector<std::size_t> path, uint32_t distance, uint32_t throughtput)
	: path(path),
	distance(distance),
	throughtput(throughtput)
{
}




/**
 * Output stream operator for the Path.
 */
auto Graph::operator<<(std::ostream& stream, const Path& p) -> std::ostream&
{
	stream << "Distance: " << p.distance << '\n';
	stream << "Throughtput: " << p.throughtput << '\n';

	for (std::size_t i = 0; auto& vertex : p.path)
	{
		stream << vertex;;
		if (i < p.path.size() - 1)
		{
			stream << " -> ";
		}
		i++;
	}
	stream << '\n';
	stream << std::flush;

	return stream;
}




/**
 * Constructor creating sufficiently big vector for colors assignment.
 * 
 * \param color_count Default amount of colors used.
 * \param vertex_count Amount of the vertices.
 */
Graph::Coloring::Coloring(int32_t color_count, std::size_t vertex_count)
	: color_count(color_count),
	color(vertex_count, -1)
{
}
