#include "..\inc\Examples.h"

#include "..\inc\Graph_matrix.h"
#include "..\inc\Graph_list.h"
#include "..\inc\Generator.h"

#include <memory>


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
		list = std::make_unique<List>("samples\\graphML.GRAPHML", graph_type);
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", graph_type);
	}
	else
	{
		std::cerr << "Choose either Graph::directed or Graph::undirected" << std::endl;
		return;
	}
	list->print();
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

	std::ofstream file("samples\\saved_graphML.GRAPHML");

	if (graph_type == Type::undirected)
	{
		list = std::make_unique<List>("samples\\graphML.GRAPHML", graph_type);
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", graph_type);
	}
	else
	{
		std::cerr << "Choose either Graph::directed or Graph::undirected" << std::endl;
		return;
	}
	list->print();
	list->save_graphml(file, "Graph");

	std::cout << "The graph structure has been saved to the \"saved_graphML.GRAPHML\" file in the samples library" << std::endl;

}




/**
 * \brief Example of adding an edge to the graph structure in Graph::List object.
 * 
 * \param graph_type Type of the graph based on Graph::Type enum.
 */
void Graph::Examples::add_edge_list(Type graph_type)
{
	std::unique_ptr<List> list;

	if (graph_type == Type::undirected)
	{
		list = std::make_unique<List>("samples\\graphML.GRAPHML", graph_type);
		list->print();
		list->add_edge(0, 1, 2);
		list->print();
		list->add_edge(0, 2, 7);
		list->print();
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", graph_type);
		list->print();
		list->add_edge(0, 1, 2);
		list->print();
		list->add_edge(0, 2, 7);
		list->print();
	}
	else
	{
		std::cerr << "Choose either Graph::directed or Graph::undirected" << std::endl;
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
		list = std::make_unique<List>("samples\\graphML.GRAPHML", graph_type);
		list->print();
		list->add_node();
		list->print();
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", graph_type);
		list->print();
		list->add_node();
		list->print();
	}
	else
	{
		std::cerr << "Choose either Graph::directed or Graph::undirected" << std::endl;
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
		list = std::make_unique<List>("samples\\graphML.GRAPHML", graph_type);
		list->print();
		list->remove_edge(0, 1);
		list->print();
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", graph_type);
		list->print();
		list->remove_edge(0, 1);
		list->print();
	}
	else
	{
		std::cerr << "Choose either Graph::directed or Graph::undirected" << std::endl;
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
		list = std::make_unique<List>("samples\\graphML.GRAPHML", graph_type);
		list->print();
		list->remove_node(0);
		list->print();
	}
	else if (graph_type == Type::directed)
	{
		list = std::make_unique<List>("samples\\graphMLdirected.GRAPHML", graph_type);
		list->print();
		list->remove_node(0);
		list->print();
	}
	else
	{
		std::cerr << "Choose either Graph::directed or Graph::undirected" << std::endl;
	}
}




/**
 * Example of generating a file containing a randomized pixel map.
 * 
 * Calling this function results in creating a "pixel_map.txt" file in the samples directory.
 */
void Graph::Examples::generate_pixel_map_file()
{
	Data::Generator gen;
	std::ofstream file("samples\\pixel_map.txt");
	gen.make_pixel_map(file, 5, 10);
}




/**
 * Example of creating a randomized pixel map and outputing the result on an output stream other than file.
 * In this case, std::cout output stream was used.
 * 
 */
void Graph::Examples::generate_pixel_map_cout()
{
	Data::Generator gen;
	gen.make_pixel_map(std::cout, 5, 10);
}




/**
 * \brief Create a matrix graph representation object of given type from .lst file.
 * 
 * \param graph_type Type of the graph from the Graph::Type enum.
 */
void Graph::Examples::create_matrix_from_mat_file(Type graph_type)
{
	std::unique_ptr<Matrix> matrix;

	if (graph_type == Type::undirected)
	{
		matrix = std::make_unique<Matrix>("samples\\matrixUndirected.mat", graph_type);
		matrix->print();
	}
	else if (graph_type == Type::directed)
	{
		matrix = std::make_unique<Matrix>("samples\\matrixDirected.mat", graph_type);
		matrix->print();
	}
	else
	{
		std::cerr << "Choose either directed or undirected graph type" << std::endl;
	}

}




/**
 * \brief Example of creating a matrix object from the GraphML file.
 * 
 * \param graph_type Type of the graph from the Graph::Type enum.
 */
void Graph::Examples::create_matrix_from_graphml_file(Type graph_type)
{
	std::unique_ptr<Matrix> matrix;

	if (graph_type == Type::undirected)
	{
		matrix = std::make_unique<Matrix>("samples\\graphML.GRAPHML", graph_type);
		matrix->print();
	}
	else if (graph_type == Type::directed)
	{
		matrix = std::make_unique<Matrix>("samples\\graphMLdirected.GRAPHML", graph_type);
		matrix->print();
	}
	else
	{
		std::cerr << "Please select either directed or undirected Graph type" << std::endl;
	}
}




/**
 * \brief Example of saving an existing Graph::Matrix graph representation to .GRAPHML file.
 *
 * \note This example bases on .GRAPHML file sources to allow user to compare the results
 *		 with given samples.
 *
 * \param graph_type Type of the graph based on the Graph::Type enum.
 */
void Graph::Examples::save_matrix_to_graphml_file(Type graph_type)
{
	std::unique_ptr<Matrix> matrix;

	std::ofstream file("samples\\saved_graphML.GRAPHML");

	if (graph_type == Type::undirected)
	{
		matrix = std::make_unique<Matrix>("samples\\graphML.GRAPHML", graph_type);
		matrix->print();
		matrix->save_graphml(file, "Graph");
	
		std::cout << "The graph structure has been saved to the \"saved_graphML.GRAPHML\" file in the samples library" << std::endl;
	}
	else if (graph_type == Type::directed)
	{
		matrix = std::make_unique<Matrix>("samples\\graphMLdirected.GRAPHML", graph_type);
		matrix->print();
		matrix->save_graphml(file, "Graph");

		std::cout << "The graph structure has been saved to the \"saved_graphML.GRAPHML\" file in the samples library" << std::endl;
	}
	else
	{
		std::cerr << "Please select either directed or undirected Graph type" << std::endl;
	}
}




/**
 * \brief Example of adding an edge to an existing Matrix graph structure.
 * 
 * \param graph_type Type of the graph from Graph::Type enum.
 */
void Graph::Examples::add_edge_matrix(Type graph_type)
{
	std::unique_ptr<Matrix> matrix;

	if (graph_type == Type::undirected)
	{
		matrix = std::make_unique<Matrix>("samples\\graphML.GRAPHML", graph_type);
	}
	else if (graph_type == Type::directed)
	{
		matrix = std::make_unique<Matrix>("samples\\graphMLdirected.GRAPHML", graph_type);
		
	}
	else
	{
		std::cerr << "Please select either directed or undirected Graph type\n";
		return;
	}
	matrix->print();
	matrix->add_edge(0, 1, 2);
	matrix->print();
	matrix->add_edge(0, 2, 7);
	matrix->print();
}




/**
 * \brief Example of adding an isolated vertex to an existing Matrix graph structure.
 * 
 * \param graph_type Type of the graph from the Graph::Type enum.
 */
void Graph::Examples::add_node_matrix(Type graph_type)
{
	std::unique_ptr<Matrix> matrix;

	if (graph_type == Type::undirected)
	{
		matrix = std::make_unique<Matrix>("samples\\graphML.GRAPHML", graph_type);
		matrix->print();
		matrix->add_node();
		matrix->print();
	}
	else if (graph_type == Type::directed)
	{
		matrix = std::make_unique<Matrix>("samples\\graphMLdirected.GRAPHML", graph_type);
		matrix->print();
		matrix->add_node();
		matrix->print();
	}
	else
	{
		std::cerr << "Please select either directed or undirected Graph type\n";
	}
}




/**
 * \brief Example of removing an edge from existing Matrix graph structure.
 * 
 * \param graph_type
 */
void Graph::Examples::remove_edge_matrix(Type graph_type)
{
	std::unique_ptr<Matrix> matrix;

	if (graph_type == Type::undirected)
	{
		matrix = std::make_unique<Matrix>("samples\\graphML.GRAPHML", graph_type);
		matrix->print();
		matrix->remove_edge(0, 1);
		matrix->print();
	}
	else if (graph_type == Type::directed)
	{
		matrix = std::make_unique<Matrix>("samples\\graphMLdirected.GRAPHML", graph_type);
		matrix->print();
		matrix->remove_edge(0, 1);
		matrix->print();
	}
	else
	{
		std::cerr << "Please select either directed or undirected Graph type\n";
	}
}




/**
 * \brief Example of removing a vertex with all its edges from Matrix graph structure.
 * 
 * \param graph_type Type of the graph from Graph::Type enum.
 */
void Graph::Examples::remove_node_matrix(Type graph_type)
{
	std::unique_ptr<Matrix> matrix;

	if (graph_type == Type::undirected)
	{
		matrix = std::make_unique<Matrix>("samples\\graphML.GRAPHML", graph_type);
		matrix->print();
		matrix->remove_node(0);
		matrix->print();
	}
	else if (graph_type == Type::directed)
	{
		matrix = std::make_unique<Matrix>("samples\\graphMLdirected.GRAPHML", graph_type);
		matrix->print();
		matrix->remove_node(0);
		matrix->print();
	}
	else
	{
		std::cerr << "Please select either directed or undirected Graph type\n";
	}
}
