#include "..\inc\Generator.h"

#include <fstream>

int main()
{
    Data::Generator gen;

    std::ofstream file("samples\\saved_list.lst");
    gen.make_list(file, 10, Graph::Type::undirected, 2);
}