#include "..\..\inc\Graph_list.h"

void main()
{
    Graph::List list("samples\\listDirected.lst", Graph::Type::directed);
    list.print();
}