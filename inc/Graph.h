#pragma once

#include <stdint.h>
#include <map>
#include <vector>

namespace Graph
{
	/*
		Abstract class stating the interface for each of the concretizations.
	*/
	class Graph
	{
	public:

		// basic operations interface
		virtual void print() = 0;
		virtual void add_edge(uint32_t source, uint32_t destination) = 0;
		virtual void add_node() = 0;
		virtual void remove_edge(uint32_t source, uint32_t destination) = 0;
		virtual void remove_node(uint32_t node_id) = 0;
		virtual const uint32_t get_nodes_amount() = 0;
		virtual const uint32_t get_node_degree() = 0;

		// algorithmic interface
		virtual int32_t greedy_coloring(bool log) = 0;
		virtual int32_t lf_coloring(bool log) = 0;
		virtual int32_t sl_coloring(bool log) = 0;
		virtual int32_t belman_ford(uint32_t vertex, bool log) = 0;
		virtual int32_t throughtput_belman_ford(uint32_t searched_throughtput, uint32_t vertex, bool log) = 0;

	protected:

		// algorithmic components for internal use
		virtual int32_t greedy_coloring_core(std::map<int, int>* m, bool log) = 0;
		virtual void shuffle(std::vector<int>& v, bool log) = 0;
	};
}