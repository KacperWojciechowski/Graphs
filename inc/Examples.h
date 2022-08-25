#pragma once

#include "Graph.h"

/**
 * \example create_list_from_lst.cpp
 *
 * An example showing how to create an object of Graph::List class using .lst file.
 *
 * This example shows how to pass the path to the source file
 * and graph type from the Graph::Type enum. It uses the
 * \b listDirected.lst
 * file provided as a sample in the samples folder.
 * 
 * \see https://hog.grinvin.org/Formats.action for House of Graphs adjacency list format guideline.
 */

/**
 * \example create_list_from_graphml.cpp
 * 
 * An example showing how to create an object of Graph::List class using .GRAPHML file.
 * 
 * This example shows how to pass the path to the source file and
 * graph type from the Graph::Type enum. It uses the
 * \b graphML.GRAPHML
 * file provided as a sample in the samples folder.
 * In case of graphML files, the second argument of the constructor is
 * ignored, as this data is contained within the graphML file itself.
 * 
 * \see http://graphml.graphdrawing.org/primer/graphml-primer.html for GraphML format guideline.
 */

namespace Graph
{
	/**
	 * \brief Class containing various examples in form of static functions.
	 */
	class Examples
	{
	public:
		/* Matrix examples */
		static void create_matrix_from_mat_file(Type graph_type);		// checked
		static void create_matrix_from_graphml_file(Type graph_type);	// checked
		static void save_matrix_to_graphml_file(Type graph_type);		// checked
		static void add_edge_matrix(Type graph_type);					// checked
		static void add_node_matrix(Type graph_type);					// checked
		static void remove_edge_matrix(Type graph_type);				// checked
		static void remove_node_matrix(Type graph_type);				// checked
	};
}
