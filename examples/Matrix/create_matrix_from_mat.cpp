#include "..\..\inc\Graph_matrix.h"

int main()
{
    Graph::Matrix matrix("samples\\matrixUndirected.mat", Graph::Type::undirected);
    matrix.print();
}