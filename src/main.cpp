#include "../inc/Graph_matrix.h"
#include "../inc/Examples.h"
#include "../inc/Graph_list.h"

int main()
{
	Graph::List list("samples\\graphML.GRAPHML", "Name", Graph::Type::undirected);

	list.print();

	list.remove_node(1);

	list.print();
	return 0;
}