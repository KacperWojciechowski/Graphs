#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace Data
{
	void randomize_map(uint32_t length, uint32_t width);

	struct coord
	{
		uint32_t x;
		uint32_t y;
	};

	class Pixel_map
	{
	public:
		Pixel_map(std::string file_path);
		Pixel_map(Pixel_map& p) = delete;
		Pixel_map(Pixel_map&& p) = delete;

		void print_area_map();


		uint32_t get_rows();
		uint32_t get_columns();

		uint32_t get_field(uint32_t x, uint32_t y);

		~Pixel_map();

	private:
		uint8_t** area_map;

		uint32_t rows;
		uint32_t columns;
	};
} // Data