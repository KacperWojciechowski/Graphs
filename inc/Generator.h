#pragma once

#include <string>

namespace Data
{
	/*
		Class responsible for generating various data sources such as matrix graphs, throughtput
		graphs and pixel maps. All generated data is saved in form of plain-text files with various
		extensions.
	*/
	class Generator
	{
	public:

		// graph generation
		void make_weighted_graph(std::string output_file_path, uint32_t nodes_amount);
		void make_throughtput(std::string graph_file_path, std::string output_file_path);

		// pixel map generation
		void make_pixel_map(std::string output_file_path, uint32_t length, uint32_t width);
	};
}