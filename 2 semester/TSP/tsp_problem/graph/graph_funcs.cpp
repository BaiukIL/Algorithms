#include "graph_funcs.h"

Graph operator+(const Graph &first, const Graph &second) {
  Graph unitedGraph(first);
  unitedGraph += second;
  return unitedGraph;
}

Graph FormMST(const Graph &graph) {
  Graph minSpanningTree(graph.VerticesCount());

  std::vector<Edge> edges = graph.GetEdges();
  std::sort(edges.begin(), edges.end());

  DisjointSet sets(graph.VerticesCount());
  for (vertex_t vertex = 0; vertex < graph.VerticesCount(); ++vertex) {
    sets.MakeSet(vertex);
  }

  for (auto& edge : edges) {
    if (sets.Find(edge.begin) != sets.Find(edge.end)) {
      sets.Union(edge.begin, edge.end);
      minSpanningTree.AddEdge(edge.begin, edge.end, edge.weight);
    }
  }

  return minSpanningTree;
}

Graph FormSubgraph(const Graph &graph, const std::vector<vertex_t> &vertices) {
  Graph subgraph(graph.VerticesCount());
  for (auto from = vertices.cbegin(); from < vertices.cend(); ++from) {
    for (auto to = from + 1; to < vertices.cend(); ++to) {
      subgraph.AddEdge(*from, *to, graph.GetEdgeWeight(*from, *to));
    }
  }
  return subgraph;
}

Graph FormMinPerfectMatching(const Graph &graph) {
//  find non-singular vertices (those which are incident to at least one edge)
  std::vector<vertex_t> vertices;
  for (vertex_t vertex = 0; vertex < graph.VerticesCount(); ++vertex) {
    if (!graph.GetNextVertices(vertex).empty()) {
      vertices.push_back(vertex);
    }
  }

  std::sort(vertices.begin(), vertices.end());

  weight_t minMatchingWeight = INT32_MAX;
  std::vector<vertex_t> perfectMatching;

  while(std::next_permutation(vertices.begin(), vertices.end())) {
    weight_t matchingWeight = 0;
    for (auto iter = vertices.begin(); iter != vertices.end(); ++(++iter)) {
      matchingWeight += graph.GetEdgeWeight(*iter, *(iter + 1));
    }
    if (matchingWeight < minMatchingWeight) {
      minMatchingWeight = matchingWeight;
      perfectMatching = vertices;
    }
  }

  Graph perfectMatchingGraph(graph.VerticesCount());
  for (auto iter = perfectMatching.begin(); iter != perfectMatching.end(); ++(++iter)) {
    perfectMatchingGraph.AddEdge(*iter, *(iter + 1), graph.GetEdgeWeight(*iter, *(iter + 1)));
  }

  return perfectMatchingGraph;
}

std::vector<vertex_t> FormOddVertices(const Graph &tree) {
  std::vector<vertex_t> odd;
  for (vertex_t vertex = 0; vertex < tree.VerticesCount(); ++vertex) {
    if (tree.GetNextVertices(vertex).size() % 2 != 0) {
      odd.push_back(vertex);
    }
  }
  return odd;
}

std::vector<vertex_t> FormEulerianCircuit(const Graph &eulerianGraph) {
  std::vector<vertex_t> eulerianCircuit;
//  map from edge to the number of times it was visited
  std::unordered_map<Edge, uint> visited;

//  initialize visited edges with its occurrences numbers
  for (auto &edge: eulerianGraph.GetEdges()) {
    if (visited.count(edge) == 0) {
      visited.emplace(edge, eulerianGraph.CountEdge(edge.begin, edge.end));
    }
  }

  std::stack<vertex_t> vertices;
  vertex_t start = 0;
  vertices.push(start);

  for (vertex_t current; !vertices.empty(); ) {
    current = vertices.top();
//    remain true if all incident edges are visited
    bool stuck = true;
    for (vertex_t next: eulerianGraph.GetNextVertices(current)) {
      Edge edge = Edge(current, next, eulerianGraph.GetEdgeWeight(current, next));
      Edge backEdge = Edge(next, current, eulerianGraph.GetEdgeWeight(next, current));
//      if edge is not visited (or if all multiedges are not visited)
      if (visited[edge] != 0) {
        vertices.push(next);
        --visited[edge];
        --visited[backEdge];
        stuck = false;
        break;
      }
    }
//    if all incident to current vertex edges are visited
    if (stuck) {
      eulerianCircuit.push_back(current);
      vertices.pop();
    }
  }
  return eulerianCircuit;
}
