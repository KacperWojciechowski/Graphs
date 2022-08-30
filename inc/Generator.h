#pragma once

#include <string>
#include <fstream>

#include "../inc/Graph.h"

namespace Data
{
	struct Limits
	{
		std::size_t min; /**< An upper limit of given interval */
		std::size_t max; /**< A lower limit of given interval */
	};

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
		void make_matrix(std::ostream& stream, uint32_t vertex_amount, const Limits& limits, Graph::Type type, std::uint8_t density_psc = 1);
		void make_throughtput_matrix(std::ostream& stream, std::ifstream& matrix_src, const Limits& limits, Graph::Type type);

		// list graph generation
		void make_list(std::ostream& stream, std::size_t vertex_amount, Graph::Type type, std::uint8_t density_psc = 1);

		// pixel map generationd
		void make_pixel_map(std::ostream& stream, uint32_t length, uint32_t width);
	};
}