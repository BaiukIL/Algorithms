/* functions which return structures start from `Form`
 * but not from `Get` as methods do
 * in order to distinguish functions and class methods */

#ifndef TSP_STATISTIC_GRAPH_FUNCS_H
#define TSP_STATISTIC_GRAPH_FUNCS_H

#include "set_graph.h"
#include "disjoint_set/disjoint_set.h"
#include <stack>

// unite two graphs and return new one
Graph operator+(const Graph &first, const Graph &second);

//Kruskal algorithm - return MST graph
Graph FormMST(const Graph &graph);

//return induced subgraph given by vertices from `vertices`
Graph FormSubgraph(const Graph &graph, const std::vector<vertex_t> &vertices);

//return minimum-weight perfect matching graph
//work in O(N!) (based on permutations)
//however, there is Blossom algorithm with minimum-weight optimization
//which searches MinPerfectMatching much faster.
//see realization at: http://pub.ist.ac.at/~vnk/papers/BLOSSOM5.html
Graph FormMinPerfectMatching(const Graph &graph);

//return array of vertices with odd degree in tree
std::vector<vertex_t> FormOddVertices(const Graph &tree);

//find any Eulerian circuit in Eulerian graph
std::vector<vertex_t> FormEulerianCircuit(const Graph &eulerianGraph);

#endif //TSP_STATISTIC_GRAPH_FUNCS_H
