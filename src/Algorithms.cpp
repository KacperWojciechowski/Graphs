#include "../inc/Algorithms.h"

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
