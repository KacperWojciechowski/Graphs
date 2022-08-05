#include "..\inc\Graph_list.h"

/**
 * \brief Constructor creating the graph from file.
 * 
 * The constructor accepts the path to a file in supported format. If format is incorrect
 * or the file cannot be accessed, an exception is returned. 
 * 
 * \param file_path Path of the data source file.
 * \param name Name of the graph (user-given).
 * \param type Type of the graph (from the Graph::Type enum).
 * 
 * \warning Exceptions to guard against:
 *		- std::invalid_argument - when given file extension is not supported.
 *		- std::runtime_error	- when given file cannot be accessed.
 * \attention Supported formats are .lst and .GRAPHML. The .lst representation does not
 *			  contain weights, so a weight of 1 is assumed for each connection if using
 *			  this file format as a data source. 
 * \see House of Graphs for .lst adjacency list file reference.
 */
Graph::List::List(std::string file_path, std::string name, Type type)
{
	// loading the .lst file
	if (file_path.find(".lst", 0) != std::string::npos)
	{
		std::fstream file(file_path, std::ios::in);
		if (file.good())
		{
			this->load_lst_file(file);
		}
		else
		{
			throw std::runtime_error("Could not open file");
		}
		file.close();

		this->calculate_degrees();
	}
	// loading the .GRAPHML file
	else if (file_path.find(".GRAPHML", 0) != std::string::npos)
	{
		std::fstream file(file_path, std::ios::in);
		if (file.good())
		{
			this->load_graphml_file(file);
		}
		else
		{
			throw std::runtime_error("Could not open file");
		}
		file.close();
		
		this->calculate_degrees();
	}
	// signalizing the unsupported file format
	else
	{
		throw std::invalid_argument("File not supported");
	}
}




/**
 * \brief Function loading the data from .lst file into the object.
 * 
 * This function is for internal use only. To create an object containig data
 * from .lst file, use the specified constructor.
 * 
 * \see List::List(std::string file_path, std::string name, Graph::Type type)
 * 
 * \param file Reference to the file object of the data source.
 */
void Graph::List::load_lst_file(std::fstream& file)
{
	std::string line;
	size_t pos;

	std::size_t amount = 0;

	// calculate the amount of lines in the file
	while (!file.eof())
	{
		std::getline(file, line);
		amount++;
	}
	
	file.seekg(std::ios_base::beg);

	Node vertex;

	// extract each of the adjacent vertices
	for (std::size_t i = 0; i < amount; i++)
	{
		std::getline(file, line);
		this->list.push_back({});

		if (line != "")
		{
			pos = line.find(' ');

			while (pos != std::string::npos)
			{
				line = line.substr(pos + 1);
				vertex.ID = static_cast<std::size_t>(atoi(line.c_str())) - 1;
				vertex.weight = 1;
				this->list[i].push_back(vertex);
				pos = line.find(',', pos);
			}
		}
	}
}




/**
 * \brief Function loading the data from .GRAPHML file.
 * 
 * \param file Reference to the file object of the data source.
 */
void Graph::List::load_graphml_file(std::fstream& file)
{
}




/**
 * \brief Function for calculating the degrees of each vertex.
 * 
 * This function is for internal use only.
 * 
 */
void Graph::List::calculate_degrees()
{
	std::size_t deg;

	// calculate the degree of each vertex
	for (auto itr = this->list.begin(); itr != this->list.end(); itr++)
	{
		deg = 0;
		for (auto itr2 = itr->begin(); itr2 != itr->end(); itr2++)
		{
			if (itr2->ID == std::distance(this->list.begin(), itr))
			{
				deg += 2;
			}
			else
			{
				deg++;
			}
		}
		this->degrees[std::distance(this->list.begin(), itr)] = deg;
	}
}
