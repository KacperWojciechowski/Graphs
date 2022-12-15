#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <GraphMatrix.h>
#include <stdexcept>

TEST(MatrixInterfaceTests, CreatingMatrixFromCorrectFile)
{
    EXPECT_NO_THROW({
        auto matrix = Graph::Matrix::constructFromFile("../samples/matrixDirected.mat", 
                                                       Graph::Type::directed);
    });
}

TEST(MatrixInterfaceTests, CreatingMatrixFromFileWithIncorrectExtension)
{
    EXPECT_THROW({
        auto matrix = Graph::Matrix::constructFromFile("../samples/listDirected.lst",
                                                       Graph::Type::directed);
        }, std::invalid_argument
    );
}

TEST(MatrixInterfaceTests, CreatingMatrixFromMissingFile)
{
    EXPECT_THROW(
        {auto matrix = Graph::Matrix::constructFromFile("missing.mat", Graph::Type::directed);
        }, std::runtime_error
    );
}

TEST(MatrixInterfaceTests, CompareTheSameMatrices)
{
    auto matrix1 = Graph::Matrix::constructFromFile("../samples/matrixDirected.mat",
                                                   Graph::Type::directed);
    auto matrix2 = matrix1;

    EXPECT_TRUE(matrix1 == matrix2);
}

TEST(MatrixInterfaceTests, CompareDifferentMatrices)
{
    auto matrix1 = Graph::Matrix::constructFromFile("../samples/matrixDirected.mat",
                                                    Graph::Type::directed);
    auto matrix2 = Graph::Matrix::constructFromFile("../samples/matrixUndirected.mat",
                                                    Graph::Type::undirected);
    EXPECT_FALSE(matrix1 == matrix2);
}