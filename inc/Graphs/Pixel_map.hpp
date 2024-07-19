#pragma once

// libraries
#include <cstdint>
#include <string>
#include <vector>

namespace Data
{
	/* Custom data type */
	struct coord
	{
		uint32_t x;
		uint32_t y;
	};

	/* Function for creating a randomized pixel map */
	void randomize_map(std::string file_path, uint32_t length, uint32_t width);

	/* Function for searching the index of given pixel map node */
	uint32_t find_index(const std::vector<Data::coord>& nodes, uint32_t x, uint32_t y);

	/*
		Class representing a map of binary pixels.
		Used to create a graph of pathways between adjacent pixels.
	*/
	class Pixel_map
	{
	public:
		/* Constructors */
		Pixel_map(std::string file_path);
		Pixel_map(Pixel_map& p) = delete;
		Pixel_map(Pixel_map&& p) = delete;

		/* Interface */
		void print_area_map();

		uint32_t get_rows() const;
		uint32_t get_columns() const;
		uint32_t get_field(uint32_t x, uint32_t y) const;

		/* Destructor */
		~Pixel_map();

	private:
		/* Private variables */
		uint8_t** area_map;

		uint32_t rows;
		uint32_t columns;
	};
} // Data