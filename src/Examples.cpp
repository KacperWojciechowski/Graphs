#include "..\inc\Examples.h"

#include "..\inc\Graph_matrix.h"
#include "..\inc\Graph_list.h"
#include "..\inc\Generator.h"

#include <memory>



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
