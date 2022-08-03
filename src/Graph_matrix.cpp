#include "../inc/Graph_matrix.h"

#include <stdexcept>
#include <iostream>
#include <iomanip>

#include "../inc/Coord.h"

/**
 * \brief Constructor loading the graph from file.
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
 * \attention Supported formats are .mat and .GRAPHML. 
 * \see House of Graphs for .mat adjacency matrix file reference.
 */
Graph::Matrix::Matrix(std::string file_path, std::string name, Type type)
	: name(name),
	  type(type),
	  next_index(0)
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

		// create enumeration map
		for (auto i = 0; i < this->matrix.size(); i++)
		{
			this->numeration.insert(std::pair<std::size_t, std::size_t>(this->next_index++, i));
		}
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

		// create enumeration map
		for (auto i = 0; i < this->matrix.size(); i++)
		{
			this->numeration.insert(std::pair<std::size_t, std::size_t>(this->next_index++, i));
		}
	}
	// signalizing the unsupported file format 
	else
	{
		throw std::invalid_argument("File not supported");
	}
}




/**
 * \brief Constructor creating a graph based on the given matrix made up of vectors.
 * 
 * \param mat Vector of vectors containing the adjacency matrix values (weights of the connections).
 * \param name Name of the graph (user-given).
 * \param type Type of the graph (from the Graph::Type enum).
 */
Graph::Matrix::Matrix(std::vector<std::vector<int32_t>>& mat, std::string name, Type type)
	: matrix(mat),
	name(name),
	type(type),
	next_index(0)
{
	// create enumeration map
	for (auto i = 0; i < this->matrix.size(); i++)
	{
		this->numeration.insert(std::pair<std::size_t, std::size_t>(this->next_index++, i));
	}
}




/**
 * \brief Simple copy constructor allowing for creating a deep copy from rvalue reference.
 * 
 * The constructor performs a deep copy of each of the private class members.
 * 
 * \param m rvalue reference to the copied Graph::Matrix object.
 */
Graph::Matrix::Matrix(Matrix&& m) noexcept
	: matrix(m.matrix),
	name(m.name),
	type(m.type),
	next_index(m.next_index),
	numeration(m.numeration)
{
	
}




/**
 * \brief Function displaying the information regarding current graph on standard output.
 * 
 * The function displays following information:
 * - name of the graph
 * - type of the graph
 * - graph structure along with weights in form of adjacency matrix
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




/**
 * \brief Function for adding a weighted edge between the source and destination vertices.
 * 
 * \param source ID of the source vertex of the edge.
 * \param destination ID of the end vertex of the edge.
 * \param weight Weight of inserted edge.
 * 
 * \warning Exceptions to guard against:
 *		- std::out_of_range - when either of the IDs exceed the count of vertices.
 *		- std::invalid_argument - when the weight value is equal to 0.
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




/**
 * \brief Function adding an isolated vertex to the graph structure.
 * 
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




/**
 * \brief Function for removing an edge between two given vertices.
 * 
 * \param source ID of the source vertex of the edge (counting from 0).
 * \param destination ID of the destination vertex of the edge (counting from 0).
 * 
 * \warning Exception to guard against:
 *		- std::out_of_range - when either of the IDs exceed the count of vertices.
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




/**
 * \brief Function for removing a vertex along with all its connections from the graph structure.
 * 
 * \param node_id ID of the removed vertex (counting from 0).
 * 
 * \warning Exception to guard against:
 *		- std::out_of_range - when given ID exceeds the count of vertices.
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




/**
 * \brief Getter for vertices count.
 * 
 * \return Amount of vertices in the graph structure.
 */
const std::size_t Graph::Matrix::get_nodes_amount()
{
	return this->matrix.size();
}




/**
 * \brief Getter for the degree of given vertex.
 * 
 * \param node_id ID of the vertex which degree should be returned.
 * \return Degree of the given vertex
 * 
 * \warning Exception to guard against:
 *		- std::out_of_range - when given ID exceeds the count of vertices.
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




/**
 * \brief Getter for the weight of a connection between given vertices.
 * 
 * \param source ID of the source vertex of the edge (counting from 0).
 * \param destination ID of the end vertex of the edge (counting from 0).
 * \return Value of the weight of the edge.
 * 
 * \note Return value of zero means no connection is present.
 * 
 * \warning Exception to guard against:
 *		- std::out_of_range - when either of the IDs exceeds the count of vertices.
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




/**
 * \brief Function generating a .GRAPHML file containing the graph information.
 * 
 * This format does contain the weights of the graph's edges.
 * 
 * \note In case of undefined graph type, function assumes the graph is directed.
 * 
 * \param output_file_path Path to the output file
 */
void Graph::Matrix::save_graphml(std::string output_file_path)
{
	// opening output file
	std::ofstream file(output_file_path);

	// header
	file << "<?xml version=\"1.0\"";
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
		for (std::size_t j = 0; j < this->matrix[i].size(); j++)
		{
			if (this->matrix[i][j] > 0)
			{
				if (this->type == Type::undirected && j < i)
				{
					continue;
				}
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




/**
 * \brief Function for creating a line graph of the current graph.
 * 
 * The resulting line graph is returned as a separate object. 
 * 
 * \warning Resulting line graph is undirected and does not contain any weights (all weights
 * are defaulted to 1). As such, the functionality currently does not support digraphs. 
 * 
 * \return Graph::Matrix object containing the line graph.
 * 
 * \todo Modify the function to support directed graphs.
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
	Matrix matrix(mat, this->name, this->type);
	return matrix;
}




/**
 * \brief Function loading the throughtput matrix for the modified Belman-Ford algorithm.
 * 
 * The loaded throughtput matrix ought to be in a .mat format, and is stored within the current 
 * object. This function can be called multiple times to update the throughtput matrix for the
 * same graph, without need of creating a separate object.
 * 
 * \param file_path Path to the throughtput matrix .mat file.
 * 
 * \attention The user has to make sure that the throughtput matrix is just as big as
 * the adjacency matrix.
 * 
 * \warning Exceptions to guard against:
 *		- std::invalid_argument - when the file has an unsupported extension.
 *		- std::runtime_error - when the file could not be accessed.
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




/**
 * \brief Function loads the data from .mat file into the matrix object.
 * 
 * This function is an internal function and is not to be called directly by the user.
 * 
 * \param file Reference to the std::fstream data source file object.
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




/**
 * \brief Function loads the data from .GRAPHML file.
 * 
 * This function is an internal function and is not to be called directly by the user.
 * 
 * \param file Reference to the std::fstream data source file object.
 * 
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




/**
 * \brief Function calculating the degrees of each vertex in the graph structure.
 * 
 * This function is for internal purpose and is not to be called directly by the user.
 * 
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
