#include "set_graph.h"

SetGraph::SetGraph(int size) {
  verticesCoherent.resize(size);
}

SetGraph::SetGraph(const IGraph *otherGraph) : SetGraph(otherGraph->VerticesCount()) {
  std::vector<int> nextVertices;
  for (int v = 0; v < otherGraph->VerticesCount(); ++v) {
    otherGraph->GetNextVertices(v, nextVertices);
    for (int i : nextVertices) {
      verticesCoherent[v].insert(i);
    }
  }
}

SetGraph::~SetGraph() = default;

void SetGraph::AddEdge(const int from, const int to) {
  verticesCoherent[from].insert(to);
}

int SetGraph::VerticesCount() const {
  return static_cast<int>(verticesCoherent.size());
}

void SetGraph::GetNextVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for (int i : verticesCoherent[vertex]) {
    vertices.push_back(i);
  }
}

void SetGraph::GetPrevVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for (int v = 0; v < verticesCoherent.size(); ++v) {
    if (verticesCoherent[v].count(vertex) != 0) {
      vertices.push_back(v);
    }
  }
}

