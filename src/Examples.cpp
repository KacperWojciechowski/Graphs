#include "..\inc\Examples.h"

#include "..\inc\Graph_matrix.h"
#include "..\inc\Graph_list.h"

#include <memory>

/**
 * \brief Example of creating a List object based on .lst file
 * 
 * \param graph_type Type of the graph based on the Graph::Type enum.
 */
void Graph::Examples::create_list_from_lst_file(Type graph_type)
{
	List list("samples\\list.lst", "Graph", graph_type);
	list.print();
}




/**
 * \brief Example of creating a Graph::List object based on .GRAPHML file
 * 
 * \param graph_type Type of the graph based on the Graph::Type enum.
 */
void Graph::Examples::create_list_from_graphml_file(Type graph_type)
{
	std::unique_ptr<List> list;

	if (graph_type == Type::undirected)
	{
		list = std::make_unique<List>("samples\\graphML.GRAPHML", "Name", graph_type);
		list->print();
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", "Name", graph_type);
		list->print();
	}
	else
	{
		std::cout << "Choose either Graph::directed or Graph::undirected" << std::endl;
	}
}




/**
 * \brief Example of saving an existing Graph::List graph representation to .GRAPHML file.
 * 
 * \note This example bases on .GRAPHML file sources to allow user to compare the results
 *		 with given samples.
 * 
 * \param graph_type Type of the graph based on the Graph::Type enum.
 */
void Graph::Examples::save_list_to_graphml_file(Type graph_type)
{
	std::unique_ptr<List> list;

	if (graph_type == Type::undirected)
	{
		list = std::make_unique<List>("samples\\graphML.GRAPHML", "Name", graph_type);
		list->print();
		list->save_graphml("samples\\saved_graphML.GRAPHML");
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", "Name", graph_type);
		list->print();
		list->save_graphml("samples\\saved_graphML.GRAPHML");
	}
	else
	{
		std::cout << "Choose either Graph::directed or Graph::undirected" << std::endl;
	}

}




/**
 * \brief Example of adding an edge to the graph structure in Graph::List object.
 * 
 * \param graph_type Type of the graph based on Graph::Type enum.
 */
void Graph::Examples::add_edge_list(Type graph_type)
{
	std::unique_ptr<List> list;

	std::cout << "This example adds an edge from 0 to 2, and overrides the edge from 0 to 1." << std::endl;

	if (graph_type == Type::undirected)
	{
		list = std::make_unique<List>("samples\\graphML.GRAPHML", "Name", graph_type);
		list->print();
		list->add_edge(0, 1, 2);
		list->add_edge(0, 2, 7);
		list->print();
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", "Name", graph_type);
		list->print();
		list->add_edge(0, 1, 2);
		list->add_edge(0, 2, 7);
		list->print();
	}
	else
	{
		std::cout << "Choose either Graph::directed or Graph::undirected" << std::endl;
	}
}




/**
 * \brief Example of adding a vertex to the graph structure in Graph::List object.
 * 
 * \param graph_type Type of the graph based on the Graph::Type enum.
 */
void Graph::Examples::add_node_list(Type graph_type)
{
	std::unique_ptr<List> list;

	if (graph_type == Type::undirected)
	{
		list = std::make_unique<List>("samples\\graphML.GRAPHML", "Name", graph_type);
		list->print();
		list->add_node();
		list->print();
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", "Name", graph_type);
		list->print();
		list->add_node();
		list->print();
	}
	else
	{
		std::cout << "Choose either Graph::directed or Graph::undirected" << std::endl;
	}
}




/**
 * \brief Example of removing an edge from graph structure in Graph::List object.
 * 
 * \param graph_type Type of the graph based on Graph::Type enum.
 */
void Graph::Examples::remove_edge_list(Type graph_type)
{
	std::unique_ptr<List> list;

	if (graph_type == Type::undirected)
	{
		list = std::make_unique<List>("samples\\graphML.GRAPHML", "Name", graph_type);
		list->print();
		list->remove_edge(0, 1);
		list->print();
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", "Name", graph_type);
		list->print();
		list->remove_edge(0, 1);
		list->print();
	}
	else
	{
		std::cout << "Choose either Graph::directed or Graph::undirected" << std::endl;
	}
}




/**
 * \brief Example of removing a vertex from graph structure in Graph::List object.
 * 
 * \param graph_type Type of the graph based on Graph::Type enum.
 */
void Graph::Examples::remove_node_list(Type graph_type)
{
	std::unique_ptr<List> list;

	if (graph_type == Type::undirected)
	{
		list = std::make_unique<List>("samples\\graphML.GRAPHML", "Name", graph_type);
		list->print();
		list->remove_node(0);
		list->print();
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", "Name", graph_type);
		list->print();
		list->remove_node(0);
		list->print();
	}
	else
	{
		std::cout << "Choose either Graph::directed or Graph::undirected" << std::endl;
	}
}
