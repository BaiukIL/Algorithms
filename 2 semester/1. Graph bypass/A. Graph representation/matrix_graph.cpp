#include "matrix_graph.h"

MatrixGraph::MatrixGraph(int size) {
  verticesCoherent.resize(size);
  for (int v = 0; v < size; ++v) {
    verticesCoherent[v].resize(size);
  }
}

MatrixGraph::MatrixGraph(const IGraph *otherGraph) : MatrixGraph(otherGraph->VerticesCount()) {
  for (int v = 0; v < otherGraph->VerticesCount(); ++v) {
    std::vector<int> vertices;
    otherGraph->GetNextVertices(v, vertices);
    for (int i : vertices) {
      verticesCoherent[v][i] = 1;
    }
  }
}

MatrixGraph::~MatrixGraph() = default;

void MatrixGraph::AddEdge(const int from, const int to) {
  verticesCoherent[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
  return static_cast<int>(verticesCoherent.size());
}

void MatrixGraph::GetNextVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for (int i = 0; i < verticesCoherent[vertex].size(); ++i) {
    if (verticesCoherent[vertex][i] == 1) {
      vertices.push_back(i);
    }
  }
}

void MatrixGraph::GetPrevVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for (int v = 0; v < verticesCoherent.size(); ++v) {
    if (verticesCoherent[v][vertex] == 1) {
      vertices.push_back(v);
    }
  }
}
