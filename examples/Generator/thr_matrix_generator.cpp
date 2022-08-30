#include "..\..\inc\Generator.h"

#include <iostream>
#include <fstream>

int main() {
	Data::Generator gen;

	std::ifstream file2("samples\\saved_matrix.mat");
	gen.make_throughtput_matrix(std::cout, file2, { 10, 20 }, Graph::Type::undirected);

	return 0;
}