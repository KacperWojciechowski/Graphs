#include "..\..\inc\Graph_list.h"

int main()
{
    Graph::List list("samples\\graphML.GRAPHML", Graph::Type::undirected);
    list.print();
    list.remove_node(0);
    list.print();
}