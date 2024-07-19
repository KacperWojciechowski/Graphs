#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
#include <Graphs/Pixel_map.hpp>
#include <Graphs/Benchmark.hpp>

#include <iostream>
#include <fstream>

int main()
{
	Data::Pixel_map pm("../test/sample/pixelMap.txt");
	pm.print_area_map();
	Graphs::AdjList l(pm);
	std::cout << l << std::endl;
	return 0;
}