#pragma once

#include <string>

namespace Data
{
	/*
		Class responsible for generating various data sources such as matrix graphs, throughtput
		graphs and pixel maps.
	*/
	class Generator
	{
		void generate_weighted_graph(std::string file_path, uint32_t nodes_amount);
		void generate_throughtput(std::string graph_file_path, std::string output_file_path);
	};
}