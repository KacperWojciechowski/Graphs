#include "..\inc\Graph_list.h"

#include <iomanip>

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
	: name(name),
	type(type)
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
	
	file.clear();
	file.seekg(std::ios_base::beg);

	Node vertex;

	std::cout << amount << std::endl;

	// extract each of the adjacent vertices
	for (std::size_t i = 0; i < amount; i++)
	{
		std::getline(file, line);

		if (line != "")
		{
			this->list.push_back({});

			std::cout << this->list.size() << std::endl;

			pos = line.find(' ');

			auto itr = std::next(this->list.begin(), this->list.size() - 1);

			while (pos != std::string::npos)
			{
				line = line.substr(pos + 1);
				vertex.ID = static_cast<std::size_t>(atoi(line.c_str())) - 1;
				vertex.weight = 1;
				itr->push_back(vertex);
				pos = line.find(' ', pos);
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
		this->degrees.push_back(deg);
	}
}




/**
 * \brief Function printing information regarding the graph.
 * 
 * Information displayed are such as:
 *  - Name of the graph
 *  - Type of the graph
 *	- ID of the vertex
 *	- IDs of adjacent vertexes
 *  - Weights of the connections
 *  - Degree of each vertex
 * 
 *	\note Data is displayed in format "degree: ___, ID: adj_vert_ID {weight}, adj_vert_ID {weight}, ..."
 * 
 */
void Graph::List::print()
{
	// display graph name and type info
	std::cout << "Name = " << this->name << std::endl;
	std::cout << "Type = ";

	switch (this->type)
	{
	case Type::undirected:
		std::cout << "undirected ";
		break;

	case Type::directed:
		std::cout << "directed ";
		break;

	default:
		std::cout << "undefined ";
		break;
	}

	std::cout << std::endl;

	// display vertices count
	std::cout << "Vertices = " << this->list.size() << std::endl;

	std::cout << "{" << std::endl;

	std::size_t index;

	for (auto itr = this->list.begin(); itr != this->list.end(); itr++)
	{
		index = std::distance(this->list.begin(), itr);
		std::cout << "degree: " << std::left << std::setw(6) << this->degrees[index] << ", " << index << ": ";
		for (auto itr2 = itr->begin(); itr2 != itr->end(); itr2++)
		{
			std::cout << itr2->ID << " {" << itr2->weight << "}, ";
		}
		std::cout << std::endl;
	}

	std::cout << "}" << std::endl;
}
