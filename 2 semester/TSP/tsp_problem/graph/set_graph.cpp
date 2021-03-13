#include "set_graph.h"

size_t Graph::VerticesCount() const {
  return coherent.size();
}

void Graph::AddEdge(vertex_t from, vertex_t to, weight_t weight) {
  coherent[from].emplace(to, weight);
  coherent[to].emplace(from, weight);
}

size_t Graph::CountEdge(vertex_t from, vertex_t to) const {
  return coherent[from].count(to);
}

std::vector<Edge> Graph::GetEdges() const {
  std::vector<Edge> edges;
  for (vertex_t from = 0; from < coherent.size(); ++from) {
    for (auto &pair : coherent[from]) {
      edges.emplace_back(Edge(from, pair.first, pair.second));
    }
  }
  return edges;
}

std::vector<vertex_t> Graph::GetNextVertices(vertex_t from) const {
  std::vector<vertex_t> next;
  for (auto& i: coherent[from]) {
    next.push_back(i.first);
  }
//  RVO optimization
  return next;
}

weight_t Graph::GetEdgeWeight(vertex_t from, vertex_t to) const {
  return (*coherent[from].find(to)).second;
}

Graph& Graph::operator+=(const Graph &graph) {
  if (VerticesCount() != graph.VerticesCount()) {
    throw std::runtime_error("Can't unite graphs with different vertices number");
  }
  for (auto &edge: graph.GetEdges()) {
//    Do not use AddEdge() to avoid adding redundant edges duplicates.
//    It happens because GetEdges contains asymmetric (e.g. (1, 5) and (5, 1)) edges
//    (because graph is undirected)
    coherent[edge.begin].emplace(edge.end, edge.weight);
  }
  return *this;
}
