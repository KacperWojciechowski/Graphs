#include "Graph_list.h"
#include "PixelMap.h"

#pragma warning(push, 0)
#include "rapidxml/rapidxml.hpp"
#pragma warning(pop)

#include <iomanip>
#include <fstream>
#include <cstring>
#include <ranges>
#include <algorithm>

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
	if (file_path.find(".lst") != std::string::npos)
	{
		std::ifstream file(file_path);
		if (file.good())
		{
			load_lst_file(file);
		}
		else
		{
			throw std::runtime_error("Could not open file");
		}
		file.close();

		calculate_degrees();
	}
	// loading the .GRAPHML file
	else if (file_path.find(".GRAPHML") != std::string::npos)
	{
		std::fstream file(file_path, std::ios::in);
		if (file.good())
		{
			load_graphml_file(file);
		}
		else
		{
			throw std::runtime_error("Could not open file");
		}
		file.close();

		calculate_degrees();
	}
	// signalizing the unsupported file format
	else
	{
		throw std::invalid_argument("File not supported");
	}
}




/**
 * \brief Constructor creating the Graph::List object based on any supported graph representation.
 *
 * \param matrix Reference to the Graph::Matrix object.
 */
Graph::List::List(const GraphBase& graph) noexcept
{
	// get general graph info
	type = graph.get_type();
	const std::size_t count = graph.get_nodes_amount();

	// build list
	for (std::size_t i = 0; i < count; i++)
	{
		list.emplace_back(0);
		degrees.emplace_back(graph.get_node_degree(i));

		for (std::size_t j = 0; j < graph.get_nodes_amount(); j++)
		{
			const std::size_t val = graph.get_edge(i, j);
			if (val != 0)
			{
				list[i].emplace_back( j, val );
			}
		}
	}
}




/**
 * Constructor creating a graph of neighbourhood in a pixel map.
 *
 * This constructor creates an adjacency list object representing the neighbourhood
 * of each non-wall field within a pixel map based on Data::PixelMap object. This
 * function treats all the connections between adjacent field within the map as bi-directional,
 * and as such, assumes the graph is undirected.
 *
 * \see Data::PixelMap for pixel map representation reference.
 *
 * \param map Reference to a pixel map representation.
 *
 * \ref create_list_from_pixelmap.cpp "Example of creating a list graph based on pixel map"
 */
Graph::List::List(const Data::PixelMap& map) noexcept
	: type(Graph::Type::undirected)
{
	const std::size_t columns = map.get_columns();
	const std::size_t rows = map.get_rows();

	std::vector<Data::Coord> vertices;

	// create a vector of each non-wall field coordinates within the map
	for (std::size_t i = 0; i < rows; i++)
	{
		for (std::size_t j = 0; j < columns; j++)
		{
			if (map.get_field({ i, j }) == Data::PixelMap::GAP)
			{
				vertices.emplace_back(i, j);
			}
		}
	}

	// auxiliary functions for clean code purpose
	// Function adding an element to the list of given vertex index.
	auto add_to_list = [this, &vertices](std::size_t index, std::size_t row, std::size_t col) -> void
	{
		list[index].emplace_back(Data::find_index(vertices, { row, col }), 1);
	};

	// Function checking whether an element is a non-wall field
	auto is_gap = [&map](std::size_t row, std::size_t col) -> bool
	{
		return map.get_field({ row, col }) == Data::PixelMap::GAP;
	};

	// build the list based on the found fields
	for (std::size_t index = 0; const auto & vertex : vertices)
	{
		list.emplace_back(0);

		// check upper neighbour
		if (vertex.row() > 0 && is_gap(vertex.row() - 1, vertex.col()))
		{
			add_to_list(index, vertex.row() - 1, vertex.col());
		}

		// check left neighbour
		if (vertex.col() > 0 && is_gap(vertex.row(), vertex.col() - 1))
		{
			add_to_list(index, vertex.row(), vertex.col() - 1);
		}

		// check lower neighbour
		if (vertex.row() + 1 < rows && is_gap(vertex.row() + 1, vertex.col()))
		{
			add_to_list(index, vertex.row() + 1, vertex.col());
		}

		// check right neighbour
		if (vertex.col() + 1 < columns && is_gap(vertex.row(), vertex.col() + 1))
		{
			add_to_list(index, vertex.row(), vertex.col() + 1);
		}

		index++;
	}

	calculate_degrees();
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
auto Graph::List::load_lst_file(std::istream& file) noexcept -> void
{
	std::string line;
	size_t pos;

	std::size_t amount = 0;

	// function extracting subsequent neighbouring vertices IDs
	auto extract_val = [&line, &pos]() -> std::size_t
	{
		const std::size_t val = static_cast<std::size_t>(std::stoul(line) - 1);
		pos = line.find(' ');
		if (pos != std::string::npos)
		{
			line = line.substr(pos + 1);
		}
		return val;
	};

	// build the adjacency list
	for (std::size_t index = 0; std::getline(file, line); index++)
	{
		if (line != "")
		{
			list.emplace_back(0);
			pos = 0;

			// omit the first identifier
			line = line.substr(line.find(' ') + 1);

			while (pos != std::string::npos)
			{
				list[index].emplace_back(extract_val(), 1);
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
auto Graph::List::load_graphml_file(std::istream& file) -> void
{
	// create the document and nodes instances
	auto document = std::make_unique< rapidxml::xml_document<>>();

	// split the document into vector
	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	buffer.emplace_back('\0');

	// parse the document
	document->parse<0>(&buffer[0]);

	// save the root node and key node
	auto root_node = document->first_node("graphml");
	auto weight_node = root_node->first_node("key");
	auto graph_node = root_node->first_node("graph");

	std::string weight_key = "";

	// acquire the weight key
	while (weight_node && std::strcmp(weight_node->first_attribute("attr.name")->value(), "weight") != 0)
	{
		weight_node = weight_node->next_sibling("key");
	}

	if (weight_node)
	{
		weight_key = weight_node->first_attribute("id")->value();
	}

	// parse the graph type
	std::string type_s = graph_node->first_attribute("edgedefault")->value();

	if (type_s == "undirected")
	{
		type = Type::undirected;
	}
	else if (type_s == "directed")
	{
		type = Type::directed;
	}
	else
	{
		throw std::runtime_error("Unsupported graph type");
	}

	// obtain vertices count
	for (rapidxml::xml_node<>* vertex = graph_node->first_node("node"); vertex; vertex = vertex->next_sibling("node"))
	{
		list.emplace_back(0);
	}

	// obtain edges
	std::string id1;
	std::string id2;
	int32_t weight;

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
			if (std::strcmp(key->first_attribute("key")->value(), weight_key.c_str()) == 0)
			{
				weight = std::atoi(key->value());
				if (weight <= 0)
				{
					throw std::runtime_error("Weight less or equal to 0");
				}
			}
		}

		index1 = static_cast<std::size_t>(std::stoi(id1));
		index2 = static_cast<std::size_t>(std::stoi(id2));

		list[index1].emplace_back( index2, weight );

		if (this->type == Type::undirected && id1 != id2)
		{
			list[index2].emplace_back( index1, weight );
		}
	}
}




/**
 * \brief Function for calculating the degrees of each vertex.
 *
 * This function is for internal use only.
 *
 */
auto Graph::List::calculate_degrees() noexcept -> void
{
	degrees.resize(list.size());

	// calculate the degree and out_degree of each vertex
	for (std::size_t i = 0; i < list.size(); i++)
	{
		for (const auto& element : list[i])
		{
			degrees[i].out_deg++;
			degrees[element.ID].in_deg++;

			if (element.ID == i)
			{
				degrees[i].deg += 2;
			}
			else
			{
				degrees[i].deg++;
			}
		}
	}
}




/**
 * \brief Simple copy constructor.
 *
 * \param l lvalue reference to the data source object.
 */
Graph::List::List(const List& l) noexcept
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
auto Graph::List::print() const noexcept -> void
{
	// display graph name and type info
	std::cout << "Type = ";

	switch (type)
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

	std::cout << '\n';

	// display vertices count
	std::cout << "Vertices = " << list.size() << '\n';

	std::cout << "{\n";

	for (std::size_t index = 0; const auto& element : list)
	{
		// display degrees
		if (type == Type::undirected)
		{
			std::cout << "degree: " << std::left << std::setw(6) << degrees[index].deg;
		}
		else
		{
			std::cout << "degrees: (in|out) " << degrees[index].in_deg << " | " << degrees[index].out_deg;
		}

		std::cout << ",    " << index << ": ";

		for (const auto& vertex : element)
		{
			std::cout << vertex.ID << " {" << vertex.weight << "}, ";
		}
		index++;
		std::cout << '\n';
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
 * \warning Exceptions to guard against:
 *		- std::out_of_range - one of the indexes is out of range
 *		- std::invalid_argument - weight is equal to zero
 *
 * \param source ID of the source vertex.
 * \param destination ID of the end vertex.
 * \param weight Weight of the connection.
 *
 * \ref add_edge_list_insert.cpp "Example of adding an edge between two vertices"\n
 * \ref add_edge_list_override.cpp "Example of modifying the weight of an existing edge"
 */
auto Graph::List::make_edge(std::size_t source, std::size_t destination, int32_t weight) -> void
{
	// validate arguments
	if (source >= list.size() || destination >= list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}
	if (weight == 0)
	{
		throw std::invalid_argument("Weight equal to zero");
	}

	// search for the iterator of connection, if added edge already exists
	auto edge_itr = std::ranges::find(list[source], destination, &Node::ID);

	// if iterator was found, override the weight of the connection
	if (edge_itr != list[source].end())
	{
		edge_itr->weight = weight;
	}
	// else insert the connection to the structure
	else
	{
		list[source].emplace_back( destination, weight );

		// calculate degrees
		degrees[source].out_deg++;
		degrees[source].deg++;
		degrees[destination].in_deg++;
	}

	// if the graph type is undirected
	if (type == Type::undirected)
	{
		// search for the iterator of mirrored connection
		edge_itr = std::ranges::find(list[destination], source, &Node::ID);

		// if iterator was found, override the weight of the connection
		if (edge_itr != list[destination].end())
		{
			edge_itr->weight = weight;
		}
		// else insert the mirrored connection to the structure
		else
		{
			list[destination].emplace_back( source, weight );
			degrees[destination].deg++;
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
auto Graph::List::add_node() noexcept -> void
{
	list.emplace_back(0);
	degrees.emplace_back( 0, 0, 0 );
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
auto Graph::List::remove_edge(std::size_t source, std::size_t destination) -> void
{
	// validate the parameter
	if (source >= list.size() || destination >= list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	// erase the edge
	for (auto itr = list[source].begin(); itr != list[source].end(); itr++)
	{
		if (itr->ID == destination)
		{
			list[source].erase(itr);

			degrees[source].out_deg--;
			degrees[source].deg--;
			degrees[destination].in_deg--;
			break;
		}
	}

	// if the graph is undirected, remove the mirrored edge
	if (type == Type::undirected)
	{
		for (auto itr = list[destination].begin(); itr != list[destination].end(); itr++)
		{
			if (itr->ID == source)
			{
				list[destination].erase(itr);

				degrees[destination].out_deg--;
				degrees[destination].deg--;
				degrees[source].in_deg--;
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
auto Graph::List::remove_node(std::size_t node_id) -> void
{
	// validate parameter
	if (node_id >= list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	// variables for vertex removal
	bool removed = false;
	std::size_t count = list.size();

	// iterators for quicker element access
	auto list_itr = list.begin();
	auto deg_itr = degrees.begin();

	// search through the list
	for (std::size_t i = 0; i < count; i++)
	{
		// remove the part of the list where the given vertex is the source of an edge
		if (!removed && i == node_id)
		{
			removed = true;

			// set default value for iterator
			list_itr = list.begin();
			deg_itr = degrees.begin();

			// advance iterator to get the desired one
			std::ranges::advance(list_itr, i);
			std::ranges::advance(deg_itr, i);

			// erase the values pointed to by iterators
			list.erase(list_itr);
			degrees.erase(deg_itr);

			count = list.size();
			i--;
		}
		// remove the parts of the list where the given vertex is the end of an edge
		else
		{
			for (auto itr2 = list[i].begin(); itr2 != list[i].end();)
			{
				// if given vertex was found
				if (itr2->ID == node_id)
				{
					// erase the vertex
					auto itr_tmp = itr2;
					itr2++;
					list[i].erase(itr_tmp);
					degrees[i].deg--;
					degrees[i].out_deg--;
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
 * \brief Function saving current graph structure  into a .GRAPHML format file.
 *
 * This format does contain the weights of the edges.
 *
 * \param stream Stream to which the graphML data format should be saved to.
 * \param name Name of the graph (user-given).
 *
 * \ref save_list_to_graphml.cpp "Example of saving current graph structure in .GRAPHML format"
 */
auto Graph::List::save_graphml(std::ostream& stream, std::string name) const noexcept -> void
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
	switch (type)
	{
	case Type::directed:
		stream << "\"directed\">\n";
		break;
	case Type::undirected:
		stream << "\"undirected\">\n";
		break;
	}

	// vertices data
	for (std::size_t i = 0; i < list.size(); i++)
	{
		stream << "\t\t<node id=\"n" << i;
		stream << "\"/>\n";
	}

	// edge data including weight
	for (std::size_t i = 0; i < list.size(); i++)
	{
		for (const auto& neighbour : list[i])
		{
			if (type == Type::undirected && neighbour.ID < i)
			{
				continue;
			}
			stream << "\t\t<edge source=\"n" << i;
			stream << "\" target=\"n" << neighbour.ID;
			stream << "\">\n";
			stream << "\t\t\t<data key=\"d0\">" + std::to_string(neighbour.weight) + "</data>\n";
			stream << "\t\t</edge>\n";
		}
	}

	// close the tags and the file
	stream << "\t</graph>\n";
	stream << "</graphml>" << std::flush;
}
