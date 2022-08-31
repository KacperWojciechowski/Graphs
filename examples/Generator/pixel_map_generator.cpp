#include "..\..\inc\Generator.h"

#include <fstream>

int main()
{
    Data::Generator gen;

    std::ofstream file("samples\\pixel_map.txt");
    
    if (file.good())
    {
        gen.make_pixel_map(file, 5, 10);
    }
}