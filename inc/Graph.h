#pragma once

#include <cstdint>
#include <string>

namespace Graph
{
	/*
		Enum class representing the available graph classes
	*/
	enum class Type
	{
		undirected = 0,
		directed,
		undefined
	};

	/*
		Abstract class stating the commmon interface for each of the graph representations.
	*/
	class Graph
	{
	public:

		// basic operations interface
		virtual void print() = 0;
		
		virtual void add_edge(std::size_t source, std::size_t destination, uint32_t weight) = 0;
		virtual void add_node() = 0;
		
		virtual void remove_edge(std::size_t source, std::size_t destionation) = 0;
		virtual void remove_node(std::size_t node_id) = 0;

		virtual const std::size_t get_nodes_amount() = 0;
		virtual const std::size_t get_node_degree(std::size_t node_id) = 0;
		virtual const uint32_t get_edge(std::size_t source, std::size_t destination) = 0;

		//virtual void save_graphml(std::string output_file_path);
	};
}