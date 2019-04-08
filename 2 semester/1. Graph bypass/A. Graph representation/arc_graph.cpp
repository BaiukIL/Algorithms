#include "arc_graph.h"

ArcGraph::ArcGraph(int size) : verticesCount(size) {}

ArcGraph::ArcGraph(const IGraph *otherGraph) : ArcGraph(otherGraph->VerticesCount()) {
  verticesCount = static_cast<unsigned int>(otherGraph->VerticesCount());
  std::vector<int> nextVertices;
  for (int v = 0; v < verticesCount; ++v) {
    otherGraph->GetNextVertices(v, nextVertices);
    for (int nextVertex : nextVertices) {
      edges.emplace_back(std::pair<int, int>(v, nextVertex));
    }
  }
}

ArcGraph::~ArcGraph() = default;

void ArcGraph::AddEdge(const int from, const int to) {
  edges.emplace_back(std::pair<int, int>(from, to));
}

int ArcGraph::VerticesCount() const {
  return verticesCount;
}

void ArcGraph::GetNextVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for (auto &edge : edges) {
    if (edge.first == vertex) {
      vertices.push_back(edge.second);
    }
  }
}

void ArcGraph::GetPrevVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for (auto &edge : edges) {
    if (edge.second == vertex) {
      vertices.push_back(edge.first);
    }
  }
}
