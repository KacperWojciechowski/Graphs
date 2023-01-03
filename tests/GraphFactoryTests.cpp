#include <Factory.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <GraphMatrix.h>

TEST(GraphFactoryTests, CreateMatrixFromFile)
{
    EXPECT_NO_THROW({auto matrix = Graph::constructFromFile<Graph::Matrix>("../samples/matrixDirected.mat",
                                                                           Graph::Type::directed);
    });
}

// test creating matrix
// test creating list
// test creating matrix from list and vice versa