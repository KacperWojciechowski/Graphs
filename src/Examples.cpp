#include "..\inc\Examples.h"

#include "..\inc\Graph_matrix.h"

/**
 * \brief Example of creating the matrix object using the .mat file as a data source.
 */
void Graph::Examples::create_matrix_from_mat_file()
{
	Matrix matrix("samples\\input.mat", "Name", Type::undirected);
	matrix.print();
}




/**
 * \brief Example of creating the matrix object using the .GRAPHML file as a data source
 * 
 */
void Graph::Examples::create_matrix_from_graphml_file()
{
	Matrix matrix("samples\\graphML.GRAPHML", "Name", Type::undirected);
	matrix.print();
}




/**
 * \brief Example of saving the matrix object into a .GRAPHML format file. 
 * 
 * \note Any format supported by the Graph::Matrix class can be used as the source to be able
 * to use this functionality. This example uses .GRAPHML data source, to allow user to compare
 * the content of sample file with the saved file. 
 * 
 * \note The resulting file is saved within the samples folder as "saved_graphML.GRAPHML"
 */
void Graph::Examples::save_matrix_to_graphml_file()
{
	Matrix matrix("samples\\graphML.GRAPHML", "Name", Type::undirected);
	matrix.save_graphml("samples\\saved_graphML.GRAPHML");
}
	