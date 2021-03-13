/* weighted multigraph based on hash-table */

#ifndef MY_CPP_PROJECT_SET_GRAPH_WEIGHT_H
#define MY_CPP_PROJECT_SET_GRAPH_WEIGHT_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "edge/edge.h"

class Graph {
 public:
  explicit Graph(size_t size) : coherent(size) {}
//  trivial copy constructor which just copies `coherent` field
  Graph(const Graph &) = default;
  ~Graph() = default;

//  return number of vertices
  size_t VerticesCount() const;
  void AddEdge(vertex_t from, vertex_t to, weight_t weight);
//  return number of edges incident to `from` and `to` vertices
  size_t CountEdge(vertex_t from, vertex_t to) const;
//  return edge weight
  weight_t GetEdgeWeight(vertex_t from, vertex_t to) const;
//  return array of vertices incident to `from`
  std::vector<vertex_t> GetNextVertices(vertex_t from) const;
//  return array of all graph edges
  std::vector<Edge> GetEdges() const;
//  unite this graph with other one (`graph`)
  Graph& operator+=(const Graph &graph);

 private:
  std::vector<std::unordered_multimap<vertex_t, weight_t>> coherent;
};

#endif //MY_CPP_PROJECT_SET_GRAPH_WEIGHT_H
