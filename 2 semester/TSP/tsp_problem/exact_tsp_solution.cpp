#include "exact_tsp_solution.h"

weight_t ExactTravelPathWeight(const Graph &map) {
  std::vector<vertex_t> path(map.VerticesCount());

  int i = 0;
//  initialize path: 0, 1, 2, 3, ..., VerticesCount() - 1
  std::for_each(path.begin(), path.end(), [&i](vertex_t &vertex) { vertex = i++; });

  weight_t minWeight = INT32_MAX;

  while(std::next_permutation(path.begin(), path.end())) {
//    edge which closes circuit: (first = 0, last = *(--path.end()))
    vertex_t previous = *(--path.end());
    weight_t currentPathWeight = 0;
//    count circuit weight
    std::for_each(path.begin(), path.end(), [&currentPathWeight, &map, previous](vertex_t current) mutable {
      currentPathWeight += map.GetEdgeWeight(previous, current);
      previous = current;
    });
    minWeight = std::min(minWeight, currentPathWeight);
  }

  return minWeight;
}
