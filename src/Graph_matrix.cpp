#include "../inc/Graph_matrix.h"

#include <stdexcept>
#include <iostream>
#include <iomanip>

#include "../lib/rapidxml/rapidxml.hpp"
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
Graph::Matrix::Matrix(std::string file_path, Type type)
	: type(type)
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




/**
 * \brief Constructor creating a graph based on the given matrix made up of vectors.
 * 
 * \param mat Vector of vectors containing the adjacency matrix values (weights of the connections).
 * \param name Name of the graph (user-given).
 * \param type Type of the graph (from the Graph::Type enum).
 */
Graph::Matrix::Matrix(std::vector<std::vector<uint32_t>>& mat, Type type)
	: matrix(mat),
	type(type)
{
	this->calculate_degrees();
}




/**
 * \brief Simple copy constructor allowing for creating a deep copy from lvalue reference.
 * 
 * \param m lvalue reference to the copied Graph::Matrix object.
 */
Graph::Matrix::Matrix(Matrix& m)
	: matrix(m.matrix),
	type(m.type),
	degrees(m.degrees),
	throughtput(m.throughtput)
{
}

/**
 * \brief Simple move constructor allowing for creating a deep copy from rvalue reference.
 * 
 * The move constructor causes the rvalue referenced object to be emptied after the copy is performed.
 * 
 * \param m rvalue reference to the copied Graph::Matrix object.
 */
Graph::Matrix::Matrix(Matrix&& m) noexcept
	: matrix(m.matrix),
	type(m.type),
	degrees(m.degrees),
	throughtput(m.throughtput)
{
	m.matrix.clear();
	m.degrees.clear();
	m.throughtput.clear();
}




/**
 * \brief Function displaying the information regarding current graph on standard output.
 * 
 * The function displays following information:
 * - type of the graph
 * - graph structure along with weights in form of adjacency matrix
 * - degrees of each of the vertices
 * 
 * \note Based on the type of the graph, the degree is presented either as a single value for undirected graph,
 *		 or as a indegree|outdegree pair for directed graph.
 */
void Graph::Matrix::print()
{
	// display type information
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

	for (std::size_t index = 0; auto row : this->matrix)
	{
		for (auto itr2 = row.begin(); itr2 != row.end(); itr2++)
		{
			std::cout << std::setw(3) << std::right << *itr2 << ", ";
		}
		if (this->type == Type::undirected)
		{
			std::cout << "  degree: " << this->degrees[index].deg << std::endl;
		}
		else
		{
			std::cout << "  degrees: (in|out) " << this->degrees[index].in_deg
				<< " | " << this->degrees[index].out_deg << std::endl;
		}
		index++;
	}

	std::cout << "]" << std::endl;
}




/**
 * \brief Function for adding a weighted edge between the source and destination vertices.
 * 
 * In case of undirected graph, this function will insert the edge both ways.
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
	// validate the parameters
	if (source >= this->matrix.size() || destination >= this->matrix.size())
	{
		throw std::out_of_range("Index out of bounds");
	}
	if (weight == 0)
	{
		throw std::invalid_argument("Weight equal to zero");
	}
	// save the previous weight for degree calculation
	int32_t previous_weight = this->matrix[source][destination];
	
	// update the weight of the connection
	this->matrix[source][destination] = weight;

	// if graph type is undirected, mark the connection both ways
	if (this->type == Type::undirected)
	{
		this->matrix[destination][source] = weight;
	}

	// update the degrees of given vertices
	if (previous_weight == 0)
	{
		this->degrees[source].out_deg++;
		this->degrees[destination].in_deg++;

		if (this->type == Type::undirected)
		{
			if (source == destination)
			{
				this->degrees[source].deg += 2;
			}
			else
			{
				this->degrees[source].deg++;
				this->degrees[destination].deg++;
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

	// add new row to the matrix
	this->matrix.push_back({});

	// fill the new row with zeros
	std::size_t index = this->matrix.size() - 1;

	for (std::size_t i = 0; i < size; i++)
	{
		this->matrix[index].push_back(0);
	}

	// add new vertex to the degrees vector
	this->degrees.push_back({ 0, 0, 0 });
}




/**
 * \brief Function for removing an edge between two given vertices.
 * 
 * If the graph is undirected the edge will be removed both ways.
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
	
	this->matrix[source][destination] = 0;

	// if graph type is undirected, mark the connection both ways
	if (this->type == Type::undirected)
	{
		this->matrix[destination][source] = 0;

		// update the undirected degree indicator for given vertices
		if (source == destination)
		{
			this->degrees[source].deg -= 2;
		}
		else
		{
			this->degrees[source].deg--;
			this->degrees[destination].deg--;
		}
	}

	// update the directed degrees indicators for given vertices
	this->degrees[source].out_deg--;
	this->degrees[destination].in_deg--;
}




/**
 * \brief Function for removing a vertex along with all its connections from the graph structure.
 * 
 * \param node_id ID of the removed vertex (counting from 0).
 * 
 * \warning Removing a vertex causes re-enumeration of each subsequent vertex in the graph, by decreasing
 *			their indexes by 1.
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

	// remove the column of the deleted vertex and update degree of each vertex if necessary
	for (std::size_t i = 0; i < this->matrix.size(); i++)
	{
		// find and decrease all degrees of vertices that have an incoming edge from the deleted vertex
		if (i == node_id)
		{
			for (std::size_t j = 0; j < this->matrix[i].size(); j++)
			{
				if (this->matrix[i][j] != 0)
				{
					this->degrees[j].in_deg--;

					// as this vertex will be deleted, loops can be ommited
					if (this->type == Type::undirected && i != j)
					{
						this->degrees[j].deg--;
					}
				}
			}
		}
		// find and decrease all degrees of vertices that have an outcoming edge to the deleted vertex
		else if (this->matrix[i][node_id] != 0)
		{	
			this->degrees[i].out_deg--;
			
			if (this->type == Type::undirected)
			{
				this->degrees[i].deg--;
			}
		}
		
		// remove the column and degree structure of deleted vertex
		this->matrix[i].erase(std::next(this->matrix[i].begin(), node_id));
	}

	// remove the row of the deleted vertex and remove the degree counter
	// for the deleted vertex
	this->matrix.erase(std::next(this->matrix.begin(), node_id));
	this->degrees.erase(std::next(this->degrees.begin(), node_id));
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
const Graph::Degree Graph::Matrix::get_node_degree(std::size_t node_id)
{
	if (node_id >= this->degrees.size())
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
 * \brief Getter for the type of the graph.
 * 
 * \return Graph::Type enum defining the type of the graph.
 */
const Graph::Type Graph::Matrix::get_type()
{
	return this->type;
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
void Graph::Matrix::save_graphml(std::ostream& stream, std::string name)
{
	// header
	stream << "<?xml version=\"1.0\"";
	stream << " encoding=\"UTF-8\"?>\n";

	// xml schema
	stream << "<graphml xmlns=";
	stream << "\"http://graphml.graphdrawing.org/xmlns\"\n";
	stream << "	xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
	stream << "	xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n";
	stream << "	http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";

	// weight data info
	stream << "\t<key id=\"d0\" for=\"edge\"\n";
	stream << "\t\tattr.name=\"weight\" attr.type=\"integer\"/>\n";

	// graph type data
	stream << "\t<graph id=";
	stream << "\"" + name + "\"";
	stream << " edgedefault=";
	switch (this->type)
	{
	case Type::directed:
		stream << "\"directed\">\n";
		break;
	case Type::undirected:
		stream << "\"undirected\">\n";
		break;
	}

	// vertices data
	for (std::size_t i = 0; i < this->matrix.size(); i++)
	{
		stream << "\t\t<node id=\"n" << i;
		stream << "\"/>\n";
	}

	// edge data including weight
	for (std::size_t i = 0; i < this->matrix.size(); i++)
	{
		for (std::size_t j = 0; j < this->matrix[i].size(); j++)
		{
			if (this->matrix[i][j] != 0)
			{
				if (this->type == Type::undirected && j < i)
				{
					continue;
				}
				stream << "\t\t<edge source=\"n" << i;
				stream << "\" target=\"n" << j;
				stream << "\">\n";
				stream << "\t\t\t<data key=\"d0\">" + std::to_string(this->matrix[i][j]) + "</data>\n";
				stream << "\t\t</edge>\n";
			}
		}
	}

	// close the tags and the file
	stream << "\t</graph>\n";
	stream << "</graphml>" << std::flush;
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
			if (this->matrix[i][j] != 0)
			{
				edges.push_back({ i, j });
			}
		}
	}

	// create sufficient matrix for the line graph and fill it with zeros
	std::vector<std::vector<uint32_t>> mat;

	std::size_t size = edges.size();
	std::size_t index;

	for (std::size_t i = 0; i < size; i++)
	{
		mat.push_back({});
		index = mat.size() - 1;
		for (std::size_t j = 0; j < size; j++)
		{
			mat[index].push_back(0);
		}
	}

	Data::coord coordinates;

	// fill the line graph
	for (std::size_t i = 0; i < size; i++)
	{
		// load coordinates of the edge
		coordinates = edges[i];

		// search through the matrix for common vertices between edges
		// in the same row of the initial adjacency matrix
		for (std::size_t k = coordinates.x; k < this->matrix.size(); k++)
		{
			if (this->matrix[coordinates.x][k] != 0 && k != coordinates.y)
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
			if (this->matrix[k][coordinates.y] != 0 && k != coordinates.x)
			{
				index = Data::find_index(edges, { k, coordinates.y });
				mat[i][index] = 1;
				mat[index][i] = 1;
			}
		}
	}

	// create and return the line graph object based on created matrix
	Matrix matrix(mat, this->type);
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
 * the adjacency matrix. Other dimentions may result in an undefined behaviour.
 * 
 * \warning Exceptions to guard against:
 *		- std::invalid_argument - when the file has an unsupported extension.
 *		- std::runtime_error - when the file could not be accessed.
 */
void Graph::Matrix::load_throughtput(std::string file_path)
{
	int32_t value;
	//std::vector<int32_t> v;
	std::size_t index;

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
				// add a row to the throughtput matrix
				this->throughtput.push_back({});
				index = this->throughtput.size() - 1;

				// load each value to the added row
				for (std::size_t j = 0; j < this->matrix[i].size(); j++)
				{
					file >> value;
					this->throughtput[index].push_back(value);
				}
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
 * 
 * \warning Exception to guard against:
 *		- std::runtime_error - when loaded weight of the connection is less or equal to 0.
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
	int32_t temp;
	std::size_t index;

	for (std::size_t i = 0; i < vertices; i++)
	{
		this->matrix.push_back({});
		index = this->matrix.size();
		for (std::size_t j = 0; j < vertices; j++)
		{
			file >> temp;
			if (temp < 0)
			{
				throw std::runtime_error("Weight less than zero");
			}
			this->matrix[index - 1].push_back(static_cast<uint32_t>(temp));
		}
	}
}




/**
 * \brief Function loads the data from .GRAPHML file.
 * 
 * This function is an internal function and is not to be called directly by the user.
 * 
 * \param file Reference to the std::fstream data source file object.
 * 
 * \warning Exception to guard against:
 *		- std::runtime_error - When loaded weight of the connection is less or equal to 0.
 * 
 */
void Graph::Matrix::load_graphml_file(std::fstream& file)
{
	// create the document and nodes instances
	auto document = std::make_unique< rapidxml::xml_document<>>();

	// split the document into vector
	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	// parse the document
	document->parse<0>(&buffer[0]);

	// save the root node and key node
	auto root_node = document->first_node("graphml");
	auto weight_node = root_node->first_node("key");
	auto graph_node = root_node->first_node("graph");

	std::string weight_key = "";

	// acquire the weight key
	while (weight_node && strcmp(weight_node->first_attribute("attr.name")->value(), "weight") != 0)
	{
		weight_node = weight_node->next_sibling("key");
	}

	if (weight_node)
	{
		weight_key = weight_node->first_attribute("id")->value();
	}

	// parse the graph type
	std::string type = graph_node->first_attribute("edgedefault")->value();

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
		throw std::runtime_error("Unsupported graph type");
	}

	// obtain vertices count
	for (rapidxml::xml_node<>* vertex = graph_node->first_node("node"); vertex; vertex = vertex->next_sibling("node"))
	{
		this->matrix.push_back({});
	}

	for (auto itr = this->matrix.begin(); itr != this->matrix.end(); itr++)
	{
		for (std::size_t i = 0; i < this->matrix.size(); i++)
		{
			itr->push_back(0);
		}
	}

	// obtain edges
	std::string id1;
	std::string id2;
	uint32_t weight;

	std::size_t index1, index2;

	for (rapidxml::xml_node<>* edge = graph_node->first_node("edge"); edge; edge = edge->next_sibling("edge"))
	{
		// get the IDs
		id1 = edge->first_attribute("source")->value();
		id2 = edge->first_attribute("target")->value();

		// stip any letters at the beginning of IDs
		for (std::size_t i = 0; i < id1.size(); i++)
		{
			if (id1[i] >= '0' && id1[i] <= '9')
			{
				id1 = id1.substr(i);
				break;
			}
		}

		for (std::size_t i = 0; i < id2.size(); i++)
		{
			if (id2[i] >= '0' && id2[i] <= '9')
			{
				id2 = id2.substr(i);
			}
		}

		// obtain weight
		weight = 1;

		for (rapidxml::xml_node<>* key = edge->first_node("data"); key; key = key->next_sibling())
		{
			if (strcmp(key->first_attribute("key")->value(), weight_key.c_str()) == 0)
			{
				weight = atoi(key->value());
			}
		}
		index1 = static_cast<std::size_t>(atoi(id1.c_str()));
		index2 = static_cast<std::size_t>(atoi(id2.c_str()));
		
		this->matrix[index1][index2] = weight;

		if (this->type == Type::undirected && id1 != id2)
		{
			this->matrix[index2][index1] = weight;
		}
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
	// create the degrees table
	for (std::size_t i = 0; i < this->matrix.size(); i++)
	{
		this->degrees.push_back({ 0, 0, 0 });
	}

	// iterate through the whole adjacency matrix to calculate degree of each node
	for (std::size_t index1 = 0; auto row : this->matrix)
	{
		for (std::size_t index2 = 0; auto element : row)
		{
			// if connection was found, increase the degree
			if (element != 0)
			{
				this->degrees[index1].out_deg++;
				this->degrees[index2].in_deg++;

				// if a loop was found, increase the degree by 2
				if (this->type == Type::undirected)
				{
					if (index1 == index2)
					{
						this->degrees[index1].deg += 2;
					}
					else
					{
						this->degrees[index1].deg++;
					}
				}
			}
			index2++;
		}
		index1++;
	}
}
