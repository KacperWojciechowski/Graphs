#include "..\..\inc\Graph_matrix.h"

int main()
{
    Graph::Matrix matrix("samples\\graphML.GRAPHML", Graph::Type::undirected);
    matrix.print();
    matrix.make_edge(0, 2, 7);
    matrix.print();
}