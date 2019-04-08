#include <iostream>
#include <vector>

typedef unsigned int vertex_t;
typedef short weight_t;

class MatrixGraph {
 private:
  std::vector<std::vector<weight_t>> vertices;

  void FloydWarshall(MatrixGraph &graph) const;

 public:
  explicit MatrixGraph(size_t size);
  MatrixGraph(const MatrixGraph &otherGraph);

  ~MatrixGraph() = default;

  void SetEdgeWeight(vertex_t from, vertex_t to, weight_t weight);

  weight_t GetEdgeWeight(vertex_t from, vertex_t to) const;

  size_t VerticesCount() const;

  void GetNextVertices(vertex_t vertex, std::vector<vertex_t> &next) const;

  void GetPrevVertices(vertex_t vertex, std::vector<vertex_t> &vertices) const;

  void LeastPathsMatrix(MatrixGraph &graph) const;
};

MatrixGraph::MatrixGraph(size_t size) {
  vertices.resize(size);
  for (vertex_t v = 0; v < size; ++v) {
    vertices[v].resize(size);
  }
}
MatrixGraph::MatrixGraph(const MatrixGraph &otherGraph) : MatrixGraph(otherGraph.VerticesCount()) {
  for (vertex_t v = 0; v < otherGraph.VerticesCount(); ++v) {
    std::vector<vertex_t> connected;
    otherGraph.GetNextVertices(v, connected);
    for (vertex_t i : connected) {
      vertices[v][i] = otherGraph.GetEdgeWeight(v, i);
    }
  }
}
void MatrixGraph::SetEdgeWeight(vertex_t from, vertex_t to, weight_t weight) {
  vertices[from][to] = weight;
}
size_t MatrixGraph::VerticesCount() const {
  return vertices.size();
}
void MatrixGraph::GetNextVertices(vertex_t vertex, std::vector<vertex_t> &next) const {
  next.clear();
  for (vertex_t i = 0; i < vertices[vertex].size(); ++i) {
    if (vertices[vertex][i] != 0) {
      next.push_back(i);
    }
  }
}
void MatrixGraph::GetPrevVertices(vertex_t vertex, std::vector<vertex_t> &previous) const {
  previous.clear();
  for (vertex_t v = 0; v < vertices.size(); ++v) {
    if (vertices[v][vertex] != 0) {
      previous.push_back(v);
    }
  }
}
weight_t MatrixGraph::GetEdgeWeight(vertex_t from, vertex_t to) const {
  return vertices[from][to];
}
void MatrixGraph::FloydWarshall(MatrixGraph &graph) const {
  for (vertex_t k = 0; k < VerticesCount(); ++k) {
    for (vertex_t i = 0; i < VerticesCount(); ++i) {
      for (vertex_t j = 0; j < VerticesCount(); ++j) {
        graph.vertices[i][j] =
            graph.vertices[i][j] <= graph.vertices[i][k] + graph.vertices[k][j] ? graph.vertices[i][j] :
            graph.vertices[i][k] + graph.vertices[k][j];
      }
    }
  }
}

void MatrixGraph::LeastPathsMatrix(MatrixGraph &graph) const {
  FloydWarshall(graph);
}

int main() {
  size_t N;
  std::cin >> N;
  MatrixGraph graph(N);
  weight_t weight;
  for (vertex_t from = 0; from < N; ++from) {
    for (vertex_t to = 0; to < N; ++to) {
      std::cin >> weight;
      graph.SetEdgeWeight(from, to, weight);
    }
  }

  MatrixGraph weightGraph(graph);
  graph.LeastPathsMatrix(weightGraph);
  for (vertex_t from = 0; from < N; ++from) {
    for (vertex_t to = 0; to < N; ++to) {
      std::cout << weightGraph.GetEdgeWeight(from, to) << ' ';
    }
    std::cout << '\n';
  }

  return 0;
}
