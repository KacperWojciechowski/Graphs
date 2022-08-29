#pragma once

/**
 * \example create_list_from_lst.cpp
 *
 * An example showing how to create an object of Graph::List class using .lst file.
 *
 * This example uses the
 * \b listDirected.lst
 * file provided as a sample in the samples folder. The constructor creates the internal graph structure contained
 * within the object. All weights of the edges present in the data source are assumed to be 1.
 * 
 * \see <a href="https://hog.grinvin.org/Formats.action"> Here </a> for House of Graphs adjacency list format guideline.
 * 
 * \see Graph::List::List(std::string file_path, Graph::Type type)
 */

/**
 * \example create_list_from_graphml.cpp
 * 
 * An example showing how to create an object of Graph::List class using .GRAPHML file.
 * 
 * This example uses the
 * \b graphML.GRAPHML
 * file provided as a sample in the samples folder.
 * In case of graphML files, the second argument of the constructor is
 * ignored, as this data is contained within the graphML file itself.
 * 
 * \see <a href="http://graphml.graphdrawing.org/primer/graphml-primer.html"> Here </a> for GraphML format guideline.
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
 * modifies the weight the edge, as shown in the different example linked below. Depending on the graph type, this function
 * inserts an edge or modifies its weight either one-way in case of directed graph, or both ways in case of undirected graph.
 * 
 * \ref make_edge_matrix_override.cpp "Modify the weight of an edge"
 * 
 * \see Graph::Matrix::make_edge()
 */

/**
 * \example make_edge_matrix_override.cpp
 * 
 * An example of changing the weight of an existing edge in Graph::Matrix object.
 * 
 * This example shows how to override the weight of already existing edge between two given vertices in the Graph::Matrix object.
 * The function does verify whether given indexes are correct, and the given weight is non-zero. In case the edge does not exists,
 * it will insert a new edge according to the graph type. The edge will be inserted / its weight modified one way in case of directed
 * graph, or both ways in case of undirected graph. An example of adding a prevously not existing edge to the graph is linked below.
 * 
 * \ref make_edge_matrix_insert.cpp "Add an edge to the graph structure"
 * 
 * \see Graph::Matrix::make_edge()
 */

/**
 * \example add_node_matrix.cpp
 * 
 * An example of adding an isolated vertex to the graph structure in the Graph::Matrix object.
 * 
 * This example shows how to add  an isolated vertex to the graph structure. In order to connect the added vertex with any other
 * vertices, the user must manually insert the connections using the Graph::Matrix::make_edge() function.
 * 
 * \see Graph::Matrix::add_node()
 */

/**
 * \example create_matrix_from_graphml.cpp
 * 
 * An example of creating a Graph::Matrix object based on the .GRAPHML data set.
 * 
 * This example uses the
 * \b graphML.GRAPHML
 * file as the data source. During the process, the function parses the file searching for information such as graph type, 
 * vertices, and edges with their weights. The graph type parameter provided by the user in this case is ignored. In case
 * the information regarding an edge's weight is not present, a weight of 1 is assumed.
 * 
 * \see <a href="http://graphml.graphdrawing.org/primer/graphml-primer.html"> Here </a> for GraphML format guideline.
 * 
 * \see Graph::Matrix::Matrix(std::string file_path, Graph::Type type) 
 */

/**
 * \example create_matrix_from_mat.cpp
 * 
 * An example of creating a Graph::Matrix object based on the .lst data format.
 * 
 * This example uses the
 * \b matrixUndirected.mat
 * file as the data source. During the process, the constructor calls a function that parses provided data source file. 
 * Graph type needs to be stated by the user, as the format does not contain any graph type information itself.
 * 
 * \see <a href="https://hog.grinvin.org/Formats.action"> Here </a> for House of Graphs adjacency matrix format guideline.
 * 
 * \see Graph::Matrix::Matrix(std::string file_path, Graph::Type type)
 */

/**
 * \example remove_edge_matrix.cpp
 * 
 * An example of removing an edge from graph structure in the Graph::Matrix object.
 * 
 * This example shows how to remove an existing edge from the graph structure contained within the Graph::Matrix object.
 * The function does not verify nor needs a verification whether the edge is present. Depending on the graph type, an edge
 * might be removed one way in case of directed graph, or both ways in case of the undirected graph.
 * 
 * \see Graph::Matrix::remove_edge()
 */

/**
 * \example remove_node_matrix.cpp
 * 
 * An example of removing a vertex from the graph structure in the Graph::Matrix object.
 * 
 * This example shows how to remove an existing vertex along with all its edges from the graph structure contained within
 * the Graph::Matrix object. All the edges connected to the given vertex will be removed regardless to the graph type. 
 * Removing a vertex causes a renumeration of each subsequent vertex, decreasing their indexes by 1. The function automatically
 * recalculates the corresponding degrees.
 * 
 * \see Graph::Matrix::remove_node()
 */

/**
 * \example save_matrix_to_graphml.cpp
 * 
 * An example of saving the graph structure from the Graph::Matrix object in a .GRAPHML format.
 * 
 * This example shows how to save the graph structure contained within the Graph::Matrix object in a .GRAPHML XML format.
 * The function accepts a reference to an output stream as a parameter, and as such allows user to save the result in any
 * output stream, such as file or standard output. 
 * 
 * \note In order to display the result, pass the std::cout reference as the argument.
 * 
 * \see Graph::Matrix::save_graphml()
 */