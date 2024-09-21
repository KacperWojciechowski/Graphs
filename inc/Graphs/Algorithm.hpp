#pragma once

#include <Graphs/Graph.hpp>

namespace Graphs::Algorithm
{
using Permutation = std::vector<NodeId>;
class AlgorithmFunctor
{
public:
    virtual void operator()(const Graphs::Graph&) = 0;
    virtual ~AlgorithmFunctor() = default;
};

template <bool isVerbose, class T>
using Verbose = typename std::enable_if_t<isVerbose, T>*;

template <bool isVerbose, class T>
using NotVerbose = Verbose<not isVerbose, T>;

constexpr bool verbose = true;
constexpr bool notVerbose = false;
} // namespace Graphs::Algorithm
