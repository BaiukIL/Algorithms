#include <iostream>
#include <vector>
#include <stack>

class ListGraph {
private:
    std::vector<std::vector<int>> verticesCoherent;

    bool DFS(const int &startVertex, std::vector<char> &colours) const;

    int VerticesCount() const;

    void GetCoherentVertices(const int &vertex, std::vector<int> &vertices) const;
public:
    explicit ListGraph(int size);

    ~ListGraph();

    void AddEdge(const int &from, const int &to);

    bool IsPairGraph() const;
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

bool ListGraph::DFS(const int &startVertex, std::vector<char> &colours) const {
  std::stack<int> dfsVertices;
  bool label = true;

  dfsVertices.push(startVertex);
  colours[startVertex] = label;
  std::vector<int> coherentVertices;

  while (!dfsVertices.empty()) {
    int curVertex = dfsVertices.top();
    dfsVertices.pop();
    label = not colours[curVertex];

    GetCoherentVertices(curVertex, coherentVertices);
    for (int i : coherentVertices) {
      if (colours[i] == 'n') {
        colours[i] = label;
        dfsVertices.push(i);
      } else if (colours[i] != label) {
        return false;
      }
    }
  }
  return true;
}

bool ListGraph::IsPairGraph() const {
//  n means "none"
  std::vector<char> colours(VerticesCount(), 'n');
  for (int v = 0; v < VerticesCount(); ++v) {
    if (colours[v] == 'n') {
      if (!DFS(v, colours)) {
        return false;
      }
    }
  }
  return true;
}

int main() {
//  v, n - numbers of vertices and edges respectively
  int v, n;
  std::cin >> v >> n;
  ListGraph graph(v);
//  beg, end - vertices of an edge
  int beg, end;
  for (int i = 0; i < n; ++i) {
    std::cin >> beg >> end;
    graph.AddEdge(beg, end);
  }

  graph.IsPairGraph() ? std::cout <<  "YES" : std::cout <<  "NO";

  return 0;
}
