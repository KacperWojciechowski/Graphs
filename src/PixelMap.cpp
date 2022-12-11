#include "PixelMap.h"

#include <string>

/**
 * Constructor creating PixelMap object based on the data loaded from input stream.
 * As this constructor accepts a stream reference, it allows user to load data from a file
 * or from a standard input stream.
 *
 * \warning Exception to guard against:
 *			- std::runtime_error - A row is deviating from the length set by the first row.
 *
 * \param stream Input stream to read the data from.
 *
 * \ref create_pixel_map.cpp "Example of creating a Data::PixelMap object"
 */
Data::PixelMap::PixelMap(std::istream& stream)
{
	std::string line;

	// load the first line
	std::size_t pos = 0;

	// function extracting values from the line
	auto extract_val = [&line, &pos]() -> std::uint8_t
	{
		std::uint32_t val;

		val = std::stoi(line);
		pos = line.find(' ');

		if (pos != std::string::npos)
		{
			line = line.substr(pos + 1);
		}
		return static_cast<uint8_t>(val);
	};

	// parse the whole map
	for (std::size_t index = 0; std::getline(stream, line); index++)
	{
		if (line != "")
		{
			map.emplace_back(0);
			pos = 0;

			while (pos != std::string::npos)
			{
				map[index].emplace_back(extract_val());
			}

			if (map[index].size() != map[0].size())
			{
				throw std::runtime_error("Deviating row length");
			}
		}
	}
}




/**
 * Simple copy constructor.
 *
 * \param p l-value reference to a Data::PixelMap object.
 */
Data::PixelMap::PixelMap(const PixelMap& p) noexcept
	: map(p.map)
{
}




/**
 * Simple move constructor.
 *
 * \param p r-value reference to a Data::PixelMap object.
 */
Data::PixelMap::PixelMap(PixelMap&& p) noexcept
	: map(p.map)
{
	p.map.clear();
}




/**
 * Print function displaying the map structure on the standard output.
 *
 */
auto Data::PixelMap::print() const noexcept -> void
{
	std::cout << "[\n";
	for (const auto& row : map)
	{
		for (const auto& element : row)
		{
			std::cout << std::to_string(element) << " ";
		}
		std::cout << '\n';
	}
	std::cout << "]" << std::endl;
}