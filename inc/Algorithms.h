#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace Graph
{
	/*
		Structure for path-searching algorithms. The structure does allow for
		storing several paths of the same length.
	*/
	struct Roadmap
	{
		std::vector<std::vector<uint32_t>> prev_node;
		std::vector<uint32_t> distance;
	};

	/*
		Abstract algorithmic interface for the specific graph representations to implement
	*/
	class Algorithms
	{
	public:

		// algorithmic interface
		virtual int32_t greedy_coloring(bool log) = 0;
		virtual int32_t lf_coloring(bool log) = 0;
		virtual int32_t sl_coloring(bool log) = 0;
		virtual Roadmap belman_ford(uint32_t start_vertex, bool log) = 0;
		virtual Roadmap throughtput_belman_ford(uint32_t start_vertex, uint32_t minimal_throughtput, bool log) = 0;

	protected:

		// coloring functions core
		virtual int32_t greedy_coloring_core(std::map<int, int>& m, bool log) = 0;
	};
}