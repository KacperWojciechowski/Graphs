#include "..\..\inc\Generator.h"
#include <fstream>

int main()
{
    Data::Generator gen;
    
    std::ofstream file("samples\\saved_matrix.mat");

    if (file.good())
    {
        gen.make_matrix(file, 10, {5, 10}, Graph::Type::undirected, 2);
    }
}