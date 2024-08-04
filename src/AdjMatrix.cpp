// this
#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>

// libraries
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

namespace Graphs
{
AdjMatrix::AdjMatrix(std::string file_path, std::string throughtput_file_path, std::string name, std::string type)
    : matrix(nullptr)
    , nodes_amount(0)
    , graph_name(name)
    , graph_type(type)
    , distance(nullptr)
    , prev_node(nullptr)
    , throughtput(nullptr) {
    std::ifstream file(file_path);

    if (file.good())
    {
        // if opened file extension is .mat
        if (file_path.find(".mat", 0) != std::string::npos)
        {
            std::string line;
            uint32_t vertexes = 0;

            // read a line from file
            std::getline(file, line);

            // reset offset and current position
            std::size_t offset = 0;
            std::size_t pos = 0;

            // count the amount of vertices
            while (true)
            {
                // search for next space character from the offset position
                pos = line.find(' ', offset);

                // if such character was found, then it means the line contains next
                // vertex
                if (pos != std::string::npos)
                {
                    vertexes++;
                    offset = pos + 1;
                }
                // if space was not found, this means there is only one more vertex left
                else
                {
                    vertexes++;
                    break;
                }
            }

            // save the vertices amount and create adjacency matrix
            this->nodes_amount = vertexes;
            this->matrix = new uint32_t*[this->nodes_amount];
            for (uint32_t i = 0; i < this->nodes_amount; i++)
            {
                this->matrix[i] = new uint32_t[this->nodes_amount];
            }

            // create arrays for the SSP functions
            this->distance = new uint32_t[this->nodes_amount];
            this->prev_node = new std::vector<uint32_t>[this->nodes_amount];

            // move to the beginnig of the file for matrix load
            file.seekg(std::ios_base::beg);

            // read the matrix
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
        // file is in GraphML format
        else if (file_path.find(".GRAPHML") != std::string::npos)
        {
            std::string line;

            // search for graph ID
            std::getline(file, line);
            std::size_t pos = line.find("graph id=");

            // skip any unneeded header lines
            while (pos == std::string::npos)
            {
                std::getline(file, line);
                pos = line.find("graph id=");
            }

            // search for edge type
            std::size_t pos2 = line.find("edgedefault=");

            // save type and ID data
            this->graph_name = line.substr(pos + 10, pos2 - 2 - (pos + 10));
            pos = line.find("\">");
            this->graph_type = line.substr(pos2 + 13, pos - (pos2 + 13));

            // search for node marker
            std::getline(file, line);
            pos = line.find("node");

            // count the amount of nodes in the file
            while (pos != std::string::npos)
            {
                this->nodes_amount++;
                std::getline(file, line);
                pos = line.find("node");
            }

            // create adjacency matrix and fill it with zeros
            this->matrix = new uint32_t*[this->nodes_amount];
            for (uint32_t i = 0; i < this->nodes_amount; i++)
            {
                this->matrix[i] = new uint32_t[this->nodes_amount];
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    this->matrix[i][j] = 0;
                }
            }

            // search for edge marker
            pos = line.find("edge");

            std::string id1;
            std::string id2;

            // extract edge info
            while (pos != std::string::npos)
            {
                // find and extract the source and target nodes IDs
                pos = line.find("source=");
                pos2 = line.find("\" target=");

                id1 = line.substr(pos + 9, pos2 - (pos + 9));
                pos = line.find("\"/>");
                id2 = line.substr(pos2 + 11, pos - (pos2 + 11));

                // input the connection into matrix
                this->matrix[atoi(id1.c_str())][atoi(id2.c_str())] = 1;

                // search for next edge marker
                std::getline(file, line);
                pos = line.find("edge");
            }
        }
        // if given file extension is unsupported
        else
        {
            std::cout << "Unsupported file format" << std::endl;
        }

        // open and read the throughtput file format
        std::ifstream thr_file(throughtput_file_path);
        uint32_t value;

        // open the throughtput matrix file
        if (thr_file.good())
        {
            // create sufficiently big matrix
            this->throughtput = new uint32_t*[this->nodes_amount];
            for (uint32_t i = 0; i < this->nodes_amount; i++)
            {
                this->throughtput[i] = new uint32_t[this->nodes_amount];

                // load the row
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    thr_file >> value;
                    this->throughtput[i][j] = value;
                }
            }
        }
        thr_file.close();
    }
    // if file is missing
    else
    {
        std::cout << "File missing" << std::endl;
    }
    file.close();
}

/*
        Create a matrix object based on Graph::List object.

        Params:
        list - reference to the List object from which to create the
                   matrix

        Return:
        None
*/
AdjMatrix::AdjMatrix(const Graph& graph)
    : throughtput(nullptr) {
    nodes_amount = graph.nodesAmount();
    matrix = new uint32_t*[nodes_amount];
    distance = new uint32_t[nodes_amount];
    prev_node = new std::vector<uint32_t>[nodes_amount];

    for (uint32_t i = 0; i < nodes_amount; i++)
    {
        matrix[i] = new uint32_t[nodes_amount];
        for (uint32_t j = 0; j < nodes_amount; j++)
        {
            matrix[i][j] = graph.findEdge({i, j}).weight.value_or(0);
        }
    }
}

/*
        Function that exports current graph into .GRAPHML file.

        Params:
        file_path - destination file path

        Return:
        None
*/
void AdjMatrix::saveGraphML(std::string file_path) {
    // opening output file
    std::ofstream file(file_path);

    // header
    file << "<?xml version=\"1.0\"";
    file << " encoding=\"UTF-8\"?>\n";

    // xml schema
    file << "<graphml xmlns=";
    file << "\"http://graphml.graphdrawing.org/xmlns\"\n";
    file << "	xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
    file << "	xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n";
    file << "	http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";

    // graph type data
    file << " <graph id=";
    file << "\"" + graph_name + "\"";
    file << " edgedefault=";
    file << "\"" + graph_type + "\">\n";

    // nodes data
    for (uint32_t i = 0; i < this->nodes_amount; i++) // Tworzy wiersze nod�w <node id="n0"/>
    {
        file << "	<node id=\"n" << i;
        file << "\"/>\n";
    }

    // edge data
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            if (this->matrix[i][j] == 1)
            {
                file << "	<edge source=\"n" << i;
                file << "\" target=\"n" << j;
                file << "\"/>\n";
            }
        }
    }
    // close the markers and the file
    file << " </graph>\n";
    file << "</graphml>";
    file.close();
}

/*	Function calculates the Estrada index for the graph
        given the equation:

        index = sum(sqrt(deg(u) + deg(v) - 2)/(deg(v)^2)

        Params:
        None

        Return:
        value of Estrada index
*/
float AdjMatrix::estrada_index() {
    uint32_t* degrees = new uint32_t[this->nodes_amount];

    // calculating degrees
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        degrees[i] = 0;
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            if (this->matrix[i][j] == 1)
            {
                degrees[i]++;
            }
        }
    }

    float index = 0;

    // calculating the index
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            if (this->matrix[i][j] == 1) // �apie s�siada
            {
                float root = std::sqrt(static_cast<float>(degrees[i] + degrees[j] - 2) / (pow(degrees[i], 2)));
                index += root;
            }
        }
    }

    // free allocated memory
    delete[] degrees;

    // return Estrada index value
    return index;
}

/*
        Add an edge between two given vertices. Function validates if
        given vertices IDs are in range.

        Params:
        source		- source vertice
        destination - destination vertice

        Return:
        None
*/
void AdjMatrix::setEdge(const EdgeInfo& edge) {
    if (edge.source < this->nodes_amount && edge.destination < this->nodes_amount)
    {
        this->matrix[edge.source][edge.destination] = 1;
        if (this->graph_type == "undirected")
        {
            this->matrix[edge.destination][edge.source] = 1;
        }
    }
}

/*
        Add a vertex to the graph. Due to creating the matrix to the specific
        size, adding a vertex forces reallocation of the structure. Added vertex
        is initially isolated.

        Params:
        None

        Return:
        None
*/
void AdjMatrix::addNode() {
    // increase the amount of vertices in the temporary variable
    uint32_t amount = this->nodes_amount + 1;

    // create sufficiently big matrix
    uint32_t** m = new uint32_t*[amount];

    for (uint32_t i = 0; i < amount; i++)
    {
        m[i] = new uint32_t[amount];
    }

    // move and fit the current matrix into the new one
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            std::memcpy(m[i], this->matrix[i], this->nodes_amount * sizeof(uint32_t));
        }
        // free memory allocated by the previous matrix row
        delete[] this->matrix[i];
    }

    // fill last column and row with zeros
    for (uint32_t i = 0; i < amount; i++)
    {
        m[i][amount - 1] = 0;
        if (i == amount - 1)
        {
            for (uint32_t j = 0; j < amount - 1; j++)
            {
                m[i][j] = 0;
            }
        }
    }

    // free the remains of unused allocated memory
    delete[] this->matrix;

    // increase the nodes amount and overwrite the address of previous matrix
    this->nodes_amount++;
    this->matrix = m;
}

/*
        Removing the edge between two nodes. Function verifies is the vertices
        indexes are within range.

        Params:
        source		- source vertex of the edge
        destination	- destination vertex of the edge

*/
void AdjMatrix::removeEdge(const EdgeInfo& edge) {
    if (edge.source < this->nodes_amount && edge.destination < this->nodes_amount)
    {
        this->matrix[edge.source][edge.destination] = 0;
        if (this->graph_type == "undirected")
        {
            this->matrix[edge.destination][edge.source] = 0;
        }
    }
}

/*
        Removing the node along with all its edges. The function
        validates if the vertex id is within range. Removal of a vertex
        causes renumeration of subsequent verticies and reallocating
        the matrix.

        Params:
        node_id - id of the node to remove

        Return:
        None
*/
void AdjMatrix::removeNode(NodeId node) {
    if (node < this->nodes_amount)
    {
        // save reduced nodes amount in temporary variable
        uint32_t amount = this->nodes_amount - 1;

        // prepare shrunken matrix
        uint32_t** m = new uint32_t*[amount];
        for (uint32_t i = 0; i < amount; i++)
        {
            m[i] = new uint32_t[amount];
        }

        uint32_t row = 0;
        uint32_t col = 0;

        // rewrite the values from old matrix to the new one
        // while skipping the column and row of deleted vertex
        for (uint32_t i = 0; i < this->nodes_amount; i++)
        {
            if (i != node)
            {
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    if (j != node)
                    {
                        m[row][col] = this->matrix[i][j];
                        col++;
                    }
                }
                col = 0;
                row++;
            }
            // free already moved row
            delete[] this->matrix[i];
        }
        // free the rest of the matrix
        delete[] this->matrix;

        // overwrite the pointer with the addres of new matrix
        // and save new vertices count
        this->matrix = m;
        this->nodes_amount = amount;
    }
}

std::string AdjMatrix::show() const {
    std::stringstream out;

    out << "Name = " << this->graph_name;
    out << "\nType = " << this->graph_type;
    out << "\nNodes amount = " << this->nodes_amount << "\n";

    out << "[\n";
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            out << this->matrix[i][j] << ", ";
        }
        out << "\n";
    }
    out << "]\n";
    return out.str();
}

void AdjMatrix::print_throughtput() {
    std::cout << "[" << std::endl;
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            std::cout << this->throughtput[i][j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;
}

uint32_t AdjMatrix::nodesAmount() const {
    return this->nodes_amount;
}

EdgeInfo AdjMatrix::findEdge(const EdgeInfo& edge) const {
    if (edge.source > this->nodes_amount or edge.destination > this->nodes_amount)
    {
        return {edge.source, edge.destination, std::nullopt};
    }
    if (matrix[edge.source - 1][edge.destination - 1] == 0)
    {
        return {edge.source, edge.destination, std::nullopt};
    }
    else
    {
        return {edge.source, edge.destination, matrix[edge.source - 1][edge.destination - 1]};
    }
}

std::vector<NodeId> AdjMatrix::getNodeIds() const {
    std::vector<NodeId> ids(this->nodes_amount);
    std::iota(ids.begin(), ids.end(), 1);
    return ids;
}

std::vector<NodeId> AdjMatrix::getNeighborsOf(NodeId node) const {
    std::vector<NodeId> neighbors;
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        if (this->matrix[node - 1][i] != 0)
        {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

void AdjMatrix::change_to_line_graph() {
    // gather all the edges from the adjacency matrix of initial graph
    std::vector<coord> edges;
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = i; j < this->nodes_amount; j++)
        {
            if (this->matrix[i][j] == 1)
            {
                edges.push_back({i, j});
            }
        }
    }

    // create sufficient matrix for the line graph and fill it with zeros
    uint32_t size = static_cast<uint32_t>(edges.size());
    uint32_t** m = new uint32_t*[size];
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

    // fill the line graph
    for (uint32_t i = 0; i < size; i++)
    {
        // load coordinates of the edge
        coordinates = edges[i];

        // search though the matrix for common vertices between edges
        // in the same row of the initial adjacency matrix
        for (uint32_t k = coordinates.x; k < this->nodes_amount; k++)
        {
            // if common vertex was found, calculate the line graph vertex index
            // and mark the connection
            if (this->matrix[coordinates.x][k] == 1 && k != coordinates.y)
            {
                index = find_index(edges, coordinates.x, k);
                m[i][index] = 1;
                m[index][i] = 1;
            }
        }
        // search through the matrix for common vertices between edges
        // in the same column of the initial adjacency matrix
        for (uint32_t k = coordinates.x; k < coordinates.y; k++)
        {
            // if common vertex was found, calculate the line graph vertex index
            // and mark the connection
            if (this->matrix[k][coordinates.y] == 1 && k != coordinates.x)
            {
                index = find_index(edges, k, coordinates.y);
                m[i][index] = 1;
                m[index][i] = 1;
            }
        }
    }

    // free previous matrix memory allocation
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        delete[] this->matrix[i];
    }
    delete[] this->matrix;

    // overwrite the matrix with the pointer to line graph's adjacency matrix
    // and save new size of the graph
    this->matrix = m;
    this->nodes_amount = size;
}

int32_t AdjMatrix::belman_ford(uint32_t vertex, bool log) {
    // set default values for the distances and previous vertices
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        this->distance[i] = 0x7FFFFFFF;
        this->prev_node[i].clear();
    }

    // set zero as a distance for the starting vertex
    this->distance[vertex] = 0;

    bool change_found;

    auto start = std::chrono::system_clock::now();

    // search through the graph multiple times to find shortest paths
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        // assume change in distance was not found
        change_found = false;
        for (uint32_t x = 0; x < this->nodes_amount; x++)
        {
            for (uint32_t j = 0; j < this->nodes_amount; j++)
            {
                // if change in distance between connected vertices was found, save said
                // information
                if (this->distance[j] > this->distance[x] + this->matrix[x][j] && this->matrix[x][j] != 0)
                {
                    // remember that change was found, to continue search
                    change_found = true;
                    this->distance[j] = this->distance[x] + this->matrix[x][j];
                    this->prev_node[j].clear();
                    this->prev_node[j].push_back(x);
                }
            }
            // if log flag is set, print informations about distance and previous
            // nodes
            if (log)
            {
                // distance information
                std::cout << "Distances: " << std::endl;
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    std::cout << this->distance[j] << ", ";
                }
                // previous vertices information
                std::cout << std::endl << "Previous: " << std::endl;
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    for (auto itr = this->prev_node[j].begin(); itr != this->prev_node[j].end(); itr++)
                    {
                        std::cout << *itr << ", ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
        // if change was not found, optimize the algorithm by stopping the search
        if (!change_found)
        {
            break;
        }
    }

    // calculate how long the algorithm was running
    auto end = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // if log flag is set, print the duration
    if (log)
    {
        std::cout << "Duration: " << diff.count() << " us" << std::endl;
    }
    return diff.count();
}

int32_t AdjMatrix::throughtput_belman_ford(uint32_t searched_throughtput, uint32_t vertex, bool log) {
    // set default values for the distances and previous vertices
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        this->distance[i] = 0x7FFFFFFF;
        this->prev_node[i].clear();
    }

    // set zero as a distance for the starting vertex
    this->distance[vertex] = 0;

    bool change_found;

    auto start = std::chrono::system_clock::now();
    // search through the graph multiple times to find shortest paths
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        // assume change in distance was not found
        change_found = false;
        for (uint32_t x = 0; x < this->nodes_amount; x++)
        {
            for (uint32_t j = 0; j < this->nodes_amount; j++)
            {
                // if change in distance between connected vertices was found while
                // fulfilling the throughtput requirement, save said information
                if (this->distance[j] > this->distance[x] + this->matrix[x][j]
                    && this->throughtput[x][j] >= searched_throughtput)
                {
                    // remember the change was found to continue search
                    change_found = true;
                    this->distance[j] = this->distance[x] + this->matrix[x][j];
                    this->prev_node[j].clear();
                    this->prev_node[j].push_back(x);
                }
                // if a path of the same distance was found, fulfilling the throughtput
                // requirement, and is not already saved, save it in prev_node vector.
                else if (this->distance[j] == this->distance[x] + this->matrix[x][j]
                         && this->throughtput[x][j] >= searched_throughtput
                         && std::find(this->prev_node[j].begin(), this->prev_node[j].end(), x)
                                == this->prev_node[j].end())
                {
                    // remember the change was found to continue search
                    change_found = true;
                    this->prev_node[j].push_back(x);
                }
            }
            // if log flag is set, print informations about distance and previous
            // nodes
            if (log)
            {
                // distance information
                std::cout << "Distances: " << std::endl;
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    std::cout << this->distance[j] << ", ";
                }
                // previous vertices information
                std::cout << std::endl << "Previous: " << std::endl;
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    for (auto itr = this->prev_node[j].begin(); itr != this->prev_node[j].end(); itr++)
                    {
                        std::cout << *itr << ", ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
        // if change was not found, optimize the algorithm by stopping the search
        if (!change_found)
        {
            break;
        }
    }

    // calculate how long the algorithm was running
    auto end = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // if log flag is set, print the duration
    if (log)
    {
        std::cout << "Duration: " << diff.count() << " us" << std::endl;
    }

    return diff.count();
}

/*
        Destructor freeing the memory allocation
*/
AdjMatrix::~AdjMatrix() {
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        delete[] this->matrix[i];
    }
    delete[] this->matrix;
}

uint32_t AdjMatrix::find_index(std::vector<coord>& edges, uint32_t x, uint32_t y) {
    uint32_t size = static_cast<uint32_t>(edges.size());

    uint32_t ret = 0xFFFFFFFF;

    // if coordinates match the source and destination of given edge,
    // it means that this edge is the line graph's vertex that we're looking for
    for (uint32_t i = 0; i < size; i++)
    {
        if (edges[i].x == x && edges[i].y == y)
        {
            ret = i;
        }
    }
    return ret;
}
} // namespace Graphs

void Data::generate_weighted_graph(std::string file_path, uint32_t nodes_amount) {
    std::ofstream file(file_path);

    srand(time(NULL));

    if (file.good())
    {
        // generate a graph of given size with random weights in the
        // range of <0;n>. Zero means no connection between vertices.
        for (uint32_t i = 0; i < nodes_amount; i++)
        {
            for (uint32_t j = 0; j < nodes_amount; j++)
            {
                if (i != j)
                {
                    file << (rand() % nodes_amount) * (rand() % 6 <= 3 ? 1 : 0);
                }
                else
                {
                    file << 0;
                }
                if (j < nodes_amount - 1)
                {
                    file << " ";
                }
            }
            if (i < nodes_amount - 1)
            {
                file << std::endl;
            }
        }
    }
    file.close();
}

void Data::generate_throughtput(std::string input_file_path, std::string output_file_path, uint32_t nodes_amount) {
    std::ofstream o_file(output_file_path);
    std::ifstream i_file(input_file_path);

    srand(time(NULL));

    uint32_t value;

    if (i_file.good())
    {
        // generate a matrix of throughtput for given graph file
        for (uint32_t i = 0; i < nodes_amount; i++)
        {
            for (uint32_t j = 0; j < nodes_amount; j++)
            {
                i_file >> value;
                if (value != 0)
                {
                    o_file << rand() % 9 + 1;
                }
                else
                {
                    o_file << 0;
                }
                if (j < nodes_amount - 1)
                {
                    o_file << " ";
                }
            }
            if (i < nodes_amount - 1)
            {
                o_file << std::endl;
            }
        }
    }
    o_file.close();
    i_file.close();
}
