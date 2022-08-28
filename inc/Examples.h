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
 * 
 * \see Graph::List::List(std::string file_path, Graph::Type type)
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
 * 
 * \see Graph::List::List(std::string file_path, Graph::Type type)
 */

/**
 * \example make_edge_list_insert.cpp
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
 * \ref make_edge_list_override.cpp "Overriding the weight of an existing edge"\n
 * 
 * \see Graph::List::make_edge()
 */

/**
 * \example make_edge_list_override.cpp
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
 * \ref make_edge_list_insert.cpp "Inserting an edge between two vertices"
 * 
 * \see Graph::List::make_edge()
 */

/**
 * \example add_node_list.cpp
 * 
 * An example of adding an isolated vertex to the adjacency list graph structure. 
 * 
 * This example shows how to add an isolated vertex to the graph structure in the Graph::List object. Added vertex will
 * assume the index equal to the previous count of vertices. After adding the vertex, user needs to manually insert any
 * edges connected to it, using the add_edge() function.
 * 
 * \ref add_edge_list_insert.cpp "Adding an edge between two vertices"
 * 
 * \see Graph::List::add_node() 
 */

/**
 * \example remove_edge_list.cpp
 * 
 * An example of removing existing edge between two given vertices in adjacency list graph structure.
 * 
 * This example shows how to remove an existing edge between two given vertices in the graph structure of the Graph::List object.
 * If the type of the graph is undirected, the edge will be removed both ways. Else, only one-way edge will be removed, preserving
 * any edge directed the other way around.
 * 
 * \note Using this method does not need to verify whether the edge exists beforehand.
 * 
 * \see Graph::List::remove_edge()
 */

/**
 * \example remove_node_list.cpp
 * 
 * An example of removing a vertex from the adjacency list graph structure.
 * 
 * This example shows how to remove a vertex along with all its edges from the graph structure of the Graph::List object.
 * This function will remove all ingoing and outgoing edges regardless of the graph type. Removing the vertex causes the
 * renumeration of each subsequent vertex by decreasing their indexes by 1.
 * 
 * \see Graph::List::remove_node()
 */

/**
 * \example save_list_to_graphml.cpp
 * 
 * An example of saving the graph structure of Graph::List object in a .GRAPHML format in given output stream.
 * 
 * This example shows how to save current graph structure contained within Graph::List object in a .GRAPHML XML format.
 * Created XML contains information such as graph name, type, IDs of vertices with added "n" notation in front, and all edges 
 * with corresponding weights. The output is saved to the output stream passed as an argument to the function, and as such 
 * can work with both file stream and standard output stream.
 * 
 * \note In order to display the resulting XML, pass std::cout reference as a stream argument.
 * 
 * \note A sample XML output can be seen in the samples directory in 
 * \b "saved_graphML.GRAPHML" 
 * file.
 * 
 * \see Graph::List::save_graphml()
 */

/**
 * \example pixel_map_generator.cpp
 * 
 * An example of generating a randomized pixel map data source via Data::Generator class.
 * 
 * This example shows how to generate a randomized pixel map data. The output is directed to an output stream passed
 * as an argument to the function, and as such, can redirect the output to file or to standard output.
 * 
 * \note To redirect the function result to the standard output, pass std::cout reference as an argument.
 * 
 * \see Data::Generator::make_pixel_map()
 */


/**
 * \example make_edge_matrix_insert.cpp
 * 
 * An example of adding an edge between two given vertices in Graph::Matrix object.
 * 
 * This example shows how to add a new edge to the graph structure in the Graph::Matrix object. The function does verify
 * whether given indexes are correct, and the given weight is non-zero. In case the edge already exists, this function
 * modifies the weight the edge, as shown in the different example linked below.
 * 
 * \ref make_edge_matrix_override.cpp "Modify the weight of an edge"
 * 
 * \see Graph::Matrix::make_edge()
 */





