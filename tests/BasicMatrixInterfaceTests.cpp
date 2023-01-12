#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <GraphMatrix.h>

#include <stdexcept>
#include <iostream>


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

TEST(MatrixInterfaceTests, ConstructMatrixFromDynamicMatrix)
{
    Graph::Matrix::DynamicMatrix dynamicMatrix = {{1, 0}, {0, 1}};
    EXPECT_NO_THROW({auto matrix = Graph::Matrix::constructFromDynamicMatrix(std::move(dynamicMatrix),
                                                            Graph::Type::undirected);   
    });
}

struct DynamicMatrixSet
{
    Graph::Matrix::DynamicMatrix matrix;
    Graph::Type type;
};

struct MatrixInterfaceTestsParametric : public testing::TestWithParam<DynamicMatrixSet>
{};

TEST_P(MatrixInterfaceTestsParametric, InvalidConstructionFromDynamicMatrix)
{
    auto dynamicMatrixSet = GetParam();

    EXPECT_THROW({auto matrix = Graph::Matrix::constructFromDynamicMatrix(std::move(dynamicMatrixSet.matrix),
                                                                          dynamicMatrixSet.type);
    }, std::invalid_argument);
}

INSTANTIATE_TEST_SUITE_P(
    ParametricInstantiation,
    MatrixInterfaceTestsParametric,
    ::testing::Values(DynamicMatrixSet{Graph::Matrix::DynamicMatrix{{1, 0, 1}, {}, {0, 0, 1}}, // partially empty 
                                       Graph::Type::undirected},
                      DynamicMatrixSet{Graph::Matrix::DynamicMatrix{{}}, // empty
                                       Graph::Type::undirected},
                      DynamicMatrixSet{Graph::Matrix::DynamicMatrix{{1, 0, 1}, {0, 1}}, // not square 
                                       Graph::Type::undirected}));


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