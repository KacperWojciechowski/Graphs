#include "Graph_matrix.h"
#include "Graph_list.h"
#include "Pixel_map.h"
#include "Benchmark.h"

#include <iostream>
#include <fstream>

int main()
{
	Data::Pixel_map pm("../test/sample/pixelMap.txt");
	pm.print_area_map();
	Graph::List l(pm);
	l.print();
	return 0;
}