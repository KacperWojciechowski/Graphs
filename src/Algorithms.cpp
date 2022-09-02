#include "../inc/Algorithms.h"

#include <algorithm>
#include <iostream>

/**
 * \brief Function displaying the data stored within the Roadmap class.
 * 
 * \note In order to follow the full path, the user needs to recreate the path
 *		 based on the stored data.
 */
auto Graph::Roadmap::print() const -> void
{
	for (std::size_t index = 0; auto element : this->distance)
	{
		// print distance
		std::cout << "Distance: " << element << std::endl;
		std::cout << "Prev node: ";

		// print previous node info
		for (std::size_t index2 = 0; auto vertex : this->prev_node[index])
		{
			std::cout << vertex;
			if (index2 < this->prev_node[index].size() - 1)
			{
				std::cout << " / ";
			}
			index2++;
		}
		index++;
		
		// flush the stream and go to new line
		std::cout << std::endl;
	}
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
auto Graph::Roadmap::path(std::size_t end) const -> std::vector<Path>
{

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
auto Graph::operator<<(std::ostream& stream, Path&& p) -> std::ostream&
{
	stream << "Distance: " << p.distance << '\n';
	stream << "Throughtput: " << p.throughtput << '\n';

	for (auto& vertex : p.path)
	{
		stream << vertex << " -> ";
	}
	stream << '\n';
	stream << std::flush;
}

