#include "list_graph.h"

ListGraph::ListGraph(int size) {
  verticesCoherent.resize(size);
}

ListGraph::ListGraph(const IGraph *otherGraph) : ListGraph(otherGraph->VerticesCount()) {
  for (int v = 0; v < otherGraph->VerticesCount(); ++v) {
    otherGraph->GetNextVertices(v, verticesCoherent[v]);
  }
}

ListGraph::~ListGraph() = default;

void ListGraph::AddEdge(const int from, const int to) {
  verticesCoherent[from].push_back(to);
}

int ListGraph::VerticesCount() const {
  return static_cast<int>(verticesCoherent.size());
}

void ListGraph::GetNextVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  vertices = verticesCoherent[vertex];
}

void ListGraph::GetPrevVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for (int v = 0; v < verticesCoherent.size(); ++v) {
    for (int i = 0; i < verticesCoherent[v].size(); ++i) {
      if (verticesCoherent[v][i] == vertex) {
        vertices.push_back(v);
      }
    }
  }
}

