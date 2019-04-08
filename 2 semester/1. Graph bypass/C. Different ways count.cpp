#include <iostream>
#include <queue>
#include <vector>

class ListGraph {
private:
    std::vector<std::vector<int>> verticesCoherent;
public:
    explicit ListGraph(int size);
    ~ListGraph();

    void AddEdge(const int &from, const int &to);

    int VerticesCount() const;

    void GetCoherentVertices(const int &vertex, std::vector<int> &vertices) const;

    int LeastPathsCount(const int &u, const int &w) const;
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

int ListGraph::LeastPathsCount(const int &u, const int &w) const {
  std::vector<int> minPathCount(VerticesCount(), 0);
  std::vector<int> minPathLen(VerticesCount(), 0);
  minPathCount[u] = 1;
  minPathLen[u] = 0;

  std::queue<int> bfsVertices;
  bfsVertices.push(u);

  while (!bfsVertices.empty()) {
    int curVertex = bfsVertices.front();
    bfsVertices.pop();
    std::vector<int> coherentVertices;
    GetCoherentVertices(curVertex, coherentVertices);

    for (int i : coherentVertices) {
      if (minPathCount[i] == 0) {
        bfsVertices.push(i);
        minPathCount[i] += minPathCount[curVertex];
        minPathLen[i] = minPathLen[curVertex] + 1;
      } else if (minPathLen[curVertex] + 1 == minPathLen[i]) {
        minPathCount[i] += minPathCount[curVertex];
      }
    }
  }
  return minPathCount[w];
}

int main() {
  int v, n;
  std::cin >> v >> n;
  ListGraph graph(v);
  int beg, end;
  for (int i = 0; i < n; ++i) {
    std::cin >> beg >> end;
    graph.AddEdge(beg, end);
  }
  int u, w;
  std::cin >> u >> w;
  std::cout << graph.LeastPathsCount(u, w);

  return 0;
}
