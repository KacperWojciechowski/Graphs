#include "Graph_list.h"

#include <fstream>
#include <iostream>

Graph::List::List(std::string file_path)
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
}

Graph::List::List(Data::Pixel_map& map)
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
}

void Graph::List::print()
{
	std::cout << "Nodes amount = " << this->node_map.size() << std::endl;

	std::cout << "{" << std::endl;
	Node* ptr;
	std::map<int, int>::iterator itr;
	for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
	{
		ptr = this->nodes[itr->second];
		std::cout << itr->first << ": ";
		while (ptr != nullptr)
		{
			std::cout << ptr->neighbour << ", ";
			ptr = ptr->next;
		}
		std::cout << std::endl;
	}
	std::cout << "}" << std::endl;
}

void Graph::List::print_deg()
{
	std::map<int, int>::iterator itr;
	for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
	{
		std::cout << itr->first << ": " << this->degrees[itr->second] << std::endl;
	}
}

uint32_t Graph::List::get_nodes_amount()
{
	return static_cast<uint32_t>(this->node_map.size());
}

uint32_t Graph::List::get_node_degree(uint32_t node)
{
	return this->degrees[this->node_map.find(node)->second];
}

uint32_t Graph::List::get_neighbour(uint32_t x, uint32_t y)
{
	Node* ptr = this->nodes[this->node_map.find(x)->second];
	for (uint32_t i = 0; i < y - 1; i++)
	{
		ptr = ptr->next;
	}
	return ptr->neighbour;
}

void Graph::List::add_edge(uint32_t source, uint32_t destination)
{
	uint32_t index = this->node_map.find(source)->second;
	Node* ptr = this->nodes[index];
	if (ptr != nullptr)
	{
		while (ptr->next != nullptr)
		{
			ptr = ptr->next;
		}
		ptr->next = new Node;
		ptr->next->neighbour = destination;
		ptr->next->next = nullptr;
		this->degrees[index]++;
	}
	else
	{
		ptr = new Node;
		ptr->neighbour = destination;
		ptr->next = nullptr;
		this->degrees[index]++;
	}

	index = this->node_map.find(destination)->second;
	ptr = this->nodes[index];
	if (ptr != nullptr)
	{
		while (ptr->next != nullptr)
		{
			ptr = ptr->next;
		}
		ptr->next = new Node;
		ptr->next->neighbour = source;
		ptr->next->next = nullptr;
		this->degrees[index]++;
	}
	else
	{
		ptr = new Node;
		ptr->neighbour = source;
		ptr->next = nullptr;
		this->degrees[index]++;
	}
}

void Graph::List::remove_edge(uint32_t source, uint32_t destination)
{
	uint32_t index = this->node_map.find(source)->second;
	Node* ptr = this->nodes[index];
	Node* next;
	if (ptr != nullptr)
	{
		while (ptr->next != nullptr && ptr->next->neighbour != destination)
		{
			ptr = ptr->next;
		}
		if (ptr->next != nullptr)
		{
			next = ptr->next->next;
			delete ptr->next;
			ptr->next = next;
			this->degrees[index]--;
		}
	}
	
	index = this->node_map.find(destination)->second;
	ptr = this->nodes[index];
	if (ptr != nullptr)
	{
		while (ptr->next != nullptr && ptr->next->neighbour != source)
		{
			ptr = ptr->next;
		}
		if (ptr->next != nullptr)
		{
			next = ptr->next->next;
			delete ptr->next;
			ptr->next = next;
			this->degrees[index]--;
		}
	}
}

void Graph::List::add_node()
{
	uint32_t amount = static_cast<uint32_t>(this->node_map.size());
	Node** ptr = new Node * [amount + static_cast<unsigned __int64>(1)];
	memcpy(ptr, this->nodes, sizeof(Node*) * amount);
	
	
	uint32_t* deg_ptr = new uint32_t[amount + static_cast<unsigned __int64>(1)];
	memcpy(deg_ptr, this->degrees, sizeof(uint32_t) * amount);

	ptr[amount] = nullptr;
	deg_ptr[amount] = 0;

	this->node_map.insert(std::pair<int, int>(amount + 1, amount));

	delete[] this->degrees;
	delete[] this->nodes;
	
	this->degrees = deg_ptr;
	this->nodes = ptr;
}

void Graph::List::remove_node(uint32_t node_id)
{
	std::map<int, int>::iterator itr = this->node_map.find(node_id);
	if (itr != this->node_map.end())
	{
		Node* ptr = this->nodes[itr->second];
		Node* next;

		if (ptr != nullptr)
		{
			next = ptr->next;
			while (ptr != nullptr)
			{
				delete ptr;
				ptr = next;
				if (ptr != nullptr)
				{
					next = ptr->next;
				}
			}
		}

		this->node_map.erase(node_id);
	}
}

Graph::List::~List()
{
	Node* ptr;
	Node* next;
	uint32_t amount = static_cast<uint32_t>(this->node_map.size());
	std::map<int, int>::iterator itr;
	for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
	{
		ptr = this->nodes[itr->second];
		if (ptr != nullptr)
		{
			next = ptr->next;

			while (ptr != nullptr)
			{
				delete ptr;
				ptr = next;
				if (ptr != nullptr)
				{
					next = ptr->next;
				}
			}
		}
	}
	delete[] this->nodes;
	delete[] this->degrees;
}

uint32_t Graph::find_index(std::vector<Data::coord>& nodes, uint32_t x, uint32_t y)
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
