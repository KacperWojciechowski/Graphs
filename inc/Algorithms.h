#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace Graph
{
	/**
	 * \brief Structure for path-searching algorithms. 
	 * 
	 * \note The structure does allow for storing several paths of the same length.
	 */
	struct Roadmap
	{
		std::vector<std::vector<uint32_t>> prev_node; /**< Vector of possible previous vertices with the same distance */
		std::vector<uint32_t> distance; /**< The distance value from start vertex to any given vertex */
	};

	/*
		Abstract algorithmic interface for the specific graph representations to implement
	*/

	/**
	 * \brief Virtual class providing the algorithmic interface.
	 * 
	 * The class provides common algorithmic interface for each of its inheriting
	 * classes. Its main purpose is to provide the Data::Benchmark class with an
	 * ability to use those functions without the knowledge of which graph representation
	 * was received as a parameter.
	 * 
	 * \attention Each of the coloring algorithms public function provide permutation
	 * functionality for randomized trials of efficiency. As such, to get optimal results
	 * for the algorithm, it is recommended to run the function several times, as does the
	 * Data::Benchmark class.
	 */
	class Algorithms
	{
	public:

		// algorithmic interface ------------------------------------------------------------------------------------------------

		/**
		 * Common definition of the function providing ordering and permutation sequence according to the greedy algorithm rules.
		 * 
		 * \param log Flag stating whether the function should produce logs on the standard output
		 * during its runtime. This value is also passed to the core coloring function.
		 * \return Number of colors used in the coloring process.
		 */
		virtual int32_t greedy_coloring(bool log) = 0;

		/**
		 * Common definition of the function providing ordering and permutation sequence according to the LF algorithm rules. 
		 * 
		 * \param log Flag stating whether the function should produce logs on the standard output
		 * during its runtime. This value is also passed to the core coloring function.
		 * \return Numer of colors used in the coloring process.
		 */
		virtual int32_t lf_coloring(bool log) = 0;

		/**
		 * Common definition of the function providing ordering and permutation sequence according to the SL algorithm rules.
		 * 
		 * \param log Flag stating whether the function should produce logs on the standard output
		 * during its runtime. This value is also passed to the core coloring function.
		 * \return Numer of colors used in the coloring process.
		 */
		virtual int32_t sl_coloring(bool log) = 0;

		/**
		 * Common definition of the function providing the Belman-Ford path-searching algorithm implementation.
		 * 
		 * \param start_vertex Numer of vertex (starting from 0) from which the search for paths to all other
		 * vertices should begin.
		 * \param log Flag stating whether the function should produce logs on the standard output
		 * during its runtime.
		 * \return Data::Roadmap structure containing all necessary informations regarding the found paths.
		 */
		virtual Roadmap belman_ford(std::size_t start_vertex, bool log) = 0;

	protected:

		// coloring functions core ----------------------------------------------------------------------------------------------

		/**
		 * Common definition of the function providing the core coloring functionality.
		 * This functionality is designed for the algorithms that base on the greedy coloring sequence 
		 * (once provided the right order of vertices according to their own rules).
		 * 
		 * \param m Map containing the created ordering of vertices.
		 * \param log Flag stating whether the function should produce logs on the standard output
		 * during its runtime.
		 * \return Number of colors used in the coloring process.
		 */
		virtual int32_t greedy_coloring_core(std::map<int, int>& m, bool log) = 0;
	};
}