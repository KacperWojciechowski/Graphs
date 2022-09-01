#include "..\..\inc\PixelMap.h"
#include "..\..\inc\Graph_list.h"

#include <fstream>

int main()
{
    std::ifstream file("samples\\pixel_map.txt");

    if (file.good())
    {
        Data::PixelMap pixelmap(file);
        pixelmap.print();
        Graph::List list(pixelmap);
        list.print();
    }
}