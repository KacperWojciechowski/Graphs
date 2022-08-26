#pragma once

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

/**
 * \example add_edge_list_insert.cpp
 * 
 * An example of inserting an edge between two given vertices in the Graph::List object.
 * 
 * This example shows how to insert a connection between two previously separated vertices in
 * the graph structure. The inserted weight of the connection must be greater than zero, otherwise
 * executing the function will result in an exception.
 * 
 * In case of calling this function with pair of vertices between a connection already exists, 
 * the weight of the edge will be overwritten with the new value, as shown in the example linked below.
 * Depending of the type of the graph, the edge will be modified either one way for directed graph, or
 * two ways for undirected graphs.
 * 
 * \ref add_edge_list_override.cpp "Overriding the weight of an existing edge"\n
 */

/**
 * \example add_edge_list_override.cpp
 * 
 * An example of overriding the weight of an already existing edge between two given vertices in the Graph::List object.
 * 
 * This example shows how to modify the weight of the connection between two already connected vertices
 * in the graph structure. New weight must be greater than zero, otherwise executing the function will result in an exception.
 * 
 * In case of calling this function with pair of separated vertices, this function will insert an edge between the two
 * vertices. Type of the edge (unidirectional / bidirectional) is decided based on the graph type. To see an example of
 * inserting an edge between vertices, see the example linked below.
 * 
 * \ref add_edge_list_insert.cpp "Inserting an edge between two vertices"
 */





