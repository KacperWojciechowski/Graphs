#include "../inc/Graph_matrix.h"
#include "../inc/Examples.h"

int main()
{
	Graph::Matrix matrix("samples\\graphML.GRAPHML", "Name", Graph::Type::undirected);

	matrix.print();

	matrix.remove_node(4);

	matrix.print();
	return 0;
}