#include "..\inc\Examples.h"

#include "..\inc\Graph_matrix.h"

/* 
	Example of creating the matrix object using the .mat file as a data source
*/
void Examples::create_matrix_from_mat_file()
{
	Graph::Matrix matrix("samples\\input.mat", "Name", Graph::Type::undirected);
	matrix.print();
}




/*
	Example of creating the matrix object using the .graphml file as a data source
*/
void Examples::create_matrix_from_graphml_file()
{
	Graph::Matrix matrix("samples\\graphML.GRAPHML", "Name", Graph::Type::undirected);
	matrix.print();
}




/*
	Example of saving a matrix object to the .graphml file, based on existing .graphml file
*/
void Examples::save_matrix_to_graphml_file()
{
	Graph::Matrix matrix("samples\\graphML.GRAPHML", "Name", Graph::Type::undirected);
	matrix.save_graphml("samples\\saved_graphML.GRAPHML");
}
