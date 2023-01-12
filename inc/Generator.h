#pragma once

#include <string>
#include <fstream>

#include "IGraph.h"

namespace Data
{
	struct Limits
	{
		std::int32_t min; /**< An upper limit of given interval */
		std::int32_t max; /**< A lower limit of given interval */
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
		auto make_matrix(std::ostream& stream, uint32_t vertex_amount, const Limits& limits, Graph::Type type, std::uint8_t density_psc = 1) const noexcept -> void;
		auto make_throughtput_matrix(std::ostream& stream, std::ifstream& matrix_src, const Limits& limits, Graph::Type type) const -> void;

		// list graph generation
		auto make_list(std::ostream& stream, std::size_t vertex_amount, Graph::Type type, std::uint8_t density_psc = 1) const noexcept -> void;

		// pixel map generationd
		auto make_pixel_map(std::ostream& stream, uint32_t length, uint32_t width) const noexcept -> void;
	};
}