#include <gmock/gmock.h>
#include <Graphs/AdjMatrix.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace testing;

namespace
{
constexpr uint32_t oneNode = 1;
constexpr uint32_t threeNodes = 3;
constexpr uint32_t fourNodes = 4;

constexpr Graphs::NodeId firstNodeId = 0;
constexpr Graphs::NodeId secondNodeId = 1;
constexpr Graphs::NodeId thirdNodeId = 2;
} // namespace

namespace Graphs
{
TEST(AdjMatrixTest, constructorCreatesAnEmptyGraph)
{
    AdjMatrix graph;
    EXPECT_EQ(graph.nodesAmount(), 0);
}

TEST(AdjMatrixTest, addingNodesIncreasesNodesAmount)
{
    AdjMatrix graph;
    graph.addNodes(oneNode);
    EXPECT_EQ(graph.nodesAmount(), 1);
    graph.addNodes(threeNodes);
    EXPECT_EQ(graph.nodesAmount(), 4);
}

TEST(AdjMatrixTest, removingNodesDecreasesNodesAmount)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.removeNode(thirdNodeId);
    EXPECT_EQ(graph.nodesAmount(), 3);
    graph.removeNode(firstNodeId);
    EXPECT_EQ(graph.nodesAmount(), 2);
}

TEST(AdjMatrixText, removingNodeWhichHasBeenAlreadyRemovedDoesNotThrowAndDoesNotRemoveAnyNodes)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.removeNode(thirdNodeId);
    EXPECT_EQ(graph.nodesAmount(), 3);
    EXPECT_NO_THROW(graph.removeNode(thirdNodeId));
    EXPECT_EQ(graph.nodesAmount(), 3);
}

TEST(AdjMatrixTest, removingNodeRemovesAllEdgesConnectedToIt)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdge({firstNodeId, thirdNodeId});
    graph.setEdge({thirdNodeId, firstNodeId});
    graph.removeNode(thirdNodeId);
    EXPECT_EQ(graph.nodeDegree(firstNodeId), 0);
}

TEST(AdjMatrixTest, addingEdgeIncreasesNodeDegree)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    EXPECT_EQ(graph.nodeDegree(firstNodeId), 0);
    graph.setEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(graph.nodeDegree(firstNodeId), 1);
}

TEST(AdjMatrixTest, removingEdgeDecreasesNodeDegree)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdge({firstNodeId, thirdNodeId});
    graph.setEdge({firstNodeId, secondNodeId});
    EXPECT_EQ(graph.nodeDegree(firstNodeId), 2);
    graph.removeEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(graph.nodeDegree(firstNodeId), 1);
}

TEST(AdjMatrixTest, removingEdgeWhichHasBeenAlreadyRemovedDoesNotThrowAndDoesNotRemoveAnyEdges)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdge({firstNodeId, thirdNodeId});
    graph.setEdge({firstNodeId, secondNodeId});
    EXPECT_EQ(graph.nodeDegree(firstNodeId), 2);
    graph.removeEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(graph.nodeDegree(firstNodeId), 1);
    EXPECT_NO_THROW(graph.removeEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(graph.nodeDegree(firstNodeId), 1);
}

TEST(AdjMatrixTest, getNeighborsOfReturnsCorrectNeighbors)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdge({firstNodeId, thirdNodeId});
    graph.setEdge({firstNodeId, secondNodeId});
    EXPECT_THAT(graph.getNeighborsOf(firstNodeId), ElementsAre(secondNodeId, thirdNodeId));
}

TEST(AdjMatrixTest, findEdgeReturnsCorrectEdgeInfo)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdge({firstNodeId, thirdNodeId, 5});
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).source, firstNodeId);
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).destination, thirdNodeId);
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).weight, 5);
}

TEST(AdjMatrixTest, findEdgeReturnsEmptyWeightIfEdgeDoesNotExist)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdge({firstNodeId, thirdNodeId, 5});
    EXPECT_EQ(graph.findEdge({firstNodeId, secondNodeId}).weight, std::nullopt);
}

TEST(AdjMatrixTest, setEdgeAppliesDefaultWeightWhenNoWeightIsGiven)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).weight, 1);
}

TEST(AdjMatrixTest, setEdgeUpdatesWeightWhenEdgeAlreadyExists)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdge({firstNodeId, thirdNodeId, 5});
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).weight, 5);
    graph.setEdge({firstNodeId, thirdNodeId, 10});
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).weight, 10);
}

TEST(AdjMatrixTest, canSetMultipleEdgesAtOnce)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdges({
        {firstNodeId,  thirdNodeId,  5},
        {firstNodeId, secondNodeId, 10}
    });
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).weight, 5);
    EXPECT_EQ(graph.findEdge({firstNodeId, secondNodeId}).weight, 10);
}

TEST(AdjMatrixTest, canSetMultipleEdgesAtOnceWithDefaultWeight)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdges({
        {firstNodeId,  thirdNodeId},
        {firstNodeId, secondNodeId}
    });
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).weight, 1);
    EXPECT_EQ(graph.findEdge({firstNodeId, secondNodeId}).weight, 1);
}

TEST(AdjMatrixTest, canUpdateMultipleEdges)
{
    AdjMatrix graph;
    graph.addNodes(fourNodes);
    graph.setEdges({
        {firstNodeId,  thirdNodeId,  5},
        {firstNodeId, secondNodeId, 10}
    });
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).weight, 5);
    EXPECT_EQ(graph.findEdge({firstNodeId, secondNodeId}).weight, 10);
    graph.setEdges({
        {firstNodeId,  thirdNodeId, 10},
        {firstNodeId, secondNodeId,  5}
    });
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).weight, 10);
    EXPECT_EQ(graph.findEdge({firstNodeId, secondNodeId}).weight, 5);
}

TEST(AdjMatrixTest, settingEdgeBetweenNonexistantNodesDoesNotDoAnything)
{
    AdjMatrix graph;
    graph.addNodes(oneNode);
    EXPECT_NO_THROW(graph.setEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(graph.nodeDegree(firstNodeId), 0);
}

TEST(AdjMatrixTest, fetchingInformationForNonexistantEdgeDoesNotThrowAndReturnsEmptyWeight)
{
    AdjMatrix graph;
    graph.addNodes(oneNode);
    EXPECT_NO_THROW(graph.findEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).weight, std::nullopt);
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).source, firstNodeId);
    EXPECT_EQ(graph.findEdge({firstNodeId, thirdNodeId}).destination, thirdNodeId);
}

} // namespace Graphs
