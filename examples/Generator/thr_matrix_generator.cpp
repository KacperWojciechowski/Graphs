#include "..\..\inc\Generator.h"

#include <iostream>
#include <fstream>

int main() {
	Data::Generator gen;

	std::ifstream in("samples\\matrixUndirected.mat");
	std::ofstream out("samples\\saved_thr_matrix.mat");
	
	if (in.good() && out.good())
	{
		gen.make_throughtput_matrix(out, in, { 10, 20 }, Graph::Type::undirected);
	}
	return 0;
}