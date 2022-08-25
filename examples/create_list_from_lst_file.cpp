#include "..\inc\Graph_list.h"

int main()
{
    Graph::List list("samples\\listDirected.lst", Graph::Type::directed);
    list.print();
}