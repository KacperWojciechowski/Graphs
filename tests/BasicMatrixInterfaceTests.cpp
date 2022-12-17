#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <GraphMatrix.h>

#include <stdexcept>
#include <iostream>

TEST(MatrixInterfaceTests, ConstructingMatrixFromCorrectFile)
{
    EXPECT_NO_THROW({
        auto matrix = Graph::Matrix::constructFromFile("../samples/matrixDirected.mat", 
                                                       Graph::Type::directed);
    });
}

TEST(MatrixInterfaceTests, ConstructingMatrixFromFileWithIncorrectExtension)
{
    EXPECT_THROW({
        auto matrix = Graph::Matrix::constructFromFile("../samples/listDirected.lst",
                                                       Graph::Type::directed);
        }, std::invalid_argument
    );
}

TEST(MatrixInterfaceTests, ConstructingMatrixFromMissingFile)
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

std::string getExpectedPrintOutput(const Graph::Matrix& m)
{
    return m.getType() == Graph::Type::directed
            ? "[\nType = directed\n  1,   1 {in_deg = 2, out_deg = 2}\n  1,   0 {in_deg = 1, out_deg = 1}\n]\n"
            : "[\nType = undirected\n  1,   0 {deg = 2}\n  0,   1 {deg = 2}\n]\n";
}

bool isExpectedPrintOutput(const Graph::Matrix& m)
{
    testing::internal::CaptureStdout();
    std::cout << m;
    std::string receivedOutput = testing::internal::GetCapturedStdout();
    std::string expectedOutput = getExpectedPrintOutput(m);

    return (receivedOutput == expectedOutput);
}

TEST(MatrixInterfaceTests, PrintDirectedMatrixTest)
{
    Graph::Matrix::DynamicMatrix dynamicMatrix = {{1, 1}, {1, 0}};
    auto matrix = Graph::Matrix::constructFromDynamicMatrix(std::move(dynamicMatrix),
                                                            Graph::Type::directed);
    
    EXPECT_TRUE(isExpectedPrintOutput(matrix));
    EXPECT_TRUE(&(std::cout << matrix) == &std::cout);
}

TEST(MatrixInterfaceTests, PrintUndirectedMatrixTest)
{
    Graph::Matrix::DynamicMatrix dynamicMatrix = {{1, 0}, {0, 1}};
    auto matrix = Graph::Matrix::constructFromDynamicMatrix(std::move(dynamicMatrix),
                                                            Graph::Type::undirected);
    EXPECT_TRUE(isExpectedPrintOutput(matrix));
    EXPECT_TRUE(&(std::cout << matrix) == &std::cout);
}

// Add test for constructing from DynamicMatrix
// Add test for constructing from not square DynamicMatrix
// Add test for constructing from empty DynamicMatrix