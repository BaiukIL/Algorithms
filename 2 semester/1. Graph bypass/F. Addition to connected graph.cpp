#include <iostream>
#include <unordered_set>
#include <stack>
#include <vector>

class SetGraph {
private:
    std::vector<std::unordered_set<int>> verticesCoherent;
public:
    explicit SetGraph(unsigned long size);
    ~SetGraph();

    void AddEdge(int from, int to);

    unsigned long VerticesCount() const;

    void GetNextVertices(int vertex, std::vector<int>& vertices) const;

    void GetPrevVertices(int vertex, std::vector<int>& vertices) const;

    template <class container>
    void DFS(const int &startVertex, container &leaveTimes, std::vector<bool> &visited) const;

    unsigned long AdditionToSCC();
};

SetGraph::SetGraph(unsigned long size) {
  verticesCoherent.resize(size);
}

SetGraph::~SetGraph() = default;

void SetGraph::AddEdge(const int from, const int to) {
  verticesCoherent[from].insert(to);
}

unsigned long SetGraph::VerticesCount() const {
  return static_cast<int>(verticesCoherent.size());
}

void SetGraph::GetNextVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for (int i : verticesCoherent[vertex]) {
    vertices.push_back(i);
  }
}

void SetGraph::GetPrevVertices(const int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for (int v = 0; v < verticesCoherent.size(); ++v) {
    if (verticesCoherent[v].count(vertex) != 0) {
      vertices.push_back(v);
    }
  }
}

inline void ContainerInsertion(std::stack<int> &stack, const int &value) {
  stack.push(value);
}

inline void ContainerInsertion(std::unordered_set<int> &unordered_set, const int &value) {
  unordered_set.insert(value);
}

//DFS can be used by std::stack or std::unordered_set containers
template <class container>
void SetGraph::DFS(const int &startVertex, container &leaveTimes, std::vector<bool> &visited) const {
  std::stack<int> dfsVertices;
  dfsVertices.push(startVertex);

  std::vector<int> coherentVertices;
  while (!dfsVertices.empty()) {
    int curVertex = dfsVertices.top();
//    mark vertex with grey
    visited[curVertex] = true;
    GetNextVertices(curVertex, coherentVertices);
//    check whether curVertex is leaf in dfs tree
    bool leaf = true;
//    if vertex has unchecked child then it is not leaf
    for (int i : coherentVertices) {
      if (!visited[i]) {
        dfsVertices.push(i);
        leaf = false;
      }
    }
    if (leaf) {
//      climb up the dfs tree while there are grey vertices on the way
      while (!dfsVertices.empty() && visited[dfsVertices.top()]) {
        curVertex = dfsVertices.top();
        dfsVertices.pop();
        ContainerInsertion(leaveTimes, curVertex);
      }
    }
  }
}

unsigned long SetGraph::AdditionToSCC() {
//  build travers graph
  SetGraph traversGraph(VerticesCount());
  for (int v = 0; v < VerticesCount(); ++v) {
    std::vector<int> nextVertices;
    GetNextVertices(v, nextVertices);
    for (int i : nextVertices) {
      traversGraph.AddEdge(i, v);
    }
  }

//  leaveTimes - stack of vertices' leaving times in travers graph DFS
  std::vector<bool> visited(traversGraph.VerticesCount(), false);
  std::stack<int> leaveTimes;
  for (int i = 0; i < traversGraph.VerticesCount(); ++i) {
    if (!visited[i]) {
      traversGraph.DFS(i, leaveTimes, visited);
    }
  }

  visited.assign(VerticesCount(), false);
//  create condensedVertices vector which shows what vertices in initial graph are strongly connected
  std::vector<std::unordered_set<int>> condensedVertices;
//    SCC = Strongly Connected Component
  std::vector<int> verticesOfSCC(VerticesCount());
  for (int numOfSCC = 0; !leaveTimes.empty(); ) {
    int startVertex = leaveTimes.top();
    leaveTimes.pop();

    if (!visited[startVertex]) {
      std::unordered_set<int> SCC;
      DFS(startVertex, SCC, visited);
//      number vertices according to their SCC assignment
//      example: if 3 and 5 are in 5th SCC then verticesOfSCC[3] = 5, verticesOfSCC[5] = 5,
      for (int v: SCC) {
        verticesOfSCC[v] = numOfSCC;
      }
      condensedVertices.push_back(SCC);
      ++numOfSCC;
    }
  }

//  build condensed graph
  SetGraph condensedGraph(condensedVertices.size());
  for (std::unordered_set<int> &SCC : condensedVertices) {
    for (int v : SCC) {
      std::vector<int> nextVertices;
      GetNextVertices(v, nextVertices);
      for (int i : nextVertices) {
//        if vertices are in different SCC and connected with edge then push edge between these SCC
        if (verticesOfSCC[v] != verticesOfSCC[i]) {
          condensedGraph.AddEdge(verticesOfSCC[v], verticesOfSCC[i]);
        }
      }
    }
  }

//  count the number of "output" vertices (i.e. those which with have no ancestors) in condensed graph
//  and "input" vertices (those which have no children)
  unsigned long result = 0;
//  if condensed graph have more than one SCC
  if (condensedGraph.VerticesCount() != 1) {
    std::vector<int> input;
    std::vector<int> output;
    std::vector<int> nextVertices;
    std::vector<int> prevVertices;
    for (int i = 0; i < condensedGraph.VerticesCount(); ++i) {
      condensedGraph.GetNextVertices(i, nextVertices);
      if (nextVertices.empty()) {
        input.push_back(i);
      }
      condensedGraph.GetPrevVertices(i, prevVertices);
      if (prevVertices.empty()) {
        output.push_back(i);
      }
    }
    result = input.size() > output.size() ? input.size() : output.size();
  }

  return result;
}

int main() {
//  V, E - numbers of vertices and edges respectively
  int V, E;
  std::cin >> V >> E;
  SetGraph graph(V);
//  beg, end - vertices of an edge
  int beg, end;
  for (int i = 0; i < E; ++i) {
    std::cin >> beg >> end;
    graph.AddEdge(beg - 1, end - 1);
  }

  std::cout << graph.AdditionToSCC();

  return 0;
}
