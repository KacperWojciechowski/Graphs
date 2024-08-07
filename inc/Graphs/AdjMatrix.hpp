#pragma once

#include <Graphs/Graph.hpp>
#include <string>
#include <vector>

namespace Data
{
void generate_weighted_graph(std::string file_path, unsigned int nodes_amount);
void generate_throughtput(std::string input_file_path, std::string output_file_path, unsigned int nodes_amount);
} // namespace Data

namespace Graphs
{
class AdjList;

class AdjMatrix : public Graph
{
    public:
    /* Constructors */
    AdjMatrix(std::string file_path, std::string throughtput_file_path, std::string name, std::string type);
    AdjMatrix(const Graph& list);

    AdjMatrix(AdjMatrix& m) = delete;
    AdjMatrix(AdjMatrix&& m) = delete;

    /* Interface */
    void print();
    void print_throughtput();
    void saveGraphML(std::string file_path);

    float estrada_index();

    uint32_t nodesAmount() const override;
    uint32_t nodeDegree(NodeId) const override;
    EdgeInfo findEdge(const EdgeInfo&) const override;

    void setEdge(const EdgeInfo&) override;
    void addNode() override;
    void removeNode(NodeId) override;
    void removeEdge(const EdgeInfo&) override;
    std::vector<NodeId> getNodeIds() const override;
    std::vector<NodeId> getNeighborsOf(NodeId) const override;

    void change_to_line_graph();

    int32_t belman_ford(unsigned int vertex, bool log);
    int32_t throughtput_belman_ford(unsigned int searched_throughtput, unsigned int vertex, bool log);

    /* Destructor */
    virtual ~AdjMatrix();

    private:
    std::string show() const override;

    /* Custom data types */
    struct coord
    {
        unsigned int x;
        unsigned int y;
    };

    /* Private functions */
    unsigned int find_index(std::vector<coord>& edges, unsigned int x, unsigned int y);

    /* Private variables */
    unsigned int nodes_amount;
    unsigned int** matrix;
    std::string graph_name;
    std::string graph_type;

    unsigned int* distance;
    std::vector<unsigned int>* prev_node;
    unsigned int** throughtput;
};
} // namespace Graphs
