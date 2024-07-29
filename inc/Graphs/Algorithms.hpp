#pragma once

#include <Graphs/Graph.hpp>

namespace Algorithm {
class AlgorithmFunctor {
public:
  virtual void operator()() = 0;
  virtual ~AlgorithmFunctor() = default;
};

template <bool isVerbose>
using isVerbose_t = typename std::enable_if_t<isVerbose>;

template <bool isVerbose> class GreedyColoring : public AlgorithmFunctor {
public:
  template <isVerbose_t>
  GreedyColoring(const Graphs::Graph &graph, std::ostream &out)
      : graph(graph), outStream{std::make_unique<std::ostream>(out)} {}

  template <not isVerbose_t>
  GreedyColoring(const Graphs::Graph &graph) : graph(graph) {}
  void operator()() override;

private:
  const Graphs::Graph &graph;
  std::unique_ptr<std::ostream> outStream{};
};
} // namespace Algorithm