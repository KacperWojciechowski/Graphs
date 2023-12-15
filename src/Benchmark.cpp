#include <iostream>

#include "Benchmark.h"

void Graph::Benchmark::run(Graph& graph, std::string identifier, std::string file_path, uint16_t iterations, Mode mode, bool log)
{
	std::fstream file;
	switch (mode)
	{
	case Mode::append:
		file.open(file_path, std::ios_base::out | std::ios_base::app);
		break;
	case Mode::overwrite:
		file.open(file_path, std::ios_base::out);
		break;
	}
	if (file.good())
	{
		this->color_benchmark(graph, identifier, iterations, file, log);
	}
	else
	{
		std::cout << "Error opening the benchmark file" << std::endl;
	}
	std::cout << "Benchmark of" << identifier << " done" << std::endl;
	file.close();
}

void Graph::Benchmark::color_benchmark(Graph& graph, std::string identifier, uint16_t iterations, std::fstream& file, bool log)
{
	for (uint16_t i = 0; i < iterations; i++)
	{
		file << identifier << ";";
		file << i << ";";
		file << graph.greedy_coloring(log) << ";";
		file << graph.lf_coloring(log) << ";";
		file << graph.sl_coloring(log) << std::endl;
		std::cout << "    Iteration " << i + 1 << " done" << std::endl;
	}
}
