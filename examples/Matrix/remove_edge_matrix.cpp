#include "..\..\inc\Graph_matrix.h"

int main()
{
    Graph::Matrix matrix("samples\\graphML.GRAPHML", Graph::Type::undirected);
    matrix.print();
    matrix.remove_edge(0, 1);
    matrix.print();
}