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

		/**
		 * Constructor accepting the coordinates.
		 *
		 * Allows user to use the structure as an initializer list while passing
		 * as a parameter to a function.
		 *
		 * \param x X coordinate.
		 * \param y Y coordinate.
		 */
		Coord(std::size_t x, std::size_t y) noexcept
			: x(x),
			y(y)
		{}

		// getters

		auto [[nodiscard]] row() const noexcept -> std::size_t const&
		{
			return x;
		}

		auto [[nodiscard]] row() noexcept -> std::size_t&
		{
			return x;
		}

		auto [[nodiscard]] col() const noexcept -> std::size_t const&
		{
			return y;
		}

		auto [[nodiscard]] col() noexcept -> std::size_t&
		{
			return y;
		}
	};

	auto [[nodiscard]] find_index(const std::vector<Coord>& nodes, const Coord& _coord) noexcept -> std::size_t;
}