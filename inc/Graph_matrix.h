#pragma once

#include "Graph_list.h"

#include <stdint.h>
#include <string>

#include <vector>


namespace Graph
{
	class List;

	class Matrix
	{
	public:
		Matrix(std::string file_path, std::string ng, std::string type);
		Matrix(List& list);

		Matrix(Matrix& m) = delete;
		Matrix(Matrix&& m) = delete;

		void print();
		void printGraphML();
		void zad_3_a();
		void zad_3_d();
		void zad_3_c();

		void add_edge(uint32_t source, uint32_t destination);
		void add_node();

		void remove_edge(uint32_t source, uint32_t destination);
		void remove_node(uint32_t node_id);

		const uint32_t get_nodes_amount();
		const uint32_t get_value(uint32_t x, uint32_t y);
		void save_graphml(std::string file_path);

		void change_to_line_graph();

        int belman_ford(uint32_t vertex, bool log);
        int throughtput_belman_ford(uint32_t searched_throughtput, uint32_t vertex, bool log);

		~Matrix();

	private:
		struct coord
		{
			uint32_t x;
			uint32_t y;
		};

		uint32_t find_index(std::vector<coord>& edges, uint32_t x, uint32_t y);

		uint32_t nodes_amount;
		uint32_t** matrix;
		std::string nazwa_grafu;
		std::string typ_grafu;
	};

}