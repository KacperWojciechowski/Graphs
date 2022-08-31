#pragma once

#include <cstdint>
#include <vector>
#include <iostream>

namespace Data
{
	/**
	 * \brief Class representing a map of binary pixels.
	 * 
	 * Used to create an adjacency list representation of a graph of pathways between adjacent pixels.
	 * 
	 * \attention Zeros indicate the fields while ones indicate the walls.
	 */
	class PixelMap
	{
	public:

		// constructors
		PixelMap(std::istream& stream);

		PixelMap(PixelMap& p);
		PixelMap(PixelMap&& p);

		// interface
		void print();

		size_t get_rows();
		size_t get_columns();
		uint8_t get_field(uint32_t x, uint32_t y);

	private:
		std::vector<std::vector<uint8_t>> map; /**< Structure containing the map. Consists of std::vector of std::vector objects. Each
													field within the map is stored as an unsigned char. */
	};
}