#include "..\..\inc\Generator.h"

#include <iostream>
#include <fstream>

int main() {
	Data::Generator gen;

	std::ofstream file("samples\\matrixUndirected.mat");
	std::ifstream file2("samples\\saved_thr_matrix.mat");
	
	if (file.good() && file2.good())
	{
		gen.make_throughtput_matrix(std::cout, file2, { 10, 20 }, Graph::Type::undirected);
	}
	return 0;
}