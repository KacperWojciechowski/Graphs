#pragma once

#include <iostream>
#include <optional>
#include <cstdint>

namespace Graphs
{
	using NodeId = uint32_t;

	struct EdgeInfo
	{
		NodeId source;
		NodeId destination;
		std::optional<uint32_t> weight;
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
		virtual uint32_t weightOf(const EdgeInfo&) const = 0;
		virtual uint32_t nodesAmount() const = 0;
		virtual uint32_t nodeDegree(NodeId) const = 0;

		virtual void setEdge(const EdgeInfo&) = 0;
		virtual void addNode() = 0;
		virtual void removeNode(NodeId) = 0;
		virtual void removeEdge(const EdgeInfo&) = 0;

		virtual ~Graph() = default;
	};
}