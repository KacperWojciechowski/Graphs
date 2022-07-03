#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace Data
{
	/*
		Class representing a map of binary pixels. Zeros indicate the fields,
		while ones indicate the walls.

		Used to create a graph of pathways between adjacent pixels.
	*/
	class Pixel_map
	{
	public:

		// constructors
		Pixel_map(std::string file_path);

		Pixel_map(Pixel_map& p) = delete;
		Pixel_map(Pixel_map&& p) = delete;

		// interface
		void print();

		size_t get_rows();
		size_t get_columns();
		uint8_t get_field(uint32_t x, uint32_t y);

	private:
		std::vector<std::vector<uint8_t>> map;
	};
}