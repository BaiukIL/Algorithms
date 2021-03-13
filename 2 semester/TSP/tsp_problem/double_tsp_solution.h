/* Find TSP solution which is at most 1.5 times the optimal in O(E*logV) */

#ifndef TSP_TSP_SOLUTION_H
#define TSP_TSP_SOLUTION_H

#include "graph/set_graph.h"
#include "graph/graph_funcs.h"
#include <stack>

weight_t DoubleTravelPathWeight(const Graph &map);

#endif //TSP_TSP_SOLUTION_H
