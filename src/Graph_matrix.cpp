// this
#include "Graph_matrix.h"

// libraries
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <cstring>
#include <cmath>

/* 
	Constructor loading the graph from file. Works with .mat and .GRAPHML files,
	detects missing file and unsupportedd file format returning default empty graph.
	The function automatically determines the size of the graph contained within file,
	and creates sufficiently big matrix. 

	Params:
	file_path				- string containing the path to the graph file
	throughtput_file_path	- string containing the path to the throughtput matrix file of the graph
	name					- string containing the name of the graph. Will be overwritten in case of
							  reading from .GRAPHML
	type					- string containing the type of the graph (either "directed" or "undirected").
							  Will be overwritten in case of reading from .GRAPHML

	Return:
	None
*/
Graph::Matrix::Matrix(std::string file_path, std::string throughtput_file_path, std::string name, std::string type)
	: matrix(nullptr),
	  nodes_amount(0),
	  graph_name(name),
	  graph_type(type),
	  distance(nullptr),
	  prev_node(nullptr),
	  throughtput(nullptr)
{
	std::ifstream file(file_path);

	if (file.good())
	{
		// if opened file extension is .mat
		if (file_path.find(".mat", 0) != std::string::npos)
		{
			std::string line;
			uint32_t vertexes = 0;

			// read a line from file
			std::getline(file, line);

			// reset offset and current position
			std::size_t offset = 0;
			std::size_t pos = 0;

			// count the amount of vertices
			while (true)
			{
				// search for next space character from the offset position
				pos = line.find(' ', offset);

				// if such character was found, then it means the line contains next vertex
				if (pos != std::string::npos)
				{
					vertexes++;
					offset = pos + 1;
				}
				// if space was not found, this means there is only one more vertex left
				else
				{
					vertexes++;
					break;
				}
			}

			// save the vertices amount and create adjacency matrix
			this->nodes_amount = vertexes;
			this->matrix = new uint32_t * [this->nodes_amount];
			for (uint32_t i = 0; i < this->nodes_amount; i++)
			{
				this->matrix[i] = new uint32_t[this->nodes_amount];
			}

			// create arrays for the SSP functions
			this->distance = new uint32_t[this->nodes_amount];
			this->prev_node = new std::vector<uint32_t>[this->nodes_amount];

			// move to the beginnig of the file for matrix load
			file.seekg(std::ios_base::beg);

			// read the matrix
			uint32_t temp;
			for (uint32_t i = 0; i < this->nodes_amount; i++)
			{
				for (uint32_t j = 0; j < this->nodes_amount; j++)
				{
					file >> temp;
					this->matrix[i][j] = temp;
				}
			}
		}
		// file is in GraphML format
		else if(file_path.find(".GRAPHML") != std::string::npos)
		{
			std::string line;

			// search for graph ID
			std::getline(file, line);
			std::size_t pos = line.find("graph id=");

			// skip any unneeded header lines
			while (pos == std::string::npos)
			{
				std::getline(file, line);
				pos = line.find("graph id=");
			}

			// search for edge type
			std::size_t pos2 = line.find("edgedefault=");

			// save type and ID data
			this->graph_name = line.substr(pos + 10, pos2 - 2 - (pos + 10));
			pos = line.find("\">");
			this->graph_type = line.substr(pos2 + 13, pos - (pos2 + 13));

			// search for node marker
			std::getline(file, line);
			pos = line.find("node");

			// count the amount of nodes in the file
			while (pos != std::string::npos)
			{
				this->nodes_amount++;
				std::getline(file, line);
				pos = line.find("node");
			}
			
			// create adjacency matrix and fill it with zeros
			this->matrix = new uint32_t*[this->nodes_amount];
			for (uint32_t i = 0; i < this->nodes_amount; i++)
			{
				this->matrix[i] = new uint32_t[this->nodes_amount];
				for (uint32_t j = 0; j < this->nodes_amount; j++)
				{
					this->matrix[i][j] = 0;
				}
			}

			// search for edge marker
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
		// if given file extension is unsupported
		else
		{
		std::cout << "Unsupported file format" << std::endl;
		}
		
		// open and read the throughtput file format
		std::ifstream thr_file(throughtput_file_path);
		uint32_t value;

		// open the throughtput matrix file
		if (thr_file.good())
		{
			// create sufficiently big matrix
			this->throughtput = new uint32_t * [this->nodes_amount];
			for (uint32_t i = 0; i < this->nodes_amount; i++)
			{
				this->throughtput[i] = new uint32_t[this->nodes_amount];
				
				// load the row
				for (uint32_t j = 0; j < this->nodes_amount; j++)
				{
					thr_file >> value;
					this->throughtput[i][j] = value;
				}
			}
		}
		thr_file.close();
	}
	// if file is missing
	else
	{
		std::cout << "File missing" << std::endl;
	}
	file.close();
}


/*
	Create a matrix object based on Graph::List object. 

	Params:
	list - reference to the List object from which to create the
		   matrix

	Return:
	None
*/
Graph::Matrix::Matrix(List& list)
	: throughtput(nullptr)
{
	// get vertices count and create sufficient matrix
	// filling it with zeros
	this->nodes_amount = list.get_nodes_amount();
	this->matrix = new uint32_t * [this->nodes_amount];
	this->distance = new uint32_t[this->nodes_amount];
	this->prev_node = new std::vector<uint32_t>[this->nodes_amount];

	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		this->matrix[i] = new uint32_t[this->nodes_amount];
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			this->matrix[i][j] = 0;
		}
	}

	uint32_t neighbours;
	uint32_t neighbour_id;

	// iterate throug the neighbours of each vertex
	// and save the edges
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		neighbours = list.get_node_degree(i);

		for (uint32_t j = 0; j < neighbours; j++)
		{
			neighbour_id = list.get_neighbour(i, j);
			this->matrix[i][neighbour_id - 1] = 1;
		}
	}
}

/*
	Function that exports current graph into .GRAPHML file.

	Params:
	file_path - destination file path

	Return:
	None
*/
void Graph::Matrix::saveGraphML(std::string file_path)
{
	// opening output file
	std::ofstream file(file_path);
	
	// header
	file << "<?xml version=\"1.0\"";
	file << " encoding=\"UTF-8\"?>\n";

	// xml schema
	file << "<graphml xmlns=";
	file << "\"http://graphml.graphdrawing.org/xmlns\"\n";
	file << "	xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
	file << "	xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n";
	file << "	http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";

	// graph type data
	file << " <graph id=";
	file << "\"" + graph_name + "\"";
	file << " edgedefault=";
	file << "\"" + graph_type + "\">\n";

	// nodes data
	for (uint32_t i = 0; i < this->nodes_amount; i++) // Tworzy wiersze nod�w <node id="n0"/>
	{
		file << "	<node id=\"n" << i;
		file << "\"/>\n";
	}

	// edge data
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			if (this->matrix[i][j] == 1 )
			{
				file << "	<edge source=\"n" << i;
				file << "\" target=\"n" << j;
				file << "\"/>\n";
			}
		}
	}
	// close the markers and the file
	file << " </graph>\n";
	file << "</graphml>";
	file.close();
}

/*	Function calculates the Estrada index for the graph
	given the equation:

	index = sum(sqrt(deg(u) + deg(v) - 2)/(deg(v)^2)

	Params:
	None
   
	Return: 
	value of Estrada index
*/
float Graph::Matrix::estrada_index()
{
	uint32_t* degrees = new uint32_t[this->nodes_amount];

	// calculating degrees
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		degrees[i] = 0;
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			if (this->matrix[i][j] == 1)
			{
				degrees[i]++;
			}
		}
	}

	float index = 0;

	// calculating the index
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			if (this->matrix[i][j] == 1) // �apie s�siada
			{
				float root = std::sqrt(static_cast<float>(degrees[i] + degrees[j] - 2)/(pow(degrees[i], 2))); 
				index += root;
			}
		}		
	}

	// free allocated memory
	delete[] degrees;
	
	// return Estrada index value
	return index;
}

/*
	Add an edge between two given vertices. Function validates if
	given vertices IDs are in range.

	Params:
	source		- source vertice
	destination - destination vertice

	Return:
	None
*/
void Graph::Matrix::add_edge(uint32_t source, uint32_t destination)
{
	if (source < this->nodes_amount && destination < this->nodes_amount)
	{
		this->matrix[source][destination] = 1;
		if (this->graph_type == "undirected")
		{
			this->matrix[destination][source] = 1;
		}
	}
}

/*
	Add a vertex to the graph. Due to creating the matrix to the specific
	size, adding a vertex forces reallocation of the structure. Added vertex
	is initially isolated.

	Params:
	None

	Return:
	None
*/
void Graph::Matrix::add_node()
{
	// increase the amount of vertices in the temporary variable
	uint32_t amount = this->nodes_amount + 1;

	// create sufficiently big matrix
	uint32_t** m = new uint32_t * [amount];

	for (uint32_t i = 0; i < amount; i++)
	{
		m[i] = new uint32_t[amount];
	}

	// move and fit the current matrix into the new one
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			std::memcpy(m[i], this->matrix[i], this->nodes_amount * sizeof(uint32_t));
		}
		// free memory allocated by the previous matrix row
		delete[] this->matrix[i];
	}

	// fill last column and row with zeros
	for (uint32_t i = 0; i < amount; i++)
	{
		m[i][amount - 1] = 0;
		if (i == amount - 1)
		{
			for (uint32_t j = 0; j < amount - 1; j++)
			{
				m[i][j] = 0;
			}
		}
	}

	// free the remains of unused allocated memory
	delete[] this->matrix;

	// increase the nodes amount and overwrite the address of previous matrix
	this->nodes_amount++;
	this->matrix = m;
}

/*
	Removing the edge between two nodes. Function verifies is the vertices
	indexes are within range.

	Params:
	source		- source vertex of the edge
	destination	- destination vertex of the edge

*/
void Graph::Matrix::remove_edge(uint32_t source, uint32_t destination)
{
	if (source < this->nodes_amount && destination < this->nodes_amount)
	{
		this->matrix[source][destination] = 0;
		if (this->graph_type == "undirected")
		{
			this->matrix[destination][source] = 0;
		}
	}
}

/*
	Removing the node along with all its edges. The function
	validates if the vertex id is within range. Removal of a vertex
	causes renumeration of subsequent verticies and reallocating
	the matrix.

	Params:
	node_id - id of the node to remove

	Return:
	None
*/
void Graph::Matrix::remove_node(uint32_t node_id)
{
	if (node_id < this->nodes_amount)
	{
		// save reduced nodes amount in temporary variable
		uint32_t amount = this->nodes_amount - 1;

		// prepare shrunken matrix
		uint32_t** m = new uint32_t * [amount];
		for (uint32_t i = 0; i < amount; i++)
		{
			m[i] = new uint32_t[amount];
		}

		uint32_t row = 0;
		uint32_t col = 0;

		// rewrite the values from old matrix to the new one
		// while skipping the column and row of deleted vertex
		for (uint32_t i = 0; i < this->nodes_amount; i++)
		{
			if (i != node_id)
			{
				for (uint32_t j = 0; j < this->nodes_amount; j++)
				{
					if (j != node_id)
					{
						m[row][col] = this->matrix[i][j];
						col++;
					}
				}
				col = 0;
				row++;
			}
			// free already moved row
			delete[] this->matrix[i];
		}
		// free the rest of the matrix
		delete[] this->matrix;

		// overwrite the pointer with the addres of new matrix
		// and save new vertices count
		this->matrix = m;
		this->nodes_amount = amount;
	}
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
	// print basic information
	std::cout << "Name = " << this->graph_name << std::endl;
	std::cout << "Type = " << this->graph_type << std::endl;
	std::cout << "Nodes amount = " << this->nodes_amount << std::endl;

	// print adjacency matrix
	std::cout << "[" << std::endl;
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			std::cout << this->matrix[i][j] << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}

/*
	Function printing the throughtput matrix.

	Params:
	None

	Return:
	None
*/
void Graph::Matrix::print_throughtput()
{
	std::cout << "[" << std::endl;
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			std::cout << this->throughtput[i][j] << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}

/*
	Function returning nodes count as a const value.

	Params:
	None

	Return:
	Amount of nodes in current graph
*/
const uint32_t Graph::Matrix::get_nodes_amount()
{
	return this->nodes_amount;
}


/*
	Function returning the value of adjacency matrix for given
	coordinates. Automatically validates if coordinates are
	out of bounds. In case of out of bounds indexes returns 0xFFFFFFFF.

	Params:
	x - row coordinate of the matrix
	y - column coordinate of the matrix

	Return:
	Value of the connection between given vertices. Returns 0xFFFFFFFF
	if coordinates were out of bounds.
*/
const uint32_t Graph::Matrix::get_value(uint32_t x, uint32_t y)
{
	uint32_t ret = 0xFFFFFFFF;
	if (x < this->nodes_amount && y < this->nodes_amount)
	{
		this->matrix[x][y] = 0;
	}
	return ret;
}

/*
	Function transforming current graph into its line graph.
	This function overwrites the initial graph, and causes reallocation
	of memory during the creation of line graph's adjacency matrix.
	Newly created line graph is contained within the same object.
	Compatibile only with undirected graphs.

	Params:
	None

	Return:
	None
*/
void Graph::Matrix::change_to_line_graph()
{
	// gather all the edges from the adjacency matrix of initial graph
	std::vector<coord> edges;
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = i; j < this->nodes_amount; j++)
		{
			if (this->matrix[i][j] == 1)
			{
				edges.push_back({ i, j });
			}
		}
	}

	// create sufficient matrix for the line graph and fill it with zeros
	uint32_t size = static_cast<uint32_t>(edges.size());
	uint32_t** m = new uint32_t * [size];
	for (uint32_t i = 0; i < size; i++)
	{
		m[i] = new uint32_t[size];
		for (uint32_t j = 0; j < size; j++)
		{
			m[i][j] = 0;
		}
	}

	coord coordinates;
	uint32_t index;

	// fill the line graph
	for (uint32_t i = 0; i < size; i++)
	{
		// load coordinates of the edge
		coordinates = edges[i];

		// search though the matrix for common vertices between edges
		// in the same row of the initial adjacency matrix
		for (uint32_t k = coordinates.x; k < this->nodes_amount; k++)
		{
			// if common vertex was found, calculate the line graph vertex index
			// and mark the connection
			if (this->matrix[coordinates.x][k] == 1 && k != coordinates.y)
			{
				index = find_index(edges, coordinates.x, k);
				m[i][index] = 1;
				m[index][i] = 1;
			}
		}
		// search through the matrix for common vertices between edges
		// in the same column of the initial adjacency matrix
		for (uint32_t k = coordinates.x; k < coordinates.y; k++)
		{
			// if common vertex was found, calculate the line graph vertex index
			// and mark the connection
			if (this->matrix[k][coordinates.y] == 1 && k != coordinates.x)
			{
				index = find_index(edges, k, coordinates.y);
				m[i][index] = 1;
				m[index][i] = 1;
			}
		}
	}

	// free previous matrix memory allocation
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		delete[] this->matrix[i];
	}
	delete[] this->matrix;

	// overwrite the matrix with the pointer to line graph's adjacency matrix
	// and save new size of the graph
	this->matrix = m;
	this->nodes_amount = size;
}

/*
	Function provides the shortests paths between selected vertex and all other
	vertices. Results are stored inside the class. Value 0xFFFFFFFF in the distance
	array signifies that no path between given vertices exist. In case of several
	paths found, the algorithm remembers only the first path found.

	Params:
	vertex	- vertex from which to search for the paths
	log		- flag signaling whether to print logs during algorithm run

	Return:
	The duration of algorithm operations. Note that the duration is affected by
	logs, so for true algorithm performance, it must be measured with logs turned
	off by setting the log flag to false.

	this->distance	- distance from selected vertex to all other vertices
	this->prev_node - previous node in the path to given vertex
*/
int32_t Graph::Matrix::belman_ford(uint32_t vertex, bool log)
{
	// set default values for the distances and previous vertices
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		this->distance[i] = 0x7FFFFFFF;
		this->prev_node[i].clear();
	}

	// set zero as a distance for the starting vertex
	this->distance[vertex] = 0;

	bool change_found;

	auto start = std::chrono::system_clock::now();

	// search through the graph multiple times to find shortest paths
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		// assume change in distance was not found
		change_found = false;
		for (uint32_t x = 0; x < this->nodes_amount; x++)
		{
			for (uint32_t j = 0; j < this->nodes_amount; j++)
			{
				// if change in distance between connected vertices was found, save said information
				if (this->distance[j] > this->distance[x] + this->matrix[x][j] && this->matrix[x][j] != 0)
				{
					// remember that change was found, to continue search
					change_found = true;
					this->distance[j] = this->distance[x] + this->matrix[x][j];
					this->prev_node[j].clear();
					this->prev_node[j].push_back(x);
				}
			}
			// if log flag is set, print informations about distance and previous nodes
			if (log)
			{
				// distance information
				std::cout << "Distances: " << std::endl;
				for (uint32_t j = 0; j < this->nodes_amount; j++)
				{
					std::cout << this->distance[j] << ", ";
				}
				// previous vertices information
				std::cout << std::endl << "Previous: " << std::endl;
				for (uint32_t j = 0; j < this->nodes_amount; j++)
				{
					for (auto itr = this->prev_node[j].begin(); itr != this->prev_node[j].end(); itr++)
						std::cout << *itr << ", ";
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
		// if change was not found, optimize the algorithm by stopping the search
		if (!change_found)
		{
			break;
		}
	}

	// calculate how long the algorithm was running
	auto end = std::chrono::system_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	// if log flag is set, print the duration
	if (log)
	{
		std::cout << "Duration: " << diff.count() << " us" << std::endl;
	}
	return diff.count();
}

/*
	Modified Belman-Fort algorithm finding the shortests paths with
	additionally fulfilling the throughtput requirement. As a result, several
	paths may be stored within the prev_node vectors, for all the previous
	vertices fulfilling the minimum throughtput requirement.

	Params:
	searched_throughtput	- the minimal throughtput the path must fulfill
	vertex					- the vertex to begin the search from
	log						- flag signaling whether to print logs during run

	Result:
	The duration of algorithm operations. Note that the duration is affected by
	logs, so for true algorithm performance, it must be measured with logs turned
	off by setting the log flag to false.

	this->distance	- array of distances from the beginning vertex to all other
					  vertices
	this->prev_node - array of vectors containing the possible paths fulfilling
					  the minimal throughtput requirement
*/
int32_t Graph::Matrix::throughtput_belman_ford(uint32_t searched_throughtput, uint32_t vertex, bool log)
{
	// set default values for the distances and previous vertices
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		this->distance[i] = 0x7FFFFFFF;
		this->prev_node[i].clear();
	}

	// set zero as a distance for the starting vertex
	this->distance[vertex] = 0;

	bool change_found;

	auto start = std::chrono::system_clock::now();
	// search through the graph multiple times to find shortest paths
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		// assume change in distance was not found
		change_found = false;
		for (uint32_t x = 0; x < this->nodes_amount; x++)
		{
			for (uint32_t j = 0; j < this->nodes_amount; j++)
			{
				// if change in distance between connected vertices was found while fulfilling the throughtput requirement, 
				// save said information
				if (this->distance[j] > this->distance[x] + this->matrix[x][j] && this->throughtput[x][j] >= searched_throughtput)
				{
					// remember the change was found to continue search
					change_found = true;
					this->distance[j] = this->distance[x] + this->matrix[x][j];
					this->prev_node[j].clear();
					this->prev_node[j].push_back(x);
				}
				// if a path of the same distance was found, fulfilling the throughtput requirement, and is not already
				// saved, save it in prev_node vector.
				else if (this->distance[j] == this->distance[x] + this->matrix[x][j]
					&& this->throughtput[x][j] >= searched_throughtput
					&& std::find(this->prev_node[j].begin(), this->prev_node[j].end(), x) == this->prev_node[j].end())
				{
					// remember the change was found to continue search
					change_found = true;
					this->prev_node[j].push_back(x);
				}
			}
			// if log flag is set, print informations about distance and previous nodes
			if (log)
			{
				// distance information
				std::cout << "Distances: " << std::endl;
				for (uint32_t j = 0; j < this->nodes_amount; j++)
				{
					std::cout << this->distance[j] << ", ";
				}
				// previous vertices information
				std::cout << std::endl << "Previous: " << std::endl;
				for (uint32_t j = 0; j < this->nodes_amount; j++)
				{
					for (auto itr = this->prev_node[j].begin(); itr != this->prev_node[j].end(); itr++)
						std::cout << *itr << ", ";
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
		// if change was not found, optimize the algorithm by stopping the search
		if (!change_found)
		{
			break;
		}
	}

	// calculate how long the algorithm was running
	auto end = std::chrono::system_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	// if log flag is set, print the duration
	if (log)
	{
		std::cout << "Duration: " << diff.count() << " us" << std::endl;
	}
	
	return diff.count();
}

/*
	Destructor freeing the memory allocation
*/
Graph::Matrix::~Matrix()
{
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		delete[] this->matrix[i];
	}
	delete[] this->matrix;
}

/*
	Function searching for the line graph's vertex index. Created for
	the purpose of function calculating the line graph of current graph.
	If the vertex index was not found, function returns 0xFFFFFFFF.

	Params:
	edges - reference to the edges vector of the initial graph
*/
uint32_t Graph::Matrix::find_index(std::vector<coord>& edges, uint32_t x, uint32_t y)
{
	uint32_t size = static_cast<uint32_t>(edges.size());

	uint32_t ret = 0xFFFFFFFF;

	// if coordinates match the source and destination of given edge,
	// it means that this edge is the line graph's vertex that we're looking for
	for (uint32_t i = 0; i < size; i++)
	{
		if (edges[i].x == x && edges[i].y == y)
		{
			ret = i;
		}
	}
	return ret;
}

/*
	Generates a random weighted graph in given destination.

	Params:
	file_path		- string containing the path for the output file
	nodes_amount	- size of the graph

	Return:
	None
*/
void Data::generate_weighted_graph(std::string file_path, uint32_t nodes_amount)
{
	std::ofstream file(file_path);

	srand(time(NULL));

	if (file.good())
	{
		// generate a graph of given size with random weights in the 
		// range of <0;n>. Zero means no connection between vertices.
		for (uint32_t i = 0; i < nodes_amount; i++)
		{
			for (uint32_t j = 0; j < nodes_amount; j++)
			{
				if (i != j)
				{
					file << (rand() % nodes_amount) * (rand() % 6 <= 3 ? 1 : 0);
				}
				else
				{
					file << 0;
				}
				if (j < nodes_amount - 1)
				{
					file << " ";
				}
			}
			if (i < nodes_amount - 1)
			{
				file << std::endl;
			}
		}
	}
	file.close();
}

/*
	Generate throughtput matrix file based on the graph adjacency matrix file.

	Params:
	input_file_path		- adjacency matrix file path
	output_file_path	- output throughtput matrix file path
	nodes_amount		- number of vertices

	Return:
	None
*/
void Data::generate_throughtput(std::string input_file_path, std::string output_file_path, uint32_t nodes_amount)
{
	std::ofstream o_file(output_file_path);
	std::ifstream i_file(input_file_path);

	srand(time(NULL));

	uint32_t value;

	if (i_file.good())
	{
		// generate a matrix of throughtput for given graph file
		for (uint32_t i = 0; i < nodes_amount; i++)
		{
			for (uint32_t j = 0; j < nodes_amount; j++)
			{
				i_file >> value;
				if (value != 0)
				{
					o_file << rand() % 9 + 1;
				}
				else
				{
					o_file << 0;
				}
				if (j < nodes_amount - 1)
				{
					o_file << " ";
				}
			}
			if (i < nodes_amount - 1)
			{
				o_file << std::endl;
			}
		}
	}
	o_file.close();
	i_file.close();
}
