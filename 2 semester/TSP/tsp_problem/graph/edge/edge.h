#ifndef TSP_STATISTIC_EDGE_H
#define TSP_STATISTIC_EDGE_H

typedef uint vertex_t;
typedef double weight_t;

struct Edge {
  vertex_t begin;
  vertex_t end;
  weight_t weight;
  explicit Edge(vertex_t begin, vertex_t end, weight_t weight): begin(begin), end(end), weight(weight) {}
  bool operator<(const Edge &edge) const {
    return weight < edge.weight;
  }
  bool operator==(const Edge &edge) const {
    return begin == edge.begin && end == edge.end && weight == edge.weight;
  }
//  Edges are necessary to store at std::unordered_* fro quick search,
//  so we need to overload hash function
  friend struct std::hash<Edge>;
};

namespace std {
template<> struct hash<Edge> {
  uint64_t operator()(const Edge& edge) const {
//    begin * end + weight seems to hash well enough
    return hash<uint64_t>()(edge.begin*edge.end + static_cast<uint64_t>(edge.weight));
  }
};
}

#endif //TSP_STATISTIC_EDGE_H
