#include "..\..\inc\Graph_list.h"

int main()
{
    Graph::List list("samples\\graphML.GRAPHML", Graph::Type::undirected);
    list.print();
    list.add_edge(0, 2, 7);
    list.print();
}