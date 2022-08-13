#pragma once

#include "Graph.h"

namespace Graph
{
	/**
	 * \brief Class containing various examples in form of static functions.
	 */
	class Examples
	{
	public:
		/* Matrix examples */
		static void create_matrix_from_mat_file(Type graph_type);
		static void create_matrix_from_graphml_file(Type graph_type);
		static void save_matrix_to_graphml_file(Type graph_type);
		static void add_edge_matrix(Type graph_type);
		static void add_node_matrix(Type graph_type);
		static void remove_edge_matrix(Type graph_type);
		static void remove_node_matrix(Type graph_type);

		/* List examples */
		static void create_list_from_lst_file(Type graph_type);			// checked
		static void create_list_from_graphml_file(Type graph_type);		// checked
		static void save_list_to_graphml_file(Type graph_type);			// checked
		static void add_edge_list(Type graph_type);						// checked
		static void add_node_list(Type graph_type);						// checked
		static void remove_edge_list(Type graph_type);					// checked
		static void remove_node_list(Type graph_type);					// checked
	};
}
