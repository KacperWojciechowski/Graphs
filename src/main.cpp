#include "../inc/Graph_matrix.h"
#include "../inc/Examples.h"
#include "../inc/Graph_list.h"

using namespace Graph;

int main()
{
	Examples::save_matrix_to_graphml_file(Type::undirected);
	return 0;
}