#include "../inc/Algorithms.h"

#include <iostream>

/**
 * \brief Function displaying the data stored within the Roadmap class.
 * 
 * \note In order to follow the full path, the user needs to recreate the path
 *		 based on the stored data.
 */
void Graph::Roadmap::print()
{
	for (auto itr = this->distance.begin(); itr != this->distance.end(); itr++)
	{
		std::cout << "Distance: " << *itr << std::endl;
		std::cout << "Prev node: ";

		std::size_t index = std::distance(this->distance.begin(), itr);

		for (auto itr2 = this->prev_node[index].begin(); itr2 != this->prev_node[index].end(); itr2++)
		{
			std::cout << *itr2;
			if (std::distance(this->prev_node[index].begin(), itr2) < this->prev_node[index].size() - 1)
			{
				std::cout << " / ";
			}
		}
		std::cout << std::endl;
	}
}
