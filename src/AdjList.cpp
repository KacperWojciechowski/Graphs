#include <AdjList.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <ranges>

/*Graph::List::List(std::string file_path)
	: degrees(nullptr),
	nodes(nullptr)
{
	std::fstream file;
	file.open(file_path, std::ios::in);

	uint32_t amount = 0;

	if (file.is_open())
	{
		std::string line;
		size_t pos = 0;

		while (!file.eof())
		{
			std::getline(file, line);
			if (line != "")
			{
				amount++;
			}
		}

		this->nodes = new Node* [amount];
		this->degrees = new uint32_t[amount];

		for (uint32_t i = 0; i < amount; i++)
		{
			this->nodes[i] = nullptr;
			this->degrees[i] = 0;
		}

		Node* ptr;
		Node** ptr2;

		file.close();
		file.open(file_path, std::ios::in);

		for(uint32_t i = 0; i < amount; i++)
		{
			std::getline(file, line);
			if (line != "")
			{
				this->node_map.insert(std::pair<int, int>(i + 1, i));
				pos = line.find(' ');
				ptr2 = &this->nodes[i];
				while (pos != std::string::npos)
				{
					line = line.substr(pos + 1);
					ptr = new Node;
					ptr->neighbour = atoi(line.c_str());
					ptr->next = nullptr;
					*ptr2 = ptr;
					ptr2 = &((*ptr2)->next);
					this->degrees[i]++;

					pos = line.find(' ');
				}

			}
		}
	}
	else
	{
		std::cout << "File not found" << std::endl;
	}
	file.close();
}

Graph::List::List(Matrix& matrix)
	: degrees(nullptr),
	nodes(nullptr)
{
	uint32_t amount = matrix.get_nodes_amount();

	this->nodes = new Node * [amount];
	this->degrees = new uint32_t[amount];

	Node* ptr;
	Node** ptr2;
	uint32_t value;

	for (uint32_t i = 0; i < amount; i++)
	{
		this->degrees[i] = 0;
		ptr2 = &this->nodes[i];
		for (uint32_t j = 0; j < amount; j++)
		{
			value = matrix.get_value(i, j);
			while (value > 0)
			{
				ptr = new Node;
				ptr->neighbour = j + 1;
				ptr->next = nullptr;
				*ptr2 = ptr;
				ptr2 = &((*ptr2)->next);
				value--;
				this->degrees[i]++;
			}
		}
		this->node_map.insert(std::pair<int, int>(i + 1, i));
	}
}*/

/*Graph::List::List(Data::Pixel_map& map)
	: nodes(nullptr),
	degrees(nullptr)
{
	uint32_t map_columns = map.get_columns();
	uint32_t map_rows = map.get_rows();

	std::vector<Data::coord> nodes;

	for (uint32_t i = 0; i < map_rows; i++)
	{
		for (uint32_t j = 0; j < map_columns; j++)
		{
			if (map.get_field(i, j) == 0)
			{
				nodes.push_back({ i, j });
			}
		}
	}

	uint32_t size = nodes.size();
	this->nodes = new Node*[size];
	this->degrees = new uint32_t[size];

	Node* ptr;
	Node** ptr2;

	Data::coord coord;

	for (uint32_t i = 0; i < size; i++)
	{
		coord = nodes[i];
		this->node_map.insert(std::pair<int, int>(i + 1, i));
		this->degrees[i] = 0;
		ptr2 = &this->nodes[i];
		*ptr2 = nullptr;

		if (static_cast<int32_t>(coord.x) - 1 >= 0)
		{
			if (map.get_field(coord.x - 1, coord.y) == 0)
			{
				ptr = new Node;
				ptr->neighbour = find_index(nodes, coord.x - 1, coord.y) + 1;
				ptr->next = nullptr;
				*ptr2 = ptr;
				ptr2 = &((*ptr2)->next);
				this->degrees[i]++;
			}
		}

		if (static_cast<int32_t>(coord.y) - 1 >= 0)
		{
			if (map.get_field(coord.x, coord.y - 1) == 0)
			{
				ptr = new Node;
				ptr->neighbour = find_index(nodes, coord.x, coord.y - 1) + 1;
				ptr->next = nullptr;
				*ptr2 = ptr;
				ptr2 = &((*ptr2)->next);
				this->degrees[i]++;
			}
		}

		if (coord.y + 1 < map_columns)
		{
			if (map.get_field(coord.x, coord.y + 1) == 0)
			{
				ptr = new Node;
				ptr->neighbour = find_index(nodes, coord.x, coord.y + 1) + 1;
				ptr->next = nullptr;
				*ptr2 = ptr;
				ptr2 = &((*ptr2)->next);
				this->degrees[i]++;
			}
		}

		if (coord.x + 1 < map_rows)
		{
			if (map.get_field(coord.x + 1, coord.y) == 0)
			{
				ptr = new Node;
				ptr->neighbour = find_index(nodes, coord.x + 1, coord.y) + 1;
				ptr->next = nullptr;
				*ptr2 = ptr;
				ptr2 = &((*ptr2)->next);
				this->degrees[i]++;
			}
		}
	}
}*/

namespace
{
void printDegree(std::ostream& out, const graph::Degree& degree) noexcept
{
	out << "[";
	if (std::holds_alternative<graph::UndirectedDegree>(degree))
	{
		out << std::get<graph::UndirectedDegree>(degree);
	}
	else
	{
		const auto degs = std::get<graph::DirectedDegree>(degree);
		out << degs.inDeg << "|" << degs.outDeg;
	}
	out << "] ";
}

void printNeighbours(
	std::ostream& out,
	std::size_t node,
	const graph::Degree& degree,
	const std::vector<graph::Graph::Edge>& neighbours) noexcept
{
	out << "\t\t" << node << ": ";

	std::size_t i = 0;
	for (auto itr = neighbours.begin(); itr != neighbours.end(); itr++)
	{
		printDegree(out, degree);
		const auto edgeTarget = itr->first.target;
		const auto edgeWeight = itr->second;

		out << "{" << edgeTarget << ", " << edgeWeight << "}";
		if (i++ < neighbours.size() - 1)
		{
			out << ", ";
		}
	}
	out << "\n";
}

std::size_t findMaxNodeIndex(std::map<std::size_t, std::size_t> nodeMap)
{
	using Index = std::pair<std::size_t, std::size_t>;

	return std::ranges::max(std::views::keys(nodeMap), {});
}

} // namespace ::

namespace graph
{
void AdjList::print(std::ostream& out) const noexcept
{
	out << "{\n";
	out << "\tNodes count = " << nodeMap.size() << "\n";
	out << "\tGraph degree = " << getGraphDeg() << "\n";
	out << "\t{\n";
	std::ranges::for_each(nodeMap, [this, &out](const auto& node){
		printNeighbours(out, node.first, adjList[node.second].first, adjList[node.second].second);
	});
	out << "\t}\n}\n" << std::flush;
}

void AdjList::setEdge(const Edge& edge)
{
	auto& neighbours = adjList[nodeMap.at(edge.first.source)].second;
	for (auto& neighbour : neighbours)
	{
		if (neighbour.first.target == edge.first.target)
		{
			neighbour.second = edge.second;
			return;
		}
	};
	neighbours.emplace_back(edge);
}

std::size_t AdjList::addNode(std::size_t currMaxNodeIndex)
{
	if (nodeMap.find(currMaxNodeIndex) != nodeMap.end())
	{
		throw std::invalid_argument("Node with that ID already exists");
	}

	if (currMaxNodeIndex == none)
	{
		currMaxNodeIndex = findMaxNodeIndex(nodeMap);
	}

	nodeMap.insert({currMaxNodeIndex, adjList.size()});
	if (graphType == GraphType::undirected)
	{
		adjList.emplace_back(NodeInformation{std::size_t{0}, {}});
	}
	else
	{
		adjList.emplace_back(NodeInformation{Degree{}, {}});
	}

	return ++currMaxNodeIndex;
}

void AdjList::removeEdge(const EdgeCoord& edge)
{
	if (nodeMap.find(edge.source) == nodeMap.end())
	{
		throw std::invalid_argument(
			std::string("No node with given source ID: ") + std::to_string(edge.source));
	}

	auto& neighbours = adjList[nodeMap.find(edge.source)->second].second;
	std::ranges::remove_if(neighbours, [&edge](const auto& e) {
		return e.first.target == edge.target;});
}

void AdjList::removeNode(std::size_t node)
{
	auto itr = nodeMap.find(node);
	if (itr == nodeMap.end())
	{
		return;
	}

	std::ranges::for_each(nodeMap, [&itr](auto& node) {
		node.second = (node.second > itr->second) ? node.second - 1 : node.second;
	});
	adjList.erase(std::next(adjList.begin(), itr->second));
	nodeMap.erase(itr);
}

int AdjList::getEdgeWeight(const EdgeCoord& edge) const
{
	if (nodeMap.find(edge.source) == nodeMap.end())
	{
		throw std::invalid_argument("Source node of given ID does not exist");
	}

	auto& neighbours = adjList[nodeMap.at(edge.source)].second;
	auto itr = std::ranges::find_if(neighbours, [&edge](const auto& e) {
		return e.first.target == edge.target;
	});

	return (itr != neighbours.end()) ? itr->second : std::numeric_limits<int>::max();
}

std::vector<std::size_t> AdjList::getNeighboursOf(std::size_t node) const
{
	if (nodeMap.find(node) == nodeMap.end())
	{
		throw std::invalid_argument("Node of given ID does not exist");
	}

	std::vector<std::size_t> neighbours = {};
	std::ranges::for_each(adjList[nodeMap.at(node)].second, [&neighbours](const auto& edge) {
		neighbours.emplace_back(edge.first.target);
	});
	return neighbours;
}
} // namespace graph



/*uint32_t Graph::find_index(std::vector<Data::coord>& nodes, uint32_t x, uint32_t y)
{
	uint32_t size = nodes.size();
	uint32_t ret = 0xFFFFFFFF;
	for (uint32_t i = 0; i < size; i++)
	{
		if (nodes[i].x == x && nodes[i].y == y)
		{
			ret = i;
			break;
		}
	}
	return ret;
}

int32_t Graph::List::greedy_coloring(bool log)
{
	std::vector<int> v;
	std::map<int, int>::iterator itr;

	for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
	{
		v.push_back(itr->second);
	}
	shuffle(v, log);
	std::map<int, int> map;
	uint32_t count = v.size();
	for (uint32_t i = 0; i < count; i++)
	{
		map.insert(std::pair<int, int>(i, v[i]));
	}
	return this->greedy_coloring_core(&map, log);
}

int32_t Graph::List::greedy_coloring_core(std::map<int, int>* map, bool log)
{
	if (map == nullptr)
	{
		map = &(this->node_map);
	}
	uint32_t nodes_count = static_cast<uint32_t>(map->size());
	int32_t* nodes_colors = new int32_t[nodes_count];
	bool* colors_taken = new bool[nodes_count];

	for (uint32_t i = 0; i < nodes_count; i++)
	{
		nodes_colors[i] = -1;
	}

	std::map<int, int>::iterator itr = map->begin();
	nodes_colors[itr->second] = 0;
	uint32_t color;
	Node* ptr;

	for (++itr; itr != map->end(); itr++)
	{
			for (uint32_t i = 0; i < nodes_count; i++)
			{
				colors_taken[i] = false;
			}
		ptr = this->nodes[itr->second];

		while (ptr != nullptr)
		{
			if (nodes_colors[ptr->neighbour-1] > -1)
			{
				colors_taken[nodes_colors[ptr->neighbour-1]] = true;
			}

			ptr = ptr->next;
		}

		for (color = 0; colors_taken[color] == true; color++);
		nodes_colors[itr->second] = color;
	}

	int32_t max = -1;

	for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
	{
		if (log)
		{
			std::cout << "Node " << itr->second + 1 << " has color " << nodes_colors[itr->second] << std::endl;
		}
		if (nodes_colors[itr->second] > max) max = nodes_colors[itr->second];
	}
	if (log)
	{
		std::cout << "Amount of colors used: " << max + 1 << std::endl;
	}

	delete[] nodes_colors;
	delete[] colors_taken;

	return max + 1;
}

int32_t Graph::List::lf_coloring(bool log)
{
	int32_t deg = 0;
	uint32_t nodes_count = this->node_map.size();

	for (uint32_t i = 0; i < nodes_count; i++)
	{
		if (this->degrees[i] > deg) deg = this->degrees[i];
	}

	std::map<int, int> map;
	uint32_t map_index = 0;

	std::vector<int> v;

	std::map<int, int>::iterator itr;

	for (deg; deg > -1; deg--)
	{
		v.clear();
		for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
		{
			if (this->degrees[itr->second] == deg)
			{
				v.push_back(itr->second);
			}
		}
		this->shuffle(v, log);
		for (uint32_t i = 0; i < v.size(); i++)
		{
			map.insert(std::pair<int, int>(map_index, v[i]));
			map_index++;
		}
	}
	if (log)
	{
		for (itr = map.begin(); itr != map.end(); itr++)
		{
			std::cout << itr->second << " ";
		}
		std::cout << std::endl;
	}
	return this->greedy_coloring_core(&map, log);
}

int32_t Graph::List::sl_coloring(bool log)
{

	uint32_t nodes_count = this->node_map.size();
	int32_t* degrees = new int32_t[nodes_count];
	Node* ptr;
	std::map<int, int>::iterator itr;
	std::vector<int> v;
	std::vector<int> temp;
	std::map<int, int> map;
	uint32_t min;
	int32_t min_j;

	for (uint32_t i = 0; i < nodes_count; i++)
	{
		for (uint32_t j = 0; j < nodes_count; j++)
		{
			degrees[j] = -1;
		}
		v.clear();

		for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
		{
			if (std::find(std::begin(temp), std::end(temp), itr->second) == std::end(temp))
			{
				degrees[itr->second] = 0;
				ptr = this->nodes[itr->second];

				while (ptr != nullptr)
				{

					if (std::find(std::begin(temp), std::end(temp), ptr->neighbour - 1) == std::end(temp))
					{
						degrees[itr->second]++;
					}
					ptr = ptr->next;
				}
			}
		}

		min = 0xFFFFFFFF;
		for (int32_t j = 0; j < nodes_count; j++)
		{
			if (degrees[j] < min && degrees[j] > -1)
			{
				min = degrees[j];
			}
		}
		for (uint32_t j = 0; j < nodes_count; j++)
		{
			if (degrees[j] == min)
			{
				v.push_back(j);
			}
		}
		this->shuffle(v, log);
		temp.push_back(v[0]);
	}

    uint32_t map_index = 0;
	for (int32_t i = temp.size() - 1; i >= 0; i--)
	{
		map.insert(std::pair<int, int>(map_index, temp[i]));
		map_index++;
	}
	if (log)
	{
		for (itr = map.begin(); itr != map.end(); itr++)
		{
			std::cout << itr->second << " ";
		}
		std::cout << std::endl;
	}

	return this->greedy_coloring_core(&map, log);
}

void Graph::List::shuffle(std::vector<int>& v, bool log)
{
	uint32_t count = v.size();
	uint32_t index_a;
	uint32_t index_b;
	srand(time(NULL));
	uint32_t val;
	for (uint32_t i = 0; i < count; i++)
	{
		index_a = rand() % count;
		index_b = rand() % count;
		val = v[index_a];
		v[index_a] = v[index_b];
		v[index_b] = val;
	}
	if (log)
	{
		std::cout << "shuffle" << std::endl;
		for (uint32_t i = 0; i < count; i++)
		{
			std::cout << v[i] << " ";
		}
		std::cout << std::endl;
	}
}*/