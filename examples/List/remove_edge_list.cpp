#include "..\..\inc\Graph_list.h"

int main()
{
    Graph::List list("samples\\graphML.GRAPHML", Graph::Type::undirected);
    list.print();
    list.remove_edge(0, 1);
    list.print();
}