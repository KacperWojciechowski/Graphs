#include "Algorithms.h"

#include <algorithm>
#include <iostream>


/**
 * \brief Function displaying the data stored within the Roadmap class.
 *
 * \note In order to follow the full path, the user needs to recreate the path
 *		 based on the stored data.
 */
auto Graph::Roadmap::print() const noexcept -> void
{
	for (std::size_t index = 0; const auto& element : distances)
	{
		// print distance
		std::cout << "Distance: " << element << std::endl;
		std::cout << "Prev node: ";

		// print previous node info
		if (prev_node[index].empty())
		{
			std::cout << "None";
		}
		else
		{
			for (std::size_t index2 = 0; const auto& vertex : prev_node[index])
			{
				std::cout << vertex;
				if (index2 < prev_node[index].size() - 1)
				{
					std::cout << " / ";
				}
				index2++;
			}
		}
		index++;

		// go to new line
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
[[nodiscard]] auto Graph::Roadmap::paths(std::size_t end) const noexcept -> std::vector<Path>
{
	std::vector<Path> ret;
	std::vector<std::size_t> path;

	path_search(ret, path, throughtputs[end], distances[end], end);

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
auto Graph::Roadmap::path_search(std::vector<Path>& paths, std::vector<std::size_t>& path, int32_t const& thr, int32_t const& distance, std::size_t v) const noexcept -> void
{
	path.emplace_back(v);

	if (prev_node[v].empty())
	{
		paths.emplace_back(path, distance, thr);
		path.pop_back();
	}
	else
	{
		for (auto& vertex : prev_node[v])
		{
			path_search(paths, path, thr, distance, vertex);
		}
		path.pop_back();
	}
}




/**
 * Output stream operator for the Path.
 */
auto operator << (std::ostream& stream, const Graph::Path& p) noexcept -> std::ostream&
{
	stream << "Distance: " << p.distance << '\n';
	stream << "Throughtput: " << p.throughtput << '\n';

	for (std::size_t i = 0; const auto & vertex : p.path)
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
