#include <Factory.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <GraphMatrix.h>
#include <DesignatedFactory.h>

TEST(GraphFactoryTests, CreateMatrixFromFile)
{
    Graph::Factory<Graph::Matrix>&& gf = Graph::DesignatedFactory<Graph::Matrix>{};
    EXPECT_NO_THROW({auto matrix = gf.createFromFile("../samples/matrixDirected.mat",
                                                    Graph::Type::directed);
    });
}

// test creating matrix
// test creating list
// test creating matrix from list and vice versa