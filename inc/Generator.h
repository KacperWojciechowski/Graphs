#pragma once

#include <string>

namespace Data
{
	/**
	 * \brief Class responsible for generating various data sources supported by this library.
	 * 
	 * This class offers to generate such data sources as random weighted adjacency matrix,
	 * random adjacency list, random throughtput matrix, and random pixel map. All generated 
	 * data is saved in form of plain-text files with various extensions.
	 */
	class Generator
	{
	public:
		// matrix graph generation
		void make_weighted_matrix(std::string output_file_path, uint32_t vertex_amount);
		void make_throughtput_matrix(std::string graph_file_path, std::string output_file_path);

		// list graph generation
		void make_list(std::string output_file_path, uint32_t vertex_amount);

		// pixel map generationd
		void make_pixel_map(std::string output_file_path, uint32_t length, uint32_t width);
	};
}