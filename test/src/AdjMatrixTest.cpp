#include <Graphs/AdjMatrix.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace testing;

const std::string matFile = "../test/sample/adjMat.mat";
const std::string graphmlFile = "../test/sample/GraphML.GRAPHML";

namespace Graphs
{
TEST(AdjMatrixTest, createFromMatFile) {
    AdjMatrix adjMatrix(matFile);
    ASSERT_EQ(6, adjMatrix.nodesAmount());
    ASSERT_EQ(4, adjMatrix.nodeDegree(0));
    ASSERT_EQ(2, adjMatrix.nodeDegree(1));
    ASSERT_EQ(4, adjMatrix.nodeDegree(2));
    ASSERT_EQ(0, adjMatrix.nodeDegree(3));
    ASSERT_EQ(3, adjMatrix.nodeDegree(4));
    ASSERT_EQ(1, adjMatrix.nodeDegree(5));
}

TEST(AdjMatrixTest, createFromGraphMLFile) {
    AdjMatrix adjMatrix(graphmlFile);
    ASSERT_EQ(9, adjMatrix.nodesAmount());
    ASSERT_EQ(2, adjMatrix.nodeDegree(0));
    ASSERT_EQ(2, adjMatrix.nodeDegree(1));
    ASSERT_EQ(2, adjMatrix.nodeDegree(2));
    ASSERT_EQ(2, adjMatrix.nodeDegree(3));
    ASSERT_EQ(2, adjMatrix.nodeDegree(4));
    ASSERT_EQ(2, adjMatrix.nodeDegree(5));
    ASSERT_EQ(3, adjMatrix.nodeDegree(6));
    ASSERT_EQ(3, adjMatrix.nodeDegree(7));
    ASSERT_EQ(4, adjMatrix.nodeDegree(8));
}
} // namespace Graphs
