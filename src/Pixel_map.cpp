#include "Pixel_map.h"

#include <fstream>
#include <iostream>


void Data::randomize_map(uint32_t length, uint32_t width)
{
	std::fstream file;
	file.open("area.smap", std::ios::out);
	srand(static_cast<unsigned int>(time(NULL)));

	uint32_t sector;

	for (uint32_t i = 0; i < length; i++)
	{
		for (uint32_t j = 0; j < width; j++)
		{
			sector = rand() % 2;
			file << sector;
			if (j < width - 1)
			{
				file << ' ';
			}
		}
		if(i < length - 1)
		{ 
			file << std::endl;
		}
	}
	file.close();
}

Data::Pixel_map::Pixel_map(std::string file_path)
	: area_map(nullptr),
	rows(0),
	columns(0)
{
	std::ifstream file(file_path, std::ios::in);
	if (file.is_open())
	{
		std::string line;
		
		std::getline(file, line);
		size_t offset = 0;
		size_t pos = 0;

		while (true)
		{
			pos = line.find(' ', offset);
			if (pos != std::string::npos)
			{
				this->columns++;
				offset = pos + 1;
			}
			else
			{
				break;
			}
		}
		this->columns++;
		this->rows++;

		while (!file.eof() && line != "")
		{
			this->rows++;
			std::getline(file, line);
		}
		
		this->area_map = new uint8_t * [this->rows];

		for (uint32_t i = 0; i < this->rows; i++)
		{
			this->area_map[i] = new uint8_t[this->columns];
		}

		file.seekg(std::ios_base::beg);

		uint32_t temp;
		for (uint32_t i = 0; i < this->rows; i++)
		{
			for (uint32_t j = 0; j < this->columns; j++)
			{
				file >> temp;
				this->area_map[i][j] = static_cast<uint8_t>(temp);
			}
		}
	}
}

void Data::Pixel_map::print_area_map()
{
	std::cout << "[" << std::endl;
	for (uint32_t i = 0; i < this->rows; i++)
	{
		for (uint32_t j = 0; j < this->columns; j++)
		{
			std::cout << static_cast<uint32_t>(this->area_map[i][j]);
			if (j < this->columns - 1)
			{
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}

uint32_t Data::Pixel_map::get_rows()
{
	return this->rows;
}

uint32_t Data::Pixel_map::get_columns()
{
	return this->columns;
}

uint32_t Data::Pixel_map::get_field(uint32_t x, uint32_t y)
{
	return static_cast<uint32_t>(this->area_map[x][y]);
}

Data::Pixel_map::~Pixel_map()
{
	for (uint32_t i = 0; i < this->rows; i++)
	{
		delete[] this->area_map[i];
	}
	delete[] this->area_map;
}
