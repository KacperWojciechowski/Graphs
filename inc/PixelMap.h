#pragma once

#include "Coord.h"

#include <cstdint>
#include <vector>
#include <iostream>

namespace Data
{
	/**
	 * \brief Class representing a map of binary pixels.
	 * 
	 * Used to create an adjacency list representation of a graph of pathways between adjacent pixels.
	 * 
	 * \attention Zeros indicate the fields while ones indicate the walls.
	 */
	class PixelMap
	{
	public:
		static constexpr uint8_t GAP = 0; /**< A non-wall field within the pixel map structure */
		static constexpr uint8_t WALL = 1; /**< A wall field within the pixel map structure */

		// constructors
		PixelMap(std::istream& stream) noexcept;

		PixelMap(const PixelMap& p) noexcept;
		PixelMap(PixelMap&& p) noexcept;

		// interface
		auto print() const -> void;

		// getters

		auto [[nodiscard]] get_rows() const noexcept -> std::size_t
		{
			return map.size();
		}

		auto [[nodiscard]] get_columns() const -> std::size_t
		{
			return map[0].size();
		}

		auto [[nodiscard]] get_field(const Data::Coord& coord) const -> uint8_t
		{
			if (coord.x >= map.size() || coord.y >= map[0].size())
			{
				throw std::out_of_range("Index out of range");
			}
			return map[coord.x][coord.y];
		}

	private:
		std::vector<std::vector<uint8_t>> map; /**< Structure containing the map. Consists of std::vector of std::vector objects. Each
													field within the map is stored as an unsigned char. */
	};
}