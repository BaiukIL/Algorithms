#include "christofides_tsp_solution.h"

weight_t FormHamiltonianCircuit(const std::vector<vertex_t> &circuit, const Graph &map) {
  weight_t result = 0;

  std::vector<char> visited(circuit.size(), false);
  vertex_t previous = *circuit.begin();

  for (vertex_t current : circuit) {
    if (!visited[current] && previous != current) {
      result += map.GetEdgeWeight(previous, current);
      previous = current;
    }
  }

  return result;
}

weight_t ChristofidesTravelPathWeight(const Graph &map) {
//  Build min spanning tree (MST)
  Graph MST = FormMST(map);
//  Vector of odd vertices with odd degree in MST
  std::vector<vertex_t> oddVertices = FormOddVertices(MST);
//  Induced subgraph of map given by the vertices from MST
  Graph subgraph = FormSubgraph(map, oddVertices);
//  Find a minimum-weight perfect matching in the induced subgraph
  Graph perfectMatching = FormMinPerfectMatching(subgraph);
//  Combine the edges of perfectMatching and MST to form a connected Eulerian multigraph
  Graph eulerianGraph = MST + perfectMatching;
//  Form an Eulerian circuit in Eulerian graph
  std::vector<vertex_t> circuit = FormEulerianCircuit(eulerianGraph);
//  Make the circuit found in previous step into a Hamiltonian circuit by skipping repeated vertices
  weight_t result = FormHamiltonianCircuit(circuit, map);

  return result;
}
