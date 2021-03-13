#include "double_tsp_solution.h"

// if I manage to deal with templates in CMake, it will be written in DFSIterator
weight_t dfsBypath(const Graph &MST, const Graph &map) {
  weight_t result = 0;

  std::vector<char> visited(MST.VerticesCount(), false);
  std::stack<vertex_t> dfs;

  vertex_t previous = 0;
  vertex_t start = 0;
  dfs.push(start);

  for (vertex_t current; !dfs.empty(); previous = current) {
    current = dfs.top(); dfs.pop();
    visited[current] = true;
    if (current != start) {
      result += map.GetEdgeWeight(previous, current);
    }
    for (vertex_t next: MST.GetNextVertices(current)) {
      if (!visited[next]) {
        dfs.push(next);
      }
    }
  }
//  add edge from last to first to close the path
  result += map.GetEdgeWeight(previous, start);

  return result;
}

weight_t DoubleTravelPathWeight(const Graph &map) {
  Graph MST = FormMST(map);
  weight_t result = dfsBypath(MST, map);
  return result;
}
