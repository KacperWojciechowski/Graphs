#pragma once

#include <fstream> 

#include "Graph.h"

namespace Graph
{
	class Benchmark
	{
	public:
		enum class Mode
		{
			append = 0,
			overwrite
		};

		Benchmark() {}
		Benchmark(Benchmark&) = delete;
		Benchmark(Benchmark&&) = delete;

		void run(Graph& graph, std::string identifier, std::string file_path, uint16_t iterations, Mode mode, bool log);
		void color_benchmark(Graph& graph, std::string identifier, uint16_t iterations, std::fstream& file, bool log);

		~Benchmark() {}
	};
}