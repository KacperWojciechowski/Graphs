#include "../inc/Graph_matrix.h"
#include "../inc/Examples.h"

int main()
{
	Graph::Matrix matrix("samples\\graphML.GRAPHML", "Name", Graph::Type::undirected);

	Graph::Matrix m2 = matrix.change_to_line_graph();

	Graph::Matrix m3 = m2;
	return 0;
}