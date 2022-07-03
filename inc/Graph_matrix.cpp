#include "Graph_matrix.h"

#include <stdexcept>
#include <iostream>

/*
	Constructor loading the graph from file. Works with .mat and .GRAPHML files. 
	Detects missing file and unsupported file format, returning by default an empty graph.
	
	Params:
	file_path	- string containing the path to the graph file
	name		- user-given name of the graph. Overwritten if loading .GRAPHML file
	type		- selected type of the graph. Overwritten if loading .GRAPHML file

	Return:
	object of matrix class containing the loaded data. If unsupported file type was given,
	the object contains an empty matrix.
*/
Graph::Matrix::Matrix(std::string file_path, std::string name, Type type)
	: name(name),
	  type(type)
{
	// loading the .mat file
	if (file_path.find(".mat", 0) != std::string::npos)
	{
		std::fstream file(file_path, std::ios::in);
		this->load_mat_file(file);
		file.close();
	}
	// loading the .GRAPHML file
	else if (file_path.find(".GRAPHML", 0) != std::string::npos)
	{
		std::fstream file(file_path, std::ios::in);
		this->load_graphml_file(file);
		file.close();
	}
	// signalizing the unsupported file format 
	else
	{
		throw std::invalid_argument("File not supported");
	}
}




/*
	Simple copy constructor allowing for creating a deep copy
	of each of the private class fields.

	Params:
	m - reference to the copied matrix object

	Return:
	object of the matrix class containing the copy of each
	of private fields from the source object.
*/

Graph::Matrix::Matrix(Matrix& m)
{
	this->matrix = m.matrix;
	this->name = m.name;
	this->type = m.type;
}




/*
	Function printing the information regarding current graph
	such as graph name, graph type, number of vertices and
	full adjacency matrix

	Params:
	None

	Return:
	None
*/

void Graph::Matrix::print()
{
	// display name and type information
	std::cout << "Name = " << this->name << std::endl;
	std::cout << "Type = ";
	
	switch (this->type)
	{
	case Type::undefined:
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
	std::cout << "Vertices = " << this->matrix.size() << std::endl;

	// display adjacency matrix
	std::cout << "[" << std::endl;

	for (auto itr = this->matrix.begin(); itr != this->matrix.end(); itr++)
	{
		for (auto itr2 = itr->begin(); itr2 != itr->end(); itr2++)
		{
			std::cout << *itr2 << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}




/*
	Function loads the data from .mat file into the matrix object.

	Params:
	file - reference to the .mat to read data from

	Return:
	None
*/

void Graph::Matrix::load_mat_file(std::fstream& file)
{
	std::string line;
	std::size_t vertices = 0;
	
	// read a line from file
	std::getline(file, line);

	// reset offset and current position in line
	std::size_t offset = 0;
	std::size_t pos = 0;

	// read the vertices count to use in further loading of data
	while (true)
	{
		// search for the next space character from the offset position
		pos = line.find(' ', offset);

		// if such character was found, then it means the line contains next vertex
		if (pos != std::string::npos)
		{
			vertices++;
			offset = pos + 1;
		}
		// if space was not found, this means there is only one more vertex left in the line
		else
		{
			vertices++;
			break;
		}
	}

	// move to the beginning of the file for matrix load
	file.seekg(std::ios_base::beg);

	// load the matrix
	uint32_t temp;
	std::vector<uint8_t> v;

	for (std::size_t i = 0; i < vertices; i++)
	{
		v.clear();
		for (std::size_t j = 0; j < vertices; j++)
		{
			file >> temp;
			v.push_back(static_cast<uint8_t>(temp));
		}
		this->matrix.push_back(v);
	}
}




/*
	Function loads the data from .GRAPHML file

	Params:
	file	- reference to the data source file

	Return:
	None
*/

void Graph::Matrix::load_graphml_file(std::fstream& file)
{
	std::string line;

	// set the default position of graph ID
	std::size_t pos = std::string::npos;

	// search for graph ID
	while (pos == std::string::npos)
	{
		std::getline(file, line);
		pos = line.find("graph id=");
	}

	// search for the edge type
	std::size_t pos2 = line.find("edgedefault =");

	// save ID
	this->name = line.substr(pos + 10, pos2 - 2 - (pos + 10));

	// search for the end of the marker and save graph type
	pos = line.find("\">");
	std::string type = line.substr(pos2 + 13, pos - (pos2 + 13));
	
	if (type == "undirected")
	{
		this->type = Type::undirected;
	}
	else if (type == "directed")
	{
		this->type = Type::directed;
	}
	else
	{
		this->type = Type::undefined;
	}

	// search for vertices count
	std::getline(file, line);
	pos = line.find("node");

	std::size_t vertices = 0;

	while (pos != std::string::npos)
	{
		vertices++;
		std::getline(file, line);
		pos = line.find("node");
	}

	// create adjacency matrix and fill it with zeros
	std::vector<uint8_t> v;

	for (std::size_t j = 0; j < vertices; j++)
	{
		v.push_back(0);
	}

	for (std::size_t i = 0; i < vertices; i++)
	{
		this->matrix.push_back(v);
	}

	// find the edge marker
	pos = line.find("edge");

	std::string id1;
	std::string id2;

	// extract edge info
	while (pos != std::string::npos)
	{
		// find and extract the source and target nodes IDs
		pos = line.find("source=");
		pos2 = line.find("\" target=");

		id1 = line.substr(pos + 9, pos2 - (pos + 9));
		pos = line.find("\"/>");
		id2 = line.substr(pos2 + 11, pos - (pos2 + 11));

		// input the connection into matrix
		this->matrix[atoi(id1.c_str())][atoi(id2.c_str())] = 1;

		// search for next edge marker
		std::getline(file, line);
		pos = line.find("edge");
	}
}
