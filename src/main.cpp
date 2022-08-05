#include "../inc/Graph_matrix.h"
#include "../inc/Examples.h"
#include "../inc/Graph_list.h"

int main()
{
	Graph::List list("samples\\list.lst", "Name", Graph::Type::undirected);

	list.print();
	return 0;
}