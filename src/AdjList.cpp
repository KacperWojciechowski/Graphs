// this
#include <algorithm>
#include <cstring>
#include <fstream>
#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
#include <iostream>
#include <sstream>
#include <time.h>

namespace
{
std::vector<Data::coord> extractPixelMapNodes(const Data::Pixel_map& map) {
    std::vector<Data::coord> pixelMapNodes;

    for (uint32_t i = 0; i < map.get_rows(); i++)
    {
        for (uint32_t j = 0; j < map.get_columns(); j++)
        {
            if (map.get_field(i, j) == 0)
            {
                pixelMapNodes.push_back({i, j});
            }
        }
    }

    return pixelMapNodes;
}

void insertNeighborIfApplicable(auto checker, auto inserter, Data::coord coordsToInsert) {
    if (checker()) // doesnt underflow
    {
        inserter(coordsToInsert);
    }
}
} // namespace

namespace Graphs
{
AdjList::AdjList(std::string file_path) {
    std::fstream file;
    file.open(file_path, std::ios::in);

    if (file.good())
    {
        std::string line;
        size_t pos = 0;

        {
            auto amount = 0u;
            // search through file to calculate the vertices count
            while (!file.eof())
            {
                std::getline(file, line);
                if (line != "")
                {
                    amount++;
                }
            }
            nodes.resize(amount);
        }

        // reopen the file to reset its cursor position
        // (seekg was not working here for undetermined reason)
        file.close();
        file.open(file_path, std::ios::in);

        // search through the file and load the whole list
        for (uint32_t i = 0; i < nodes.size(); i++)
        {
            std::getline(file, line);
            // protection from empty lines in the file
            if (line != "")
            {
                // insert the vertex to the vertex map
                this->nodeMap.insert(std::pair<uint32_t, uint32_t>(i + 1, i));
                pos = line.find(' ');
                auto& node = nodes[i];

                // search for spaces separating the neighbours
                while (pos != std::string::npos)
                {
                    // cut off the unnecessary part of the line
                    line = line.substr(pos + 1);
                    node.emplace_back(std::stoi(line));
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

AdjList::AdjList(const Graph& graph) {
    nodes.resize(graph.nodesAmount());

    for (uint32_t i = 0; i < nodes.size(); i++)
    {
        auto& node = nodes[i];
        for (uint32_t j = 0; j < nodes.size(); j++)
        {
            auto edge = graph.findEdge({i + 1, j + 1});
            while (edge.weight.has_value() and edge.weight.value() > 0)
            {
                node.emplace_back(j + 1);
                edge.weight.value()--;
            }
        }
        nodeMap.insert(std::pair<uint32_t, uint32_t>(i + 1, i));
    }
}

AdjList::AdjList(const Data::Pixel_map& map) {
    auto pixelMapNodes = extractPixelMapNodes(map);
    nodes.resize(pixelMapNodes.size());

    for (uint32_t i = 0; i < pixelMapNodes.size(); i++)
    {
        auto coord = pixelMapNodes[i];
        auto& node = nodes[i];
        nodeMap.insert(std::pair<uint32_t, uint32_t>(i + 1, i));

        auto inserter = [&node, &pixelMapNodes](Data::coord coord) {
            node.emplace_back(Data::find_index(pixelMapNodes, coord.x, coord.y) + 1);
        };
        auto leftChecker = [coord]() {
            return coord.x - 1 < 0xFFFFFFFF;
        }; // does not underflow
        auto upChecker = [coord]() {
            return coord.y - 1 < 0xFFFFFFFF;
        }; // does not underflow
        auto rightChecker = [coord, &map]() {
            return coord.x + 1 < map.get_columns();
        };
        auto downChecker = [coord, &map]() {
            return coord.y + 1 < map.get_rows();
        };

        insertNeighborIfApplicable(upChecker, inserter, {coord.x, coord.y - 1});
        insertNeighborIfApplicable(leftChecker, inserter, {coord.x - 1, coord.y});
        insertNeighborIfApplicable(rightChecker, inserter, {coord.x + 1, coord.y});
        insertNeighborIfApplicable(downChecker, inserter, {coord.x, coord.y + 1});

        std::ranges::sort(node);
    }
}

std::string AdjList::show() const {
    std::stringstream outStream;
    outStream << "Nodes amount = " << nodeMap.size() << "\n{\n";

    for (auto& nodeMapping : nodeMap)
    {
        outStream << nodeMapping.first << ": ";
        for (auto& neighbor : nodes[nodeMapping.second])
        {
            outStream << neighbor << ", ";
        }
        outStream << "\n";
    }
    outStream << "}\n";
    return outStream.str();
}

uint32_t AdjList::nodesAmount() const {
    return static_cast<uint32_t>(this->nodeMap.size());
}

uint32_t AdjList::nodeDegree(NodeId node) const {
    return nodes[nodeMap.find(node)->second].size();
}

std::vector<NodeId> AdjList::getNeighborsOf(NodeId node) const {
    auto nodeMapping = nodeMap.find(node);
    if (nodeMapping == nodeMap.end())
    {
        return {};
    }

    return nodes[nodeMapping->second];
}

// // algorytmy LF i SL  tworz� mapy posegregowane w odpowiedniaj kolejno�ci po
// kt�rych p�niej iteruje greedy. Mapa to lista z wierzcho�k�w posegregowana
// odpowiednio,
// // mapa s�u�y do tego by jedn� warto�� mapowa� na inn�. W tym wypadku po
// kolejno�ci wpis�w z mapy program iteruje po kolejno�ci wpis�w w mapie.
// // Mapa jest tak� nak�adk� na list� -  program porusza si� po mapie, a sama
// lista pozostaje niezmienna by jej indeks�w nie rozwali� przypadkiem.
// // Mapa zawiera w sobie klucz i warto�� - to s� pary.
// int32_t AdjList::lf_coloring(bool log)
// {
// 	size_t deg = 0; // zmienna przechowuj�a aktualnie rozpatrywany stopie�
// 	size_t nodes_count = this->node_map.size();

// 	if (log)
// 	{
// 		std::cout << "LF coloring" << std::endl;
// 	}

// 	//iteruje po li�cie szukaj�c najwi�kszego stopnia
// 	for (size_t i = 0; i < nodes_count; i++)
// 	{
// 		if (this->degrees[i] > deg) deg = this->degrees[i];
// 	}

// 	//tworzy podstawowe zmienne
// 	std::map<int, int> map; // mapa docelowa
// 	uint32_t map_index = 0;

// 	std::vector<int> v;

// 	std::map<int, int>::iterator itr;

// 	//iteruje po stopniach  w d�
// 	for (deg; deg > -1; deg--)
// 	{
// 		v.clear();
// 		for (itr = this->node_map.begin(); itr != this->node_map.end();
// itr++) //przeszukuje ca�� list� w poszukiwaniu wierzcho�ka o danym stopniu
// 		{
// 			if (this->degrees[itr->second] == deg) // je�li znalaz�o
// wiewerzcho�ek, wpisuje go do wektora
// 			{
// 				v.push_back(itr->second);
// 			}
// 		}
// 		// gdy wszystkie wierzcho�ki o zadanym stopniu s� wpisane do
// wektora,
// 		// generuje permutacj� dla nich a nast�pnie wpisuje je w nowej
// kolejno�ci do mapy 		this->shuffle(v, log); 		for (uint32_t i
// = 0; i < v.size(); i++)
// 		{
// 			map.insert(std::pair<int, int>(map_index, v[i]));
// 			map_index++;
// 		}
// 	}
// 	if (log)
// 	{
// 		for (itr = map.begin(); itr != map.end(); itr++)
// 		{
// 			std::cout << itr->second << " ";
// 		}
// 		std::cout << std::endl;
// 	}
// 	return this->greedy_coloring_core(&map, log); // przes�anie do greedy
// coloring i zwr�cenie ilo�ci u�ytych kolor�w
// }

// //WA�NE: Wpisuj�c wierzcho�ek do 'tmp' pomijamy go w dalszym przeszukiwaniu,
// efektywnie przeszukuj�c podgraf indukowany przez wierzcho�ki z wy��czeniem
// wierzcho�k�w w 'tmp'

// //SL w tym wydaniu po prostu pomija przy wyszukiwaniu w podgrafu ju�
// znalezione wierzcho�ki w poprzedniej iteracji, nie zmieniaj�c przy tym zapisu
// grafu w pami�ci. int32_t AdjList::sl_coloring(bool log)
// {
// 	if (log)
// 	{
// 		std::cout << "SL coloring" << std::endl;
// 	}

// 	size_t nodes_count = this->node_map.size();//ilo�� wierzcho�k�w
// 	int32_t* degrees = new int32_t[nodes_count];//pomocnicza tablica stopni
// 	Node* ptr; //pointer
// 	std::map<int, int>::iterator itr; //iterator po mapie
// 	std::vector<int> v;
// 	std::vector<int> temp; // wektor pomocniczy przechowuj�cy wierzcho�ki w
// odwrotnnej kolejno�ci do zamierzonej 	std::map<int, int> map; // mapa
// docelowa 	uint32_t min; // minimalny stopie� 	int32_t min_j; // index
// wierzccho�ka o minimalnym stopniu

// 	//Przeszukuje wszystkie wierzcho�ki w grafie.
// 	for (uint32_t i = 0; i < nodes_count; i++)
// 	{
// 		for (uint32_t j = 0; j < nodes_count; j++)//ustawianie stopni na
// domy�ln� warto��c -1
// 		{
// 			degrees[j] = -1;
// 		}
// 		v.clear();

// 		//  ---- CZʌC PIERWSZA -- wyliczanie stopnii wierzcholk�w
// 		//iterowanie po mapie wierzcho�k�w
// 		for (itr = this->node_map.begin(); itr != this->node_map.end();
// itr++)
// 		{
// 			//Je�eli wierzcho�ek nie znajduje si� w  wektorze
// pomocniczym 			if (std::ranges::find(temp, itr->second) ==
// std::end(temp))
// 			{
// 				degrees[itr->second] = 0; // za stopie�
// podstawia 0 				ptr = this->nodes[itr->second]; //
// ustawia wska�nik na aktualny wierzcho�ek

// 				while (ptr != nullptr)//oblicza stopie�
// wierzcho�ka iteruj�c po s�siadach
// 				{

// 					//je�eli s�siadd nie jest wykluczony z
// grafu poprzez wpisanie do wektora pomocniczego to zwi�kksz stopie� o jeden
// if (std::find(std::begin(temp), std::end(temp), ptr->neighbour - 1) ==
// std::end(temp))
// 					{
// 						degrees[itr->second]++;
// 					}
// 					ptr = ptr->next; // przejd� do
// nast�pnego s�siada
// 				}
// 			}
// 		}
// 		// ---- Koniec cz�i pierwszejj  - w danej iteracji wylicza�
// wszystkie stopnie.
// 		//
// 		//
// 		// ------ CZʌ� II --  "usuni�cie" wierzccho�ka z grafu, tworz�cc
// graf indukowany przez wierzcho�ki

// 		min = 0xFFFFFFFF; // nadanie domy�lnej warto�cci minimum, kt�ra
// musi zosta� zmieniona, poprzez celowe nadanie najwi�kszej warto�ci dla
// unsinged-int32 		for (int32_t j = 0; j < nodes_count; j++)
// 		{
// 			if (degrees[j] < min && degrees[j] > -1)//wyszukiwanie
// najmniejszego stopnia i wierzcho�ka z tym stopniem
// 			{
// 				min = degrees[j];
// 			}
// 		}
// 		for (uint32_t j = 0; j < nodes_count; j++)
// 		{
// 			if (degrees[j] == min) // je�eli stopie� wierzcho�ka
// odpowiada stopniowi, wierzcho�ek umieszcza w wektorze
// 			{
// 				v.push_back(j);
// 			}
// 		}

// 		// dokonuje permutacji wektora
// 		this->shuffle(v, log);
// 		temp.push_back(v[0]);// do temp wektora wpisuje pierwszy element
// wektora permutaji
// 	}
// 	//------ KONIEC CZʌCI II -

// 	//--- zapisanie odwrotno�ci wektora tmp w mapie aby uzyska� oczekiwan�
// kolejno�� dla algorytmu SL
// 	// bo algorytm co prawda wyszukuje najmniejsze wierzccho�ki, ale
// zapisuje je w "tmp" pd  "lewej do prawej" a potrzeba mie� zapis "od prawej do
// lewej" 	uint32_t map_index = 0; 	for (int32_t i = temp.size() -
// 1; i >= 0; i--)
// 	{
// 		map.insert(std::pair<int, int>(map_index, temp[i]));
// 		map_index++;
// 	}
// 	if (log)
// 	{
// 		for (itr = map.begin(); itr != map.end(); itr++)
// 		{
// 			std::cout << itr->second << " ";
// 		}
// 		std::cout << std::endl;
// 	}

// 	return this->greedy_coloring_core(&map, log); // przes�anie mapy do
// algorytmu greedy i zwr�cenie ilo�ci u�ytych kolor�w
// }

void AdjList::addNeighborAndSortRange(Neighbors& range, NodeId tgtNeighbor) {
    range.emplace_back(tgtNeighbor);
    std::ranges::sort(range);
}

void AdjList::setEdge(const EdgeInfo& edge) {
    auto sourceNodeMapping = nodeMap.find(edge.source);
    auto destinationNodeMapping = nodeMap.find(edge.destination);

    if (sourceNodeMapping == nodeMap.end() or destinationNodeMapping == nodeMap.end())
    {
        return;
    }

    addNeighborAndSortRange(nodes[sourceNodeMapping->second], edge.destination);
    addNeighborAndSortRange(nodes[destinationNodeMapping->second], edge.source);
}

void AdjList::removeNeighborFromRange(Neighbors& range, NodeId tgtNeighbor) {
    std::ranges::remove_if(range, [tgtNeighbor](auto& neighbor) {
        return neighbor == tgtNeighbor;
    });
}

void AdjList::removeEdge(const EdgeInfo& edge) {
    auto sourceNodeMapping = nodeMap.find(edge.source);
    auto destinationNodeMapping = nodeMap.find(edge.destination);

    if (sourceNodeMapping == nodeMap.end() or destinationNodeMapping == nodeMap.end())
    {
        return;
    }

    removeNeighborFromRange(nodes[sourceNodeMapping->second], edge.destination);
    removeNeighborFromRange(nodes[destinationNodeMapping->second], edge.source);
}

void AdjList::addNode() {
    auto highestId = nodeMap.rbegin()->first;

    nodes.emplace_back();
    nodeMap.insert(std::pair<uint32_t, uint32_t>(highestId + 1, nodes.size() - 1));
}

void AdjList::removeNode(NodeId node) {
    auto nodeMapping = nodeMap.find(node);

    if (nodeMapping == nodeMap.end())
    {
        return;
    }

    std::ranges::remove_if(nodes, [this, &nodeMapping](auto& neighbors) {
        return neighbors == nodes[nodeMapping->second];
    });
    nodeMap.erase(nodeMapping);
}

EdgeInfo AdjList::findEdge(const EdgeInfo& edge) const {
    auto source = nodeMap.find(edge.source);
    auto destination = nodeMap.find(edge.destination);

    if (source == this->nodeMap.end() or destination == this->nodeMap.end())
    {
        return {edge.source, edge.destination, std::nullopt};
    }

    auto neighbor = std::ranges::find(nodes[edge.source], edge.destination);

    if (neighbor == std::end(nodes[edge.source]))
    {
        return {edge.source, edge.destination, std::nullopt};
    }

    return {edge.source, edge.destination, 1};
}

std::vector<NodeId> AdjList::getNodeIds() const {
    std::vector<NodeId> nodeIds;
    for (const auto& nodeMapping : nodeMap)
    {
        nodeIds.push_back(nodeMapping.first);
    }
    return nodeIds;
}
} // namespace Graphs
