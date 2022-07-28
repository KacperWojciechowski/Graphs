#pragma once

#include <cstdint>
#include <vector>

namespace Data
{
	/*
		Structure of coordinates of each field on the pixel map.
	*/
	struct coord
	{
		std::size_t x;
		std::size_t y;
	};

	std::size_t find_index(std::vector<coord>& nodes, coord _coord);
}