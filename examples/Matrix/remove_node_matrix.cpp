#include "..\..\inc\Graph_matrix.h"

int main()
{
    Graph::Matrix matrix("samples\\graphML.GRAPHML", Graph::Type::undirected);
    matrix.print();
    matrix.remove_node(0);
    matrix.print();
}