#include "..\..\inc\Generator.h"

#include <iostream>

int main()
{
    Data::Generator gen;

    gen.make_pixel_map(std::cout, 5, 10);
}