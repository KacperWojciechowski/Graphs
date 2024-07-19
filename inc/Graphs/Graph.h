#pragma once

#include <map>
#include <vector>

#include <optional>

namespace Graphs
{
	using NodeId = unsigned int;

	struct EdgeInfo
	{
		NodeId source;
		NodeId destination;
		std::optional<unsigned int> weight;
	};
	
	class Graph
	{
	public:
		friend std::ostream& operator<<(std::ostream& os, const Graph& g)
		{
			g.show();
			return os;
		}

		virtual void show() const = 0;
		virtual unsigned int nodesAmount() const = 0;
		virtual unsigned int nodeDegree(NodeId) const = 0;

		virtual void setEdge(EdgeInfo) = 0;
		virtual void addNode() = 0;
		virtual void removeNode(NodeId) = 0;
		virtual void removeEdge(EdgeInfo) = 0;

		virtual ~Graph() = default;

		// OBSOLETE INTERFACE
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