#pragma once

#include <string>

#include "../inc/Graph.h"

namespace Data
{
	/**
	 * \brief Class responsible for generating various data sources supported by this library.
	 * 
	 * This class offers to generate such data sources as random weighted adjacency matrix,
	 * random adjacency list, random throughtput matrix, and random pixel map. All generated 
	 * data is saved within given output stream (such as a file output stream).
	 */
	class Generator
	{
	public:
		// matrix graph generation
		void make_weighted_matrix(std::ostream& stream, uint32_t vertex_amount, Graph::Type type);
		void make_throughtput_matrix(std::ostream& stream, std::string output_file_path, Graph::Type type);

		// list graph generation
		void make_list(std::ostream& stream, std::size_t vertex_amount, Graph::Type type);

		// pixel map generationd
		void make_pixel_map(std::ostream& stream, uint32_t length, uint32_t width);
	};
}