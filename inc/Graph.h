#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace Graph
{
	/*
		Abstract class stating the commmon interface for each of the graph representations.
	*/
	class Graph
	{
	public:

		// basic operations interface
		virtual void print() = 0;
		
		virtual void add_edge(uint32_t source, uint32_t destination) = 0;
		virtual void add_node() = 0;
		
		virtual void remove_edge(uint32_t source, uint32_t destionation) = 0;
		virtual void remove_node(uint32_t node_id) = 0;

		virtual const uint32_t get_nodes_amount() = 0;
		virtual const uint32_t get_node_degree() = 0;
		virtual const uint8_t get_edge(uint32_t source, uint32_t destination) = 0;
	};
}