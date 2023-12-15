#include "Graph_matrix.h"

#include <iostream>
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>
//#include <algorithm>

Graph::Matrix::Matrix(std::string file_path, std::string ng, std::string type)
	: matrix(nullptr),
	  nodes_amount(0),
	  nazwa_grafu(ng),
	  typ_grafu(type)
{
	std::ifstream file(file_path, std::ios::in);
	if (file.is_open() && file_path.find(".mat",0) != std::string::npos)
	{
		std::string line;
		uint32_t vertexes = 0;

		std::getline(file, line);
		size_t offset = 0;
		size_t pos = 0;
		
		while (true)
		{
			pos = line.find(' ', offset);
			if (pos != std::string::npos)
			{
				vertexes++;
				offset = pos + 1;
			}
			else
			{
				break;
			}
		}
		vertexes++;
	
		this->nodes_amount = vertexes;
		this->matrix = new uint32_t * [this->nodes_amount];
		for (uint32_t i = 0; i < this->nodes_amount; i++)
		{
			this->matrix[i] = new uint32_t[this->nodes_amount];
		}

		file.seekg(std::ios_base::beg);

		uint32_t temp;
		for (uint32_t i = 0; i < this->nodes_amount; i++)
		{
			for (uint32_t j = 0; j < this->nodes_amount; j++)
			{
				file >> temp;
				this->matrix[i][j] = temp;
			}
		}
	}
	else if (file.good())
	{

		std::cout << "Zaczynam Prace" << std::endl;
		using namespace std;
		//===================== TAKA "PROTEZA" NA KOD, KTÓRA OBLICZA N BY BYLO STATYCZNE
		
		ifstream odczyt2(file_path);
		string linia2;
		vector <string> istnieja2{};
		int ile_wierzcholkow = 0;
		while (getline(odczyt2, linia2))
		{
			std::string::size_type pos = linia2.find("node");
			if (pos != string::npos) //Je¿eli jest to wierzcho³ek, to zazyna zczytywanie wierzcho³ka
			{
				string::size_type pos2 = linia2.find("=n");
				string::size_type pos3 = linia2.find("\"/>");
				pos3 = pos3 - pos2 - 2;
				string id = linia2.substr(pos2 + 2, pos3);


				// Sprawdza czy dane id jest ju¿ w vectorze, je¿eli nie to dopisuje je
				if (find(istnieja2.begin(), istnieja2.end(), id) == istnieja2.end())
				{
					istnieja2.push_back(id);
				}

			}

		}

		odczyt2.close();
		this->nodes_amount = static_cast<uint32_t>(istnieja2.size());
		std::cout << "Liczba wierzcho³ków: " << this->nodes_amount << std::endl;
		//==========================================================WLASCIWE OBLICZENIA

		string linia;
		ifstream odczyt(file_path);
		int sekwencja = 0; // 0-poczatek   1-wczytuje nody    2- buduje macierz   3- wczytuje krawedzie

		this->matrix = new uint32_t * [this->nodes_amount];

		for (uint32_t i = 0; i < this->nodes_amount; i++)
		{
			this->matrix[i] = new uint32_t[this->nodes_amount];
			for (uint32_t j = 0; j < this->nodes_amount; j++)
			{
				this->matrix[i][j] = 0;
			}
		}		

		while (getline(odczyt, linia))
		{
			string::size_type pos = linia.find("node");
			string::size_type pos_graph_id = linia.find("graph id=");
			if (pos_graph_id != string::npos)
			{
				string::size_type pos_graph_type = linia.find("edgedefault=");
				nazwa_grafu = linia.substr(pos_graph_id + 10, pos_graph_type - 14);
				cout << "Nazwa grafu: " << nazwa_grafu << endl;

				string::size_type pos3 = linia.find("\">");
				typ_grafu = linia.substr(pos_graph_type + 13, pos3 - (pos_graph_type + 13));
				cout << "Typ grafu: " << typ_grafu << endl;

			}

			if (pos != string::npos) //Je¿eli jest to wierzcho³ek, to zazyna zczytywanie wierzcho³ka
			{

				string::size_type pos2 = linia.find("id");
				string::size_type pos3 = linia.find("/>");
				pos3 = pos3 - pos2 - 4;
				string id = linia.substr(pos2 + 3, pos3);
			}

			pos = linia.find("edge");
			if (pos != string::npos) // Je¿eli jest to krawêdŸ, to zazyna tworzenie zczytywanie po³¹czenia
			{
				//================== BUDOWANIE I WYCINANIE Z TEKSTU KRAWÊDZI
				string::size_type pos2 = linia.find("source=");
				string::size_type pos3 = linia.find("\" target=");
				pos3 = pos3 - pos2 - 9;
				string id1 = linia.substr(pos2 + 9, pos3);

				string::size_type pos4 = linia.find("target=");
				string::size_type pos5 = linia.find("\"/>");
				pos5 = pos5 - pos4 - 9;
				string id2 = linia.substr(pos4 + 9, pos5);

				uint32_t a = atoi(id1.c_str());
				uint32_t b = atoi(id2.c_str());
				this->matrix[a][b] = 1;
			}

		}

		// Close the file
		odczyt.close();

		
	}
	else
	{
		std::cout << "File missing" << std::endl;
	}
	file.close();
}

void Graph::Matrix::printGraphML()
{
	using namespace std;
	//Wypisywanie
	int N = this->nodes_amount;
	cout << endl;
	cout << "Macierz o wymiarze: " << N << "x" << N << endl;
	for (int i = 0; i < N; i++)
	{
		cout << "|";
		for (int j = 0; j < N; j++)
		{
			//cout << " " << macierz[i][j];
			cout << " " << this->matrix[i][j];

		}
		cout << " |" << endl;
	}
	cout << endl;

	// Pocz¹tek realnej pracy
	cout << "\nZaczynam tworzyc plik" << "\n---\n";
	ofstream plik("macierz_out.txt");
	const string quote = "\"";

	plik << "<?xml version=";
	plik << quote + "1.0" + quote;
	plik << " encoding=";
	plik << quote + "UTF-8" + quote;
	plik << "?>\n";

	//<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
	//    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	//    xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
	//     http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
	plik << "<graphml xmlns=";
	plik << quote + "http://graphml.graphdrawing.org/xmlns" + quote << "\n";
	plik << "    xmlns:xsi=" << quote + "http://www.w3.org/2001/XMLSchema-instance" + quote << "\n";
	plik << "    xsi:schemaLocation=" << '"' << "http://graphml.graphdrawing.org/xmlns" << "\n";
	plik << "     http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd" << '"' << ">\n";


	plik << " <graph id=";
	plik << quote + nazwa_grafu + quote;
	plik << " edgedefault=";
	plik << quote + typ_grafu + quote + ">";
	plik << "\n";



	for (int i = 0; i < N; i++) // Tworzy wiersze nodów <node id="n0"/>
	{
		plik << "  <node id=\"";
		plik << "n";
		plik << i;
		plik << '"';
		plik << "/>";
		plik << "\n";

	}

	//Tworzenie krawêdzi
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (this->matrix[i][j] ==1 )//(macierz[i][j] == 1)
			{
				plik << "  <edge source=\"";
				plik << "n";
				plik << i;
				plik << '"';
				plik << " target=";
				plik << '"';
				plik << "n";
				plik << j;
				plik << '"';
				plik << "/>";
				plik << "\n";

			}


		}

	}
	plik << " </graph>" << "\n";
	plik << "</graphml>" << "\n";
	plik.close();

	cout << "\n";
	cout << "Koniec pracy";
}

void Graph::Matrix::zad_3_c()
{
	using namespace std;
	cout << "Zaczynam prace" << endl; // prints !!!Hello World!!!
	cout << "Wczytuje parametry" << endl;


	int* tablica_stopni_v = new int[this->nodes_amount];		 //stopnie wierzcho³ków [zmienna sama siê liczy]
	int liczba_krawedzi_wykresu = 0; //krawêddzie			[zmienna sama siê liczy]
	int* tablica_potencjalu_wierzcholka = new int[this->nodes_amount];
	int potencjal_grafu = 0;

	/* KONCEPCJA
	 Indeks Ekstrady dla grafu to suma  x1 + x2 +.... xn,  gdzie   Xi =  pierwiastek[ ( d(u) + d(v) - 2 ) / d(u) * d(v)}
	 Xi - wyraz wartoœci x dla danego numery i
	 v - wierzcho³ek
	 u - wierzcho³ek drugi
	 d - stopieñ
	 ]

	Z tego co rozumiem, wartoœæ ABC jest sum¹ wartoœci X liczonych dla par wszystkich wierzho³ków
	gdzie ka¿dy wyraz X  okreœliæ mo¿na wzorem:
	Pierwiastek( (stopieñ(u) + stopieñ(v) - 2 )/ (stopieñ(u) * stopieñ(V) )).
	liczymi to dla ka¿dej pray wierzcho³ków,

	*/

	//============================== TU SIE OBLICZENIA ZACZYNAJA
	//Liczenie stopni wierzcho³ków macierzy
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		tablica_stopni_v[i] = 0;
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			if (this->matrix[i][j] == 1)
			{
				tablica_stopni_v[i]++;
				liczba_krawedzi_wykresu++;

				if (j == i) { tablica_stopni_v[i]++; }

			}
		}
	}
	//wypisanie stopni:
	cout << "Wypisuje stopnie wierzcholkow:" << endl;
	for (uint32_t i = 0; i < this->nodes_amount; i++) { cout << i << ": " << tablica_stopni_v[i] << endl; }


	float ABC = 0;

	//=============================== DRUGA CZESC OBLICZEN
	//Liczenie stopni macierzy
	cout << endl;
	cout << "Potencjaly wierzcholkow: " << endl;
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		int k = 0;
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			if (this->matrix[i][j] == 1) // ³apie s¹siada
			{
				
				int u = i;
				int v = j;
				int d_u = tablica_stopni_v[u];
				int d_v = tablica_stopni_v[v];
				float X = sqrt( static_cast<float>(d_u + d_v - 2)/(d_u * d_v) ); 
				ABC = ABC + X;
				
			}
		}
		//cout << "wierzcholek:" << i << " ma potencjal: " << k << endl;
		
	}
	cout << "Wartoœæ indeksu Estrady to: " << ABC << endl;
	delete[] tablica_stopni_v;
	delete[] tablica_potencjalu_wierzcholka;
	

}

void Graph::Matrix::add_edge(uint32_t source, uint32_t destination)
{
	if (source < this->nodes_amount && destination < this->nodes_amount)
	{
		this->matrix[source][destination]++;
		this->matrix[destination][source]++;
	}
}

void Graph::Matrix::add_node()
{
	uint32_t amount = this->nodes_amount + 1;
	uint32_t** m = new uint32_t * [amount];
	for (uint32_t i = 0; i < amount; i++)
	{
		m[i] = new uint32_t[amount];
		m[i][amount - 1] = 0;
		if (i == amount - 1)
		{
			for (uint32_t j = 0; j < amount - 1; j++)
			{
				m[i][j] = 0;
			}
		}
	}
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			m[i][j] = this->matrix[i][j];
		}
		delete[] this->matrix[i];
	}
	delete[] this->matrix;
	this->nodes_amount++;
	this->matrix = m;
}

void Graph::Matrix::remove_edge(uint32_t source, uint32_t destination)
{
	if (source < this->nodes_amount && destination < this->nodes_amount)
	{
		if (this->matrix[source][destination] > 0)
		{
			this->matrix[source][destination]--;
		}
		if (this->matrix[destination][source] > 0)
		{
			this->matrix[destination][source]--;
		}
	}
}

void Graph::Matrix::remove_node(uint32_t node_id)
{
	uint32_t amount = this->nodes_amount - 1;
	uint32_t** m = new uint32_t * [amount];
	for (uint32_t i = 0; i < amount; i++)
	{
		m[i] = new uint32_t[amount];
	}
	uint32_t row = 0;
	uint32_t col = 0;
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		if (i != node_id)
		{
			for (uint32_t j = 0; j < this->nodes_amount; j++)
			{
				if (j != node_id)
				{
					m[row][col] = this->matrix[i][j];
					col++;
				}
			}
			col = 0;
			row++;
		}
		delete[] this->matrix[i];
	}
	delete[] this->matrix;
	this->matrix = m;
	this->nodes_amount = amount;
}



void Graph::Matrix::zad_3_d() 
{
	using namespace std;
	cout << "Zaczynam prace" << endl;
	cout << "Wczytuje parametry" << endl;


	uint32_t *tablica_stopni_v = new uint32_t[this->nodes_amount];		 //stopnie wierzcho³ków [zmienna sama siê liczy]
	int liczba_krawedzi_wykresu = 0; //krawêddzie			[zmienna sama siê liczy]
	int *tablica_potencjalu_wierzcholka = new int[this->nodes_amount];
	uint32_t potencjal_grafu = 0;

	
	/* KONCEPCJA
	 potencja³ wierzcho³ka grafu, to liczba k, bêd¹ca liczb¹ s¹siadów danego 'u' wierzcho³ka,
	 których (ka¿dego z osobna) stopieñ jest wiêkszy/równy temu w³aœnie wartoœci k.
	 Potencja³ grafu, to najwiêkszy potencja³ wierzcho³ka który jest w nim zawarty.

	 Program najpierw liczy stopnie wierzcho³ków i liczbe krawêdzi.
	 Nastêpnie liczy dla ka¿dego wierzcho³ka z osobna wartoœæ jego potencja³u.
	 Finalnie okreœla potencja³ grafu i koñczy zadanie
	
	
	*/

	//============================== TU SIE OBLICZENIA ZACZYNAJA
	//Liczenie stopni wierzcho³ków macierzy
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		tablica_stopni_v[i] = 0;
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			if (this->matrix[i][j] == 1)
			{
				tablica_stopni_v[i]++;
				liczba_krawedzi_wykresu++;

				if (j == i) { tablica_stopni_v[i]++; }
			}
		}
	}
	//wypisanie stopni:
	cout << "Wypisuje stopnie wierzcholkow:" << endl;
	for (uint32_t i = 0; i < this->nodes_amount; i++) { cout << i << ": " << tablica_stopni_v[i] << endl; }


	//=============================== DRUGA CZESC OBLICZEN
	//Liczenie stopni macierzy
	cout << endl;
	cout << "Potencjaly wierzcholkow: " << endl;
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		uint32_t k = 0;
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			if (this->matrix[i][j] == 1) // ³apie s¹siada
			{
				//sprawdza mu potenccja³
				if (i != j && tablica_stopni_v[j] >= k) //sprawdza warunek czy s¹siad ma wiêkszy stopien niz k
				{
					//nalicza k i zapisuje do tabelki wierzcholka
					k++;
					tablica_potencjalu_wierzcholka[i] = k;
				}

			}
		}
		cout << "wierzcholek:" << i << " ma potencjal: " << k << endl;
		if (k >= potencjal_grafu) potencjal_grafu = k;
	}
	cout << endl;
	cout << "Potencjal grafu wynosi: " << potencjal_grafu << endl;
	delete[] tablica_stopni_v;
	delete[] tablica_potencjalu_wierzcholka;

}

Graph::Matrix::Matrix(List& list)
{
	this->nodes_amount = list.get_nodes_amount();
	this->matrix = new uint32_t* [this->nodes_amount];

	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		this->matrix[i] = new uint32_t[this->nodes_amount];
	}

	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			this->matrix[i][j] = 0;
		}
	}

	uint32_t neighbours;
	uint32_t neighbour_id;

	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		neighbours = list.get_node_degree(i);

		for (uint32_t j = 0; j < neighbours; j++)
		{
			neighbour_id = list.get_neighbour(i, j);
			this->matrix[i][neighbour_id-1]++;
		}
	}
}

void Graph::Matrix::print()
{
	std::cout << "Nodes amount = " << this->nodes_amount << std::endl;

	std::cout << "[" << std::endl;
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = 0; j < this->nodes_amount; j++)
		{
			std::cout << this->matrix[i][j] << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}

const uint32_t Graph::Matrix::get_nodes_amount()
{
	return this->nodes_amount;
}

const uint32_t Graph::Matrix::get_value(uint32_t x, uint32_t y)
{
	return this->matrix[x][y];
}



void Graph::Matrix::change_to_line_graph()
{
	std::vector<coord> edges;
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		for (uint32_t j = i; j < this->nodes_amount; j++)
		{
			if (this->matrix[i][j] == 1)
			{
				edges.push_back({ i, j });
			}
		}
	}
	uint32_t size = static_cast<uint32_t>(edges.size());
	uint32_t** m = new uint32_t * [size];
	for (uint32_t i = 0; i < size; i++)
	{
		m[i] = new uint32_t[size];
		for (uint32_t j = 0; j < size; j++)
		{
			m[i][j] = 0;
		}
	}
	coord coordinates;
	uint32_t index;
	for (uint32_t i = 0; i < size; i++)
	{
		coordinates = edges[i];
		for (uint32_t k = coordinates.x; k < this->nodes_amount; k++)
		{
			if (this->matrix[coordinates.x][k] == 1 && k != coordinates.y)
			{
				index = find_index(edges, coordinates.x, k);
				m[i][index] = 1;
				m[index][i] = 1;
			}
		}
		for (uint32_t k = coordinates.x; k < coordinates.y; k++)
		{
			if (this->matrix[k][coordinates.y] == 1 && k != coordinates.x)
			{
				index = find_index(edges, k, coordinates.y);
				m[i][index] = 1;
				m[index][i] = 1;
			}
		}
	}
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		delete[] this->matrix[i];
	}
	delete[] this->matrix;
	this->matrix = m;
	this->nodes_amount = size;
}

Graph::Matrix::~Matrix()
{
	for (uint32_t i = 0; i < this->nodes_amount; i++)
	{
		delete[] this->matrix[i];
	}
	delete[] this->matrix;
}

uint32_t Graph::Matrix::find_index(std::vector<coord>& edges, uint32_t x, uint32_t y)
{
	uint32_t size = static_cast<uint32_t>(edges.size());

	uint32_t ret = 0xFFFFFFFF;

	for (uint32_t i = 0; i < size; i++)
	{
		if (edges[i].x == x && edges[i].y == y)
		{
			ret = i;
		}
	}
	return ret;
}
