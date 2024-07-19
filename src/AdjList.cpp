// this
#include <Graphs/AdjList.h>
#include <Graphs/Graph_matrix.h>

#include <fstream>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <cstring>

using namespace Graph;

namespace Graphs
{
AdjList::AdjList(std::string file_path)
	: degrees(nullptr),
	nodes(nullptr)
{
	std::fstream file;
	file.open(file_path, std::ios::in);
	
	uint32_t amount = 0;

	if (file.good())
	{
		std::string line;
		size_t pos = 0;

		// search through file to calculate the vertices count
		while (!file.eof())
		{
			std::getline(file, line);
			if (line != "")
			{
				amount++;
			}
		}
		
		// create list and array of degrees
		this->nodes = new Node* [amount];
		this->degrees = new uint32_t[amount];

		for (uint32_t i = 0; i < amount; i++)
		{
			this->nodes[i] = nullptr;
			this->degrees[i] = 0;
		}

		Node* ptr;
		Node** ptr2;

		// reopen the file to reset its cursor position
		// (seekg was not working here for undetermined reason)
		file.close();
		file.open(file_path, std::ios::in);

		// search through the file and load the whole list
		for(uint32_t i = 0; i < amount; i++)
		{
			std::getline(file, line);
			// protection from empty lines in the file
			if (line != "")
			{
				// insert the vertex to the vertex map
				this->node_map.insert(std::pair<int, int>(i + 1, i));
				pos = line.find(' ');
				ptr2 = &this->nodes[i];

				// search for spaces separating the neighbours
				while (pos != std::string::npos)
				{
					// cut off the unnecessary part of the line
					line = line.substr(pos + 1);

					// save neighbour to the list
					ptr = new Node;
					ptr->neighbour = atoi(line.c_str());
					ptr->next = nullptr;

					// move to the next neighbour
					*ptr2 = ptr;
					ptr2 = &((*ptr2)->next);
					this->degrees[i]++;

					pos = line.find(' ');
				}
				
			}
		}
	}
	// if file was not found
	else
	{
		std::cout << "File not found" << std::endl;
	}
	file.close();
}

AdjList::AdjList(::Graph::Matrix& matrix)
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

AdjList::AdjList(Data::Pixel_map& map)
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

	uint32_t size = static_cast<uint32_t>(nodes.size());
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
				ptr->neighbour = Data::find_index(nodes, coord.x - 1, coord.y) + 1;
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
				ptr->neighbour = Data::find_index(nodes, coord.x, coord.y - 1) + 1;
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

void AdjList::show() const
{
	std::cout << "Nodes amount = " << this->node_map.size() << std::endl;

	std::cout << "{" << std::endl;
	Node* ptr;
	for (auto itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
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

void AdjList::print_deg()
{
	std::map<int, int>::iterator itr;
	for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
	{
		std::cout << itr->first << ": " << this->degrees[itr->second] << std::endl;
	}
}

unsigned int AdjList::nodesAmount() const
{
	return static_cast<uint32_t>(this->node_map.size());
}

unsigned int AdjList::nodeDegree(NodeId node) const
{
	return this->degrees[this->node_map.find(node)->second];
}

uint32_t AdjList::get_neighbour(uint32_t x, uint32_t y)
{
	Node* ptr = this->nodes[this->node_map.find(x)->second];
	for (uint32_t i = 0; i < y - 1; i++)
	{
		ptr = ptr->next;
	}
	return ptr->neighbour;
}

int32_t AdjList::greedy_coloring_core(std::map<int, int>* map, bool log)
{
	if (map == nullptr) //sta�a kt�ra okre�la pointer wskazuj�cy na nulla
	{
		map = &(this->node_map);//bierze map� zaszyt� z samej implementacji stdmap - map� w�z��w
	}
	uint32_t nodes_count = static_cast<uint32_t>(map->size()); // pobiera ilo�� wierzccho�k�w
	int32_t* nodes_colors = new int32_t[nodes_count]; // tablica kolor�w wierzcho�k�w
	bool* colors_taken = new bool[nodes_count]; // tablica u�ytych kolor�w

	for (uint32_t i = 0; i < nodes_count; i++) // dla wszystkich wierzccho�k�w ustawia -1 jako kolor
	{
		nodes_colors[i] = -1;
	}

	std::map<int, int>::iterator itr = map->begin();
	nodes_colors[itr->second] = 0; // dla pierwszego wierzcho�ka ustawia kolor 0
	uint32_t color;
	Node* ptr;
	
	//iteruje przez map� wierzccho�k�w
	for (++itr; itr != map->end(); itr++)
	{
			for (uint32_t i = 0; i < nodes_count; i++) // dla ka�dego wierzcho�ka zapisanie wszystkich kolor�ow jako dost�pnych
			{
				colors_taken[i] = false;
			}
		ptr = this->nodes[itr->second]; // we wska�niku zapisanie odniesienia do aktualnego wierzcho�ka

		//nodes to jest lista.  Itr->second to warto�� iindexu zapisana w danym wpisie listy.
		while (ptr != nullptr)// iteracja po s�siadacch wierzcho�ka
		{
			if (nodes_colors[ptr->neighbour-1] > -1) // je�eli s�siad ma kolor r�ny od -1 to zaznacza kolor s�siada jako zaj�ty
			{
				colors_taken[nodes_colors[ptr->neighbour-1]] = true;
			}
			 

			ptr = ptr->next; // przej�ie do nast�pnego s�siada
		}

		for (color = 0; colors_taken[color] == true; color++); // wyszukiwanie najmniejszego dost�pnego koloru
		nodes_colors[itr->second] = color; // pokolorowanie wierzcho�ka
	}

	int32_t max = -1;

	//wypisanie kolor�w dla poszczeg�lnych wierzcho�k�w
	
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

	return max + 1; // return zwraca ilo�� u�ytych kolor�w
}


int32_t AdjList::greedy_coloring(bool log)
{
	std::vector<int> v;
	std::map<int, int>::iterator itr;

	if (log)
	{
		std::cout << "Greedy coloring" << std::endl;
	}

	for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
	{
		v.push_back(itr->second); // wpisuje wierzcho�ki do wektora permutacji
	}
	shuffle(v, log); // wygeneruj permutacje

	std::map<int, int> map; // wpisz do mapy w nowej kolejnosci

	size_t count = v.size();
	for (size_t i = 0; i < count; i++)
	{
		map.insert(std::pair<int, int>(i, v[i]));
	}
	return this->greedy_coloring_core(&map, log); // przeslij do greedy coloring core
}

// algorytmy LF i SL  tworz� mapy posegregowane w odpowiedniaj kolejno�ci po kt�rych p�niej iteruje greedy. Mapa to lista z wierzcho�k�w posegregowana odpowiednio,
// mapa s�u�y do tego by jedn� warto�� mapowa� na inn�. W tym wypadku po kolejno�ci wpis�w z mapy program iteruje po kolejno�ci wpis�w w mapie.
// Mapa jest tak� nak�adk� na list� -  program porusza si� po mapie, a sama lista pozostaje niezmienna by jej indeks�w nie rozwali� przypadkiem.
// Mapa zawiera w sobie klucz i warto�� - to s� pary.
int32_t AdjList::lf_coloring(bool log)
{
	size_t deg = 0; // zmienna przechowuj�a aktualnie rozpatrywany stopie�
	size_t nodes_count = this->node_map.size();

	if (log)
	{
		std::cout << "LF coloring" << std::endl;
	}

	//iteruje po li�cie szukaj�c najwi�kszego stopnia
	for (size_t i = 0; i < nodes_count; i++)
	{
		if (this->degrees[i] > deg) deg = this->degrees[i];
	}

	//tworzy podstawowe zmienne
	std::map<int, int> map; // mapa docelowa
	uint32_t map_index = 0;

	std::vector<int> v;

	std::map<int, int>::iterator itr;

	//iteruje po stopniach  w d�
	for (deg; deg > -1; deg--)
	{
		v.clear();
		for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++) //przeszukuje ca�� list� w poszukiwaniu wierzcho�ka o danym stopniu
		{
			if (this->degrees[itr->second] == deg) // je�li znalaz�o wiewerzcho�ek, wpisuje go do wektora
			{
				v.push_back(itr->second);
			}
		}
		// gdy wszystkie wierzcho�ki o zadanym stopniu s� wpisane do wektora, 
		// generuje permutacj� dla nich a nast�pnie wpisuje je w nowej kolejno�ci do mapy
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
	return this->greedy_coloring_core(&map, log); // przes�anie do greedy coloring i zwr�cenie ilo�ci u�ytych kolor�w
}


//WA�NE: Wpisuj�c wierzcho�ek do 'tmp' pomijamy go w dalszym przeszukiwaniu, efektywnie przeszukuj�c podgraf indukowany przez wierzcho�ki z wy��czeniem wierzcho�k�w w 'tmp'

//SL w tym wydaniu po prostu pomija przy wyszukiwaniu w podgrafu ju� znalezione wierzcho�ki w poprzedniej iteracji, nie zmieniaj�c przy tym zapisu grafu w pami�ci.
int32_t AdjList::sl_coloring(bool log)
{
	if (log)
	{
		std::cout << "SL coloring" << std::endl;
	}

	size_t nodes_count = this->node_map.size();//ilo�� wierzcho�k�w
	int32_t* degrees = new int32_t[nodes_count];//pomocnicza tablica stopni
	Node* ptr; //pointer
	std::map<int, int>::iterator itr; //iterator po mapie
	std::vector<int> v;
	std::vector<int> temp; // wektor pomocniczy przechowuj�cy wierzcho�ki w odwrotnnej kolejno�ci do zamierzonej
	std::map<int, int> map; // mapa docelowa
	uint32_t min; // minimalny stopie� 
	int32_t min_j; // index wierzccho�ka o minimalnym stopniu

	//Przeszukuje wszystkie wierzcho�ki w grafie.
	for (uint32_t i = 0; i < nodes_count; i++)
	{
		for (uint32_t j = 0; j < nodes_count; j++)//ustawianie stopni na domy�ln� warto��c -1
		{
			degrees[j] = -1;
		}
		v.clear();

		//  ---- CZʌC PIERWSZA -- wyliczanie stopnii wierzcholk�w
		//iterowanie po mapie wierzcho�k�w
		for (itr = this->node_map.begin(); itr != this->node_map.end(); itr++)
		{
			//Je�eli wierzcho�ek nie znajduje si� w  wektorze pomocniczym 
			if (std::ranges::find(temp, itr->second) == std::end(temp))
			{
				degrees[itr->second] = 0; // za stopie� podstawia 0
				ptr = this->nodes[itr->second]; // ustawia wska�nik na aktualny wierzcho�ek

				while (ptr != nullptr)//oblicza stopie� wierzcho�ka iteruj�c po s�siadach
				{

					//je�eli s�siadd nie jest wykluczony z grafu poprzez wpisanie do wektora pomocniczego to zwi�kksz stopie� o jeden
					if (std::find(std::begin(temp), std::end(temp), ptr->neighbour - 1) == std::end(temp)) 
					{
						degrees[itr->second]++;
					}
					ptr = ptr->next; // przejd� do nast�pnego s�siada
				}
			}
		}
		// ---- Koniec cz�i pierwszejj  - w danej iteracji wylicza� wszystkie stopnie.
		// 
		// 
		// ------ CZʌ� II --  "usuni�cie" wierzccho�ka z grafu, tworz�cc graf indukowany przez wierzcho�ki

		min = 0xFFFFFFFF; // nadanie domy�lnej warto�cci minimum, kt�ra musi zosta� zmieniona, poprzez celowe nadanie najwi�kszej warto�ci dla unsinged-int32
		for (int32_t j = 0; j < nodes_count; j++)
		{
			if (degrees[j] < min && degrees[j] > -1)//wyszukiwanie najmniejszego stopnia i wierzcho�ka z tym stopniem
			{
				min = degrees[j];
			}
		}
		for (uint32_t j = 0; j < nodes_count; j++)
		{
			if (degrees[j] == min) // je�eli stopie� wierzcho�ka odpowiada stopniowi, wierzcho�ek umieszcza w wektorze
			{
				v.push_back(j);
			}
		}

		

		// dokonuje permutacji wektora
		this->shuffle(v, log);
		temp.push_back(v[0]);// do temp wektora wpisuje pierwszy element wektora permutaji
	}
	//------ KONIEC CZʌCI II - 


	//--- zapisanie odwrotno�ci wektora tmp w mapie aby uzyska� oczekiwan� kolejno�� dla algorytmu SL
	// bo algorytm co prawda wyszukuje najmniejsze wierzccho�ki, ale zapisuje je w "tmp" pd  "lewej do prawej" a potrzeba mie� zapis "od prawej do lewej"
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

	return this->greedy_coloring_core(&map, log); // przes�anie mapy do algorytmu greedy i zwr�cenie ilo�ci u�ytych kolor�w
}

void AdjList::setEdge(EdgeInfo edge)
{
	uint32_t index = this->node_map.find(edge.source)->second;
	Node* ptr = this->nodes[index];
	if (ptr != nullptr)
	{
		while (ptr->next != nullptr)
		{
			ptr = ptr->next;
		}
		ptr->next = new Node;
		ptr->next->neighbour = edge.destination;
		ptr->next->next = nullptr;
		this->degrees[index]++;
	}
	else
	{
		ptr = new Node;
		ptr->neighbour = edge.destination;
		ptr->next = nullptr;
		this->degrees[index]++;
	}

	index = this->node_map.find(edge.destination)->second;
	ptr = this->nodes[index];
	if (ptr != nullptr)
	{
		while (ptr->next != nullptr)
		{
			ptr = ptr->next;
		}
		ptr->next = new Node;
		ptr->next->neighbour = edge.source;
		ptr->next->next = nullptr;
		this->degrees[index]++;
	}
	else
	{
		ptr = new Node;
		ptr->neighbour = edge.source;
		ptr->next = nullptr;
		this->degrees[index]++;
	}
}

void AdjList::removeEdge(EdgeInfo edge)
{
	uint32_t index = this->node_map.find(edge.source)->second;
	Node* ptr = this->nodes[index];
	Node* next;
	if (ptr != nullptr)
	{
		while (ptr->next != nullptr && ptr->next->neighbour != edge.destination)
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
	
	index = this->node_map.find(edge.destination)->second;
	ptr = this->nodes[index];
	if (ptr != nullptr)
	{
		while (ptr->next != nullptr && ptr->next->neighbour != edge.source)
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

void AdjList::addNode()
{
	uint32_t amount = static_cast<uint32_t>(this->node_map.size());
	Node** ptr = new Node * [amount + static_cast<unsigned long long>(1)];
	std::memcpy(ptr, this->nodes, sizeof(Node*) * amount);
	
	
	uint32_t* deg_ptr = new uint32_t[amount + static_cast<unsigned long long>(1)];
	std::memcpy(deg_ptr, this->degrees, sizeof(uint32_t) * amount);

	ptr[amount] = nullptr;
	deg_ptr[amount] = 0;

	this->node_map.insert(std::pair<int, int>(amount + 1, amount));

	delete[] this->degrees;
	delete[] this->nodes;
	
	this->degrees = deg_ptr;
	this->nodes = ptr;
}

void AdjList::removeNode(NodeId node)
{
	std::map<int, int>::iterator itr = this->node_map.find(node);
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

		this->node_map.erase(node);
	}
}

AdjList::~AdjList()
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

void AdjList::shuffle(std::vector<int>& v, bool log)
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
}

} // namespace Graphs