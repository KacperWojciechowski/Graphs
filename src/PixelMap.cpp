#include "..\inc\PixelMap.h"

#include <string>

/**
 * Constructor creating PixelMap object based on the data loaded from input stream.
 * As this constructor accepts a stream reference, it allows user to load data from a file
 * or from a standard input stream.
 * 
 * \param stream Input stream to read the data from.
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
			this->map.emplace_back(0);
			pos = 0;

			while (pos != std::string::npos)
			{
				this->map[index].emplace_back(extract_val());
			}
		}
	}
}




/**
 * Print function displaying the map structure on the standard output.
 * 
 */
void Data::PixelMap::print()
{
	std::cout << "[\n";
	for (auto& row : this->map)
	{
		for (auto& element : row)
		{
			std::cout << std::to_string(element) << ", ";
		}
		std::cout << '\n';
	}
	std::cout << "]" << std::endl;
}
