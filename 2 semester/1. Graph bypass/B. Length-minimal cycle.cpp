#include <iostream>
#include <queue>
#include <vector>

struct VertexParam {
  bool visited = false;
//  there is no "-1" vertex in any graph
  int ancestor = -1;
  int depth = 0;
};

class ListGraph {
private:
    std::vector<std::vector<int>> verticesCoherent;

    int bfsCycles(const int &rootVertex) const;

public:
    explicit ListGraph(int size);
    ~ListGraph();

    void AddEdge(const int &from, const int &to);

    int VerticesCount() const;

    void GetCoherentVertices(const int &vertex, std::vector<int> &vertices) const;

    int bfsLeastCycle() const;
};

ListGraph::ListGraph(int size) {
  verticesCoherent.resize(size);
}

ListGraph::~ListGraph() = default;

void ListGraph::AddEdge(const int &from, const int &to) {
  verticesCoherent[from].push_back(to);
  verticesCoherent[to].push_back(from);
}

int ListGraph::VerticesCount() const {
  return static_cast<int>(verticesCoherent.size());
}

void ListGraph::GetCoherentVertices(const int &vertex, std::vector<int> &vertices) const {
  vertices.clear();
  vertices = verticesCoherent[vertex];
}

int ListGraph::bfsCycles(const int &rootVertex) const {

  std::vector<VertexParam> vertices(VerticesCount());
  std::queue<int> bfsVertices;

  bfsVertices.push(rootVertex);
  vertices[rootVertex].visited = true;

  while(!bfsVertices.empty()) {
    int currentVertex = bfsVertices.front();
    bfsVertices.pop();

    std::vector<int> coherentVertices;
    GetCoherentVertices(currentVertex, coherentVertices);
    for (int i : coherentVertices) {
      if (!vertices[i].visited) {
        bfsVertices.push(i);
        vertices[i].visited = true;
        vertices[i].depth = vertices[currentVertex].depth + 1;
        vertices[i].ancestor = currentVertex;
      } else if (i != vertices[currentVertex].ancestor) {
//        the first met cycle is the shortest
        return vertices[currentVertex].depth + vertices[i].depth + 1;
      }
    }
  }
//  if there are no cycles return -1
  return -1;
}

int ListGraph::bfsLeastCycle() const {
  int minCycle = -1;
  bool found = true;
  for (int v = 0; v < VerticesCount(); ++v) {
    int currentCycle = bfsCycles(v);
    if (currentCycle > 0 && found) {
      minCycle = currentCycle;
      found = false;
    }
    if (currentCycle > 0 && currentCycle < minCycle) {
      minCycle = currentCycle;
    }
  }
  return minCycle;
}

int main() {
  int N, M;
  std::cin >> N >> M;
  ListGraph graph(N);
  int beg, end;
  for (int i = 0; i < M; ++i) {
    std::cin >> beg >> end;
    graph.AddEdge(beg, end);
  }

  std::cout << graph.bfsLeastCycle();

  return 0;
}
