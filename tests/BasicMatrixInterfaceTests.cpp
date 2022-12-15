#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <GraphMatrix.h>

TEST(MatrixInterfaceTest, CreatingMatrixFromCorrectFile)
{
    EXPECT_NO_THROW({
        auto matrix = Graph::Matrix::constructFromFile("../samples/matrixDirected.mat", 
                                                       Graph::Type::directed);
    });
}