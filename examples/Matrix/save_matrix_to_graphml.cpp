#include "..\..\inc\Graph_matrix.h"

#include <fstream>

int main()
{
    Graph::Matrix matrix("samples\\graphML.GRAPHML", Graph::Type::undirected);
    matrix.print();

    std::ofstream file("samples\\saved_graphML.GRAPHML");
    if (file.good())
    {
        matrix.save_graphml(file, "Graph");
    }
}