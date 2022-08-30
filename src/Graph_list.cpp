#include "..\inc\Graph_list.h"
#include "..\lib\rapidxml\rapidxml.hpp"

#include <iomanip>

/**
 * \brief Constructor creating the graph from file.
 * 
 * The constructor accepts the path to a file in supported format. If format is incorrect
 * or the file cannot be accessed, an exception is returned. 
 * 
 * \param file_path Path of the data source file.
 * \param type Type of the graph (from the Graph::Type enum). In case of reading from graphML file
 *			   this parameter is ignored.
 * 
 * \warning Exceptions to guard against:
 *		- std::invalid_argument - when given file extension is not supported.
 *		- std::runtime_error	- when given file cannot be accessed.
 * \attention Supported formats are .lst and .GRAPHML. The .lst representation does not
 *			  contain weights, so a weight of 1 is assumed for each connection if using
 *			  this file format as a data source. 
 * 
 * \see <a href="https://hog.grinvin.org/Formats.action"> Here </a> for House of Graphs adjacency list format guideline.
 * \see <a href="http://graphml.graphdrawing.org/primer/graphml-primer.html"> Here </a> for GraphML format guideline.
 * 
 * \ref create_list_from_lst.cpp "Example of using .lst file as data source"\n 
 * \ref create_list_from_graphml.cpp "Example of using .GRAPHML file as data source"
 */
Graph::List::List(const std::string& file_path, Type type)
	: type(type)
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
 * \brief Constructor creating the adjacency list object based on the adjacency matrix object.
 * 
 * \param matrix Reference to the Graph::Matrix object.
 */
Graph::List::List(Matrix& matrix)
{
	// get general graph info
	this->type = matrix.get_type();
	
	// build list based on the matrix
	for (std::size_t i = 0; i < matrix.get_nodes_amount(); i++)
	{
		this->list.push_back({});

		for (std::size_t j = 0; j < matrix.get_nodes_amount(); j++)
		{
			if (matrix.get_edge(i, j) != 0)
			{
				this->list[i].push_back({ j, matrix.get_edge(i, j) });
			}
		}
	}

	this->calculate_degrees();
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
 * 
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

	// extract each of the adjacent vertices
	for (std::size_t i = 0; i < amount; i++)
	{
		std::getline(file, line);

		if (line != "")
		{
			this->list.push_back({});

			pos = line.find(' ');

			auto itr = std::next(this->list.begin(), this->list.size() - 1);

			while (pos != std::string::npos)
			{
				line = line.substr(pos + 1);
				vertex.ID = static_cast<std::size_t>(atoi(line.c_str())) - 1;
				vertex.weight = 1;
				itr->push_back(vertex);
				pos = line.find(' ');
			}
		}
	}
}




/**
 * \brief Function loading the data from .GRAPHML file.
 * 
 * \param file Reference to the file object of the data source.
 * 
 * \warning Exceptions to guard against:
 *		- std::runtime_error When weight is less or equal to 0.
 */
void Graph::List::load_graphml_file(std::fstream& file)
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
		this->list.push_back({});
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

		this->list[index1].push_back({ index2, weight});

		if (this->type == Type::undirected && id1 != id2)
		{
			this->list[index2].push_back({ index1, weight });
		}
	}
}




/**
 * \brief Function for calculating the degrees of each vertex.
 * 
 * This function is for internal use only.
 * 
 */
void Graph::List::calculate_degrees()
{
	// calculate the degree of each vertex
	for (std::size_t i = 0; i < this->list.size(); i++)
	{
		this->degrees.push_back({ 0, 0, 0 });
		for (auto itr = this->list[i].begin(); itr != this->list[i].end(); itr++)
		{
			this->degrees[i].out_deg++;

			if (itr->ID == i)
			{
				this->degrees[i].deg += 2;
			}
			else
			{
				this->degrees[i].deg++;
			}
		}
	}

	if (this->type == Type::undirected)
	{
		for (std::size_t i = 0; i < this->degrees.size(); i++)
		{
			this->degrees[i].in_deg = this->degrees[i].out_deg;
		}
	}
	else
	{
		for (std::size_t i = 0; i < this->degrees.size(); i++)
		{
			for (std::size_t j = 0; j < this->list.size(); j++)
			{
				for (auto itr = this->list[j].begin(); itr != this->list[j].end(); itr++)
				{
					if (itr->ID == i)
					{
						this->degrees[i].in_deg++;
						break;
					}
				}
			}
		}
	}
}




/**
 * \brief Simple copy constructor.
 * 
 * \param l lvalue reference to the data source object.
 */
Graph::List::List(List& l)
	: list(l.list),
	type(l.type),
	degrees(l.degrees)
{
}




/**
 * \brief Simple move constructor.
 * 
 * \param l rvalue reference to the data source object
 */
Graph::List::List(List&& l) noexcept
	: list(l.list),
	type(l.type),
	degrees(l.degrees)
{
	l.list.clear();
	l.degrees.clear();
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
 *	\note Data is displayed in format "degree: ____*, ID: adj_vert_ID {weight}, adj_vert_ID {weight}, ...",
 *		  but the degree field varies based on the graph type. In case of undirected graph, only one value is shown.
 *		  In case of directed graphs, indegree and outdegree is shown in format "in|out".
 * 
 */
void Graph::List::print()
{
	// display graph name and type info
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

	for (std::size_t index = 0; auto& element : this->list)
	{
		// display degrees
		if (this->type == Type::undirected)
		{
			std::cout << "degree: " << std::left << std::setw(6) << this->degrees[index].deg;
		}
		else
		{
			std::cout << "degrees: (in|out) " << this->degrees[index].in_deg << " | " << this->degrees[index].out_deg;
		}

		std::cout << ",    " << index << ": ";

		for (auto itr2 = element.begin(); itr2 != element.end(); itr2++)
		{
			std::cout << itr2->ID << " {" << itr2->weight << "}, ";
		}
		index++;
		std::cout << std::endl;
	}

	std::cout << "}" << std::endl;
}




/**
 * \brief Function adding a connection between two given vertices.
 * 
 * If the graph type is undirected, the connection will be inserted both ways.
 * 
 * \warning If an edge between given vertices already exists, the weight of the 
 *			connection will be overwritten, and second edge will not be added.
 * 
 * \param source ID of the source vertex.
 * \param destination ID of the end vertex.
 * \param weight Weight of the connection.
 * 
 * \ref add_edge_list_insert.cpp "Example of adding an edge between two vertices"\n
 * \ref add_edge_list_override.cpp "Example of modifying the weight of an existing edge"
 */
void Graph::List::make_edge(std::size_t source, std::size_t destination, uint32_t weight)
{
	// validate arguments
	if (source >= this->list.size() || destination >= this->list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}
	if (weight == 0)
	{
		throw std::invalid_argument("Weight equal to zero");
	}

	// search for the iterator of connection, if added edge already exists
	auto edge_itr = this->list[source].end();
	for (auto itr = this->list[source].begin(); itr != this->list[source].end(); itr++)
	{
		if (itr->ID == destination)
		{
			edge_itr = itr;
			break;
		}
	}

	// if iterator was found, override the weight of the connection
	if (edge_itr != this->list[source].end())
	{
		edge_itr->weight = weight;
	}
	// else insert the connection to the structure
	else
	{
		this->list[source].push_back({ destination, weight });
		
		// calculate degrees
		this->degrees[source].out_deg++;
		this->degrees[source].deg++;
		this->degrees[destination].in_deg++;
	}

	// if the graph type is undirected
	if (this->type == Type::undirected)
	{
		// search for the iterator of mirrored connection 
		edge_itr = this->list[destination].end();
		for (auto itr = this->list[destination].begin(); itr != this->list[destination].end(); itr++)
		{
			if (itr->ID == source)
			{
				edge_itr = itr;
				break;
			}
		}

		// if iterator was found, override the weight of the connection
		if (edge_itr != this->list[destination].end())
		{
			edge_itr->weight = weight;
		}
		// else insert the mirrored connection to the structure
		else
		{
			this->list[destination].push_back({ source, weight });
			this->degrees[destination].out_deg++;
			this->degrees[destination].deg++;
			this->degrees[source].in_deg++;
		}
	}
}




/**
 * \brief Function adding a new vertex to the graph structure.
 * 
 * Added vertex is initially isolated, so any connections to it need
 * to be added manually.
 * 
 * \see add_node_list.cpp "Example of adding an isolated vertex"
 */
void Graph::List::add_node()
{
	this->list.push_back({});
	this->degrees.push_back({ 0, 0, 0 });
}




/**
 * \brief Function removing a connection between two given adjacency vertices.
 * 
 * If the graph is undirected, the connection will be removed both ways.
 * 
 * \param source ID of the beginning vertex.
 * \param destination ID of the end vertex.
 * 
 * \warning Exceptions to guard against:
 *		- std::out_of_range - When any of the given IDs is out of bounds for the adjacency list.
 * 
 * \ref remove_edge_list.cpp "Example of removing an edge from graph structure"
 */
void Graph::List::remove_edge(std::size_t source, std::size_t destination)
{
	// validate the parameter
	if (source >= this->list.size() || destination >= this->list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	// erase the edge
	for (auto itr = this->list[source].begin(); itr != this->list[source].end(); itr++)
	{
		if (itr->ID == destination)
		{
			this->list[source].erase(itr);

			this->degrees[source].out_deg--;
			this->degrees[source].deg--;
			this->degrees[destination].in_deg--;
			break;
		}
	}

	// if the graph is undirected, remove the mirrored edge 
	if (this->type == Type::undirected)
	{
		for (auto itr = this->list[destination].begin(); itr != this->list[destination].end(); itr++)
		{
			if (itr->ID == source)
			{
				this->list[destination].erase(itr);

				this->degrees[destination].out_deg--;
				this->degrees[destination].deg--;
				this->degrees[source].in_deg--;
				break;
			}
		}
	}
}




/**
 * \brief Function for removing a vertex from the graph structure.
 * 
 * \param node_id ID of the vertex to be removed from the graph structure.
 * 
 * \warning Removal of a vertex causes the re-enumeration of each subsequent vertex,
 *			by decreasing their indexes by 1.
 * 
 * \warning Exception to guard against:
 *			- std::out_of_range - when given vertex ID is out of bounds for the list.
 * 
 * \ref remove_node_list.cpp "Example of removing a vertex from graph structure"
 */
void Graph::List::remove_node(std::size_t node_id)
{
	// validate parameter
	if (node_id >= this->list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	// variables for vertex removal
	bool removed = false;
	std::list<Node>::iterator itr_tmp;
	std::size_t count = this->list.size();

	// search through the list
	for (std::size_t i = 0; i < count; i++)
	{
		// remove the part of the list where the given vertex is the source of an edge
		if (!removed && i == node_id)
		{
			removed = true;
			this->list.erase(std::next(this->list.begin(), i));
			this->degrees.erase(std::next(this->degrees.begin(), i));
			count = this->list.size();
			i--;
		}
		// remove the parts of the list where the given vertex is the end of an edge
		else
		{
			for (auto itr2 = this->list[i].begin(); itr2 != this->list[i].end();)
			{
				// if given vertex was found
				if (itr2->ID == node_id)
				{
					// erase the vertex
					itr_tmp = itr2;
					itr2++;
					this->list[i].erase(itr_tmp);
					this->degrees[i].deg--;
					this->degrees[i].out_deg--;
					continue;
				}
				// if vertex of higher ID than the given one was found
				else if (itr2->ID > node_id)
				{
					itr2->ID--;
					
				}
				// move to the next vertex
				itr2++;
			}
		}
	}
}




/**
 * \brief Getter for vertices amount.
 * 
 * \return Amount of vertices as const.
 */
std::size_t Graph::List::get_nodes_amount()
{
	return this->list.size();
}




/**
 * \brief Getter for vertex degree.
 * 
 * \param node_id ID of the vertex which degree should be returned.
 * \return Degree of the vertex as const.
 */
Graph::Degree Graph::List::get_node_degree(std::size_t node_id)
{
	// validate the parameter
	if (node_id >= this->degrees.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	// return the degree value
	return this->degrees[node_id];
}




/**
 * \brief Getter for the weight of the edge between two given vertices.
 * 
 * \note A return value of zero means there is no edge between given vertices.
 * 
 * \param source ID of the beginning vertex of the edge.
 * \param destination ID of the end vertex of the edge.
 * \return Weight of the connection.
 */
uint32_t Graph::List::get_edge(std::size_t source, std::size_t destination)
{
	uint32_t ret = 0;

	if (source >= this->list.size() || destination >= this->list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}
	for (auto itr = this->list[source].begin(); itr != this->list[source].end(); itr++)
	{
		if (itr->ID == destination)
		{
			ret = itr->weight;
			break;
		}
	}

	return ret;
}




/**
 * \brief Getter for the type of the graph.
 * 
 * \return The type of the graph as const.
 */
Graph::Type Graph::List::get_type()
{
	return this->type;
}




/**
 * \brief Function saving current graph structure  into a .GRAPHML format file.
 * 
 * This format does contain the weights of the edges.
 * 
 * \param stream Stream to which the graphML data format should be saved to.
 * \param name Name of the graph (user-given).
 * 
 * \ref save_list_to_graphml.cpp "Example of saving current graph structure in .GRAPHML format"
 */
void Graph::List::save_graphml(std::ostream& stream, std::string name)
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
	for (std::size_t i = 0; i < this->list.size(); i++)
	{
		stream << "\t\t<node id=\"n" << i;
		stream << "\"/>\n";
	}

	// edge data including weight
	for (std::size_t i = 0; i < this->list.size(); i++)
	{
		for (auto itr = this->list[i].begin(); itr != this->list[i].end(); itr++)
		{
			if (this->type == Type::undirected && itr->ID < i)
			{
				continue;
			}
			stream << "\t\t<edge source=\"n" << i;
			stream << "\" target=\"n" << itr->ID;
			stream << "\">\n";
			stream << "\t\t\t<data key=\"d0\">" + std::to_string(itr->weight) + "</data>\n";
			stream << "\t\t</edge>\n";
		}
	}

	// close the tags and the file
	stream << "\t</graph>\n";
	stream << "</graphml>" << std::flush;
}
