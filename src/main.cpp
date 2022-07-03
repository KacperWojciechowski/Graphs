#include "../inc/Graph_matrix.h"

int main()
{
	Graph::Matrix matrix("samples\\graphML.GRAPHML", "Name", Graph::Type::undefined);
	matrix.print();
	

	return 0;
}