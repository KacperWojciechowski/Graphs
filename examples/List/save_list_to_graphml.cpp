#include "..\..\inc\Graph_list.h"

#include <fstream>

int main()
{
    Graph::List list("samples\\graphML.GRAPHML", Graph::Type::undirected);
    list.print();
    
    std::ofstream file("samples\\saved_graphML.GRAPHML");
   
    if (file.good())
    {
        list.save_graphml(file, "Graph");
    }
}