#pragma once

#include <cstdint>
#include <vector>

namespace Data
{
	/**
	* 
	 * \brief Structure containing coordinates of the vertex or edge.
	 * 
	 * \note If the structure stores an edge coordinates, the first coordinate states
	 * the starting vertex, while the second coordinate states the ending
	 * vertex.
	 */
	struct Coord
	{ 
		std::size_t x; /**< x coordinate of a vertex / starting vertex of an edge */
		std::size_t y; /**< y coordinate of a vertex / ending vertex of an edge */

		Coord() = default;
		Coord(std::size_t x, std::size_t y);

		auto row() const -> std::size_t const&
		{
			return x;
		}

		auto row() -> std::size_t&
		{
			return x;
		}

		auto col() const -> std::size_t const&
		{
			return y;
		}

		auto col() -> std::size_t&
		{
			return y;
		}
	};

	auto find_index(const std::vector<Coord>& nodes, const Coord& _coord) -> std::size_t;
}