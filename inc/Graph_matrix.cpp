#include "Graph_matrix.h"

#include <stdexcept>
#include <iostream>
#include <iomanip>

#include "../inc/Coord.h"

/*
	Constructor loading the graph from file. Works with .mat and .GRAPHML files. 
	Detects missing file and unsupported file format, returning by default an empty graph.
	The class works only with integer weights.
	
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
		if (file.good())
		{
			this->load_mat_file(file);
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
		if(file.good())
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




/*
	Constructor creating a graph based on the given matrix made up of vectors.

	Params:
	mat		- vector of vectors containing the matrix values
	name	- name of the graph
	type	- type of the graph

	Return:
	None
*/
Graph::Matrix::Matrix(std::vector<std::vector<int32_t>>& mat, std::string name, Type type)
{
	this->matrix = mat;
	this->name = name;
	this->type = type;
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
	std::cout << "Vertices = " << this->matrix.size() << std::endl;

	// display adjacency matrix
	std::cout << "[" << std::endl;

	for (auto itr = this->matrix.begin(); itr != this->matrix.end(); itr++)
	{
		for (auto itr2 = itr->begin(); itr2 != itr->end(); itr2++)
		{
			std::cout << std::setw(3) << std::right << *itr2 << ", ";
		}
		std::cout << std::endl;
	}

	std::cout << "]" << std::endl;
}




/*
	Function adds a connection between source and destination vertices.

	Params:
	source		- source vertex
	destination	- destination vertex
	weight		- weight of inserted connection

	Return:
	None
*/

void Graph::Matrix::add_edge(std::size_t source, std::size_t destination, uint32_t weight)
{
	// validate the vertices indexes
	if (source >= this->matrix.size() || destination >= this->matrix.size())
	{
		throw std::out_of_range("Index out of bounds");
	}
	else if (weight == 0)
	{
		throw std::invalid_argument("Weight equal to zero");
	}
	// if indexes are correct, mark the connection
	else
	{
		this->matrix[source][destination] = weight;

		// if graph type is undirected, mark the connection both ways
		if (this->type == Type::undirected)
		{
			this->matrix[destination][source] = weight;
		}

		// update the degrees of given vertices
		if (source == destination)
		{
			this->degrees[source] += 2;
		}
		else
		{
			this->degrees[source] ++;
			if (this->type == Type::undirected)
			{
				this->degrees[destination]++;
			}
		}
	}
}





/*
	Add a node with no connections to the graph.

	Params:
	None

	Return:
	None
*/

void Graph::Matrix::add_node()
{
	// add zeros at the end of each row to mark the new vertex to be added
	for (auto itr = this->matrix.begin(); itr != this->matrix.end(); itr++)
	{
		itr->push_back(0);
	}

	// create the vector representing the neighbours of the new vertex
	std::size_t size = this->matrix[0].size();
	std::vector<int32_t> v;

	for (std::size_t i = 0; i < size; i++)
	{
		v.push_back(0);
	}

	// add new vertex row to the matrix
	this->matrix.push_back(v);
	this->degrees.push_back(0);
}




/*
	Remove a connection between two given vertices

	Params:
	source		- source vertex of the connection
	destination	- destination vertex of the connection

	Return:
	None
*/

void Graph::Matrix::remove_edge(std::size_t source, std::size_t destination)
{
	// validate the vertices indexes
	if (source >= this->matrix.size() || destination >= this->matrix.size())
	{
		throw std::out_of_range("Index out of bounds");
	}
	// if indexes are correct, mark the connection
	else
	{
		this->matrix[source][destination] = 0;

		// if graph type is undirected, mark the connection both ways
		if (this->type == Type::undirected)
		{
			this->matrix[destination][source] = 0;
		}

		// update the degrees of given vertices
		if (source == destination)
		{
			this->degrees[source] -= 2;
		}
		else
		{
			this->degrees[source] --;

			if (this->type == Type::undirected)
			{
				this->degrees[destination] --;
			}
		}
	}
}




/*
	Remove a vertex along with all its connections from the graph.

	Params:
	node_id - id of the node to remove

	Return:
	None
*/

void Graph::Matrix::remove_node(std::size_t node_id)
{
	// validate the vertex ID
	if (node_id >= this->matrix.size())
	{
		throw std::out_of_range("ID out of bounds");
	}
	// if vertex ID is correct, remove all the connections and the vertex itself
	else
	{
		// remove the column of the deleted vertex and update degree of each vertex
		for (auto itr = this->matrix.begin(); itr != this->matrix.end(); itr++)
		{
			if (*(std::next(itr->begin(), node_id)) == 1)
			{
				this->degrees[std::distance(this->matrix.begin(), itr)]--;
			}
			itr->erase(std::next(itr->begin(), node_id));
			
		}

		// remove the row of the deleted vertex and remove the degree counter
		// for the deleted vertex
		this->matrix.erase(std::next(this->matrix.begin(), node_id));
		this->degrees.erase(std::next(this->degrees.begin(), node_id));
	}
}




/*
	Getter for vertices count.

	Params:
	None

	Return:
	return - amount of vertices as const value
*/

const std::size_t Graph::Matrix::get_nodes_amount()
{
	return this->matrix.size();
}




/*
	Getter for vertex degree.

	Params:
	node_id - ID of the vertex which degree is to be fetched.

	Return:
	return - const value of the degree of given vertex
	
*/
const std::size_t Graph::Matrix::get_node_degree(std::size_t node_id)
{
	if (node_id >= this->matrix.size())
	{
		throw std::out_of_range("ID out of bounds");
	}
	else
	{
		return this->degrees[node_id];
	}
}




/*
	Getter for the edge value between given vertices.

	Params:
	source		- source vertex of the edge
	destination - destination vertex of the edge

	Return:
	return - const value of the edge weight between given vertices. Zero
			 means no connection.
*/
const uint32_t Graph::Matrix::get_edge(std::size_t source, std::size_t destination)
{
	if (source >= this->matrix.size() || destination >= this->matrix.size())
	{
		throw std::out_of_range("Index out of bounds");
	}
	else
	{
		return this->matrix[source][destination];
	}
}




/*
	Function generating a .GRAPHML file containing current graph information.
	The format does contain the weights of the graph connections.

	Params:
	output_file_path - path to the output file

	Return:
	None
*/
void Graph::Matrix::save_graphml(std::string output_file_path)
{
	// opening output file
	std::ofstream file(output_file_path);

	// header
	file << "<?xml version=\"1,0\"";
	file << " encoding=\"UTF-8\"?>\n";

	// xml schema
	file << "<graphml xmlns=";
	file << "\"http://graphml.graphdrawing.org/xmlns\"\n";
	file << "	xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
	file << "	xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n";
	file << "	http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";

	// weight data info
	file << "<key id=\"d0\" for=\"edge\"\n";
	file << "	attr.name=\"weight\" attr.type=\"integer\"/>\n";

	// graph type data
	file << " <graph id=";
	file << "\"" + this->name + "\"";
	file << " edgedefault=";
	switch (this->type)
	{
	case Type::directed:
		file << "\"directed\">\n";
		break;
	case Type::undirected:
		file << "\"undirected\">\n";
		break;
	// in case of undefined type, directed type is assumed
	case Type::undefined:
		file << "\"directed\">\n";
		break;
	}

	// vertices data
	for (std::size_t i = 0; i < this->matrix.size(); i++)
	{
		file << "	<node id=\"n" << i;
		file << "\"/>\n";
	}

	// edge data including weight
	for (std::size_t i = 0; i < this->matrix.size(); i++)
	{
		for (std::size_t j = i; j < this->matrix[i].size(); j++)
		{
			if (this->matrix[i][j] > 0)
			{
				file << "	<edge source=\"n" << i;
				file << "\" target=\"n" << j;
				file << "\">\n";
				file << "		<data key=\"d0\">" + std::to_string(this->matrix[i][j]) + "</data>\n";
				file << "	</edge>\n";
			}
		}
	}

	// close the tags and the file
	file << "	</graph>\n";
	file << "</graphml>";
	file.close();
	
}




/*
	Function creating a line graph of an existing graph. The resulting
	line graph is returned as a different object. Resulting line graph
	is undirected and does not contain any weights.

	Params:
	None

	Return:
	return - line graph object
*/
Graph::Matrix Graph::Matrix::change_to_line_graph()
{
	std::vector<Data::coord> edges;

	// gather all the edges from the adjacency matrix of initial graph
	for (std::size_t i = 0; i < this->matrix.size(); i++)
	{
		for (std::size_t j = i; j < this->matrix[i].size(); j++)
		{
			if (this->matrix[i][j] > 0)
			{
				edges.push_back({ i, j });
			}
		}
	}

	// create sufficient matrix for the line graph and fill it with zeros
	std::vector<std::vector<int32_t>> mat;
	std::vector<int32_t> v;

	std::size_t size = edges.size();
	for (std::size_t i = 0; i < size; i++)
	{
		v.clear();
		for (std::size_t j = 0; j < size; j++)
		{
			v.push_back(0);
		}
		mat.push_back(v);
	}

	Data::coord coordinates;
	std::size_t index;

	// fill the line graph
	for (std::size_t i = 0; i < size; i++)
	{
		// load coordinates of the edge
		coordinates = edges[i];

		// search through the matrix for common vertices between edges
		// in the same row of the initial adjacency matrix
		for (std::size_t k = coordinates.x; k < this->matrix.size(); k++)
		{
			if (this->matrix[coordinates.x][k] > 0 && k != coordinates.y)
			{
				index = Data::find_index(edges, { coordinates.x, k });
				mat[i][index] = 1;
				mat[index][i] = 1;
			}
		}

		// search through the matrix for common vertices between edges
		// in the same column of the initial adjacency matrix
		for (std::size_t k = coordinates.x; k < coordinates.y; k++)
		{
			if (this->matrix[k][coordinates.y] > 0 && k != coordinates.x)
			{
				index = Data::find_index(edges, { k, coordinates.y });
				mat[i][index] = 1;
				mat[index][i] = 1;
			}
		}
	}

	// create and return the line graph object based on created matrix
	return Matrix(mat, this->name, this->type);
}




/*
	Function loading the throughtput matrix for the modified
	Belman-Ford algorithm, and stores it within the matrix object. 
	The throughtput matrix ought to be in a .mat format. The user
	must make sure that the throughtput matrix dimentions are the same
	as the adjacency matrix. This function can be used multiple times
	to update the throughtput matrix for the same graph, without need of
	creating a separate object.

	Params:
	file_path - path to the throughtput matrix file

	Return:
	none
*/
void Graph::Matrix::load_throughtput(std::string file_path)
{
	int32_t value;
	std::vector<int32_t> v;

	// check for the right file format
	if (file_path.find(".mat") != std::string::npos)
	{
		// if the file is in correct format, open it
		std::ifstream file(file_path);

		if (file.good())
		{
			// read each value from the throughtput matrix
			for (std::size_t i = 0; i < this->matrix.size(); i++)
			{
				v.clear();
				for (std::size_t j = 0; j < this->matrix[i].size(); j++)
				{
					file >> value;
					v.push_back(value);
				}
				// save the row of the throughtput matrix to the object
				this->throughtput.push_back(v);
			}
		}
		// if file could not be opened
		else
		{
			throw std::runtime_error("Could not open file");
		}
		file.close();
	}
	// if file is in incorrect format
	else
	{
		throw std::invalid_argument("File not supported");
	}
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
	std::vector<int32_t> v;

	for (std::size_t i = 0; i < vertices; i++)
	{
		v.clear();
		for (std::size_t j = 0; j < vertices; j++)
		{
			file >> temp;
			v.push_back(temp);
		}
		this->matrix.push_back(v);
	}
}




/*
	Function loads the data from .GRAPHML file. Does not support weighted graphs

	Params:
	file	- reference to the data source file

	Return:
	None
*/

void Graph::Matrix::load_graphml_file(std::fstream& file)
{
	std::string line;
	std::string line2;

	// set the default position of graph ID
	std::size_t pos = std::string::npos;
	std::size_t pos2 = std::string::npos;

	std::string weight_id;

	// search for graph ID and weight marker ID
	while (pos == std::string::npos)
	{
		std::getline(file, line);
		line2 = line;
		// search for the key marker
		pos = line.find("key");
		if (pos != std::string::npos)
		{
			// search for the weight keyword
			pos2 = line.find("weight");
			// if the keyword was not found, check the next line
			if (pos2 == std::string::npos)
			{
				std::getline(file, line2);
				pos2 = line2.find("weight");
			}
			// if the keyword was found, save the ID, else skip
			if (pos2 != std::string::npos)
			{
				weight_id = line.substr(pos + 8, 2);
			}
		}
		pos = line2.find("graph id=");
	}

	// search for the edge type
	pos2 = line.find("edgedefault=");

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
	std::vector<int32_t> v;

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

	int weight;
	std::string weight_str;

	// extract edge info
	while (pos != std::string::npos)
	{
		// find and extract the source and target nodes IDs
		pos = line.find("source=");
		pos2 = line.find("\" target=");

		id1 = line.substr(pos + 9, pos2 - (pos + 9));
		pos = line.find("\">");
		id2 = line.substr(pos2 + 11, pos - (pos2 + 11));

		// if the weight is specified, look for it
		if (!weight_id.empty())
		{
			pos = line.find("data");
			pos2 = line.find(weight_id);
			// search this and next line for the right data tag
			do {
				std::getline(file, line);
				pos = line.find("data");
				pos2 = line.find(weight_id);
			} while (pos != std::string::npos && pos2 == std::string::npos);

			// if data tags ended, assume the weight as 1
			if (pos == std::string::npos)
			{
				weight = 1;
			}
			// else extract the right weight
			else
			{
				pos = line.find("<", pos2);
				weight = atoi(line.substr(pos2 + 4, pos - (pos2 + 4)).c_str());
				// skip the </edge> closing tag and load the next edge tag
				std::getline(file, line);
				std::getline(file, line);
			}
		}
		// if no weight data ID was given, assume all weights as 1
		else
		{
			weight = 1;
		}

		// input the connection into matrix
		this->matrix[atoi(id1.c_str())][atoi(id2.c_str())] = weight;

		// if graph is undirected, make the connection both ways
		if (this->type == Type::undirected)
		{
			this->matrix[atoi(id2.c_str())][atoi(id1.c_str())] = weight;
		}

		// search for next edge marker
		pos = line.find("edge");
		if (pos == std::string::npos || line[pos - 1] == '/')
		{
			std::getline(file, line);
		}
		pos = line.find("edge");
	}
}




/*
	Calculate the degrees of each vertex.

	Params:
	None

	Return:
	None
*/

void Graph::Matrix::calculate_degrees()
{
	std::size_t deg;

	// iterate through the whole adjacency matrix to calculate degree of each node
	for (auto itr = this->matrix.begin(); itr != this->matrix.end(); itr++)
	{
		deg = 0;
		for (auto itr2 = itr->begin(); itr2 != itr->end(); itr2++)
		{
			// if connection was found, increase the degree
			if (*itr2 == 1)
			{
				// if a loop was found, increase the degree by 2
				if (std::distance(itr->begin(), itr2) == std::distance(this->matrix.begin(), itr))
				{
					deg += 2;
				}
				// not a loop
				else
				{
					deg += 1;
				}
			}
		}
	}
}
