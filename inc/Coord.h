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
		uint32_t x;
		uint32_t y;
	};

	uint32_t find_index(std::vector<coord>& nodes, coord _coord);
}