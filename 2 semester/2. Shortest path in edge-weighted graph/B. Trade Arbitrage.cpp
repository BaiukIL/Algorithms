#include <iostream>
#include <vector>
#include <tuple>

class ArcGraphWeight {
private:
    std::vector<std::tuple<size_t, size_t, float>> edges;
    size_t verticesCount;

    size_t VerticesCount() const noexcept;

    void GetPrevVertices(size_t vertex, std::vector<std::pair<size_t, float>> &ancestors) const noexcept;

    bool BellmanFord() const noexcept;

    void Relax(std::vector<float>& distances, size_t begin, size_t end, float weight) const noexcept;
public:
    explicit ArcGraphWeight(size_t size) noexcept;

    ~ArcGraphWeight() = default;

    void AddEdge(size_t from, size_t to, float weight) noexcept;

    bool HasArbitrage() const noexcept;
};

ArcGraphWeight::ArcGraphWeight(size_t size) noexcept : verticesCount(size) {}

void ArcGraphWeight::AddEdge(size_t from, size_t to, float weight) noexcept {
  edges.emplace_back(std::make_tuple(from, to, weight));
}

size_t ArcGraphWeight::VerticesCount() const noexcept {
  return verticesCount;
}

void ArcGraphWeight::GetPrevVertices(size_t vertex, std::vector<std::pair<size_t, float>> &ancestors) const noexcept {
  ancestors.clear();
  for (auto &edge : edges) {
    if (std::get<1>(edge) == vertex) {
      ancestors.emplace_back(std::make_pair(std::get<0>(edge), std::get<2>(edge)));
    }
  }
}

bool ArcGraphWeight::HasArbitrage() const noexcept {
  return BellmanFord();
}

void ArcGraphWeight::Relax(std::vector<float> &distances, size_t begin, size_t end, float weight) const noexcept {
  if (distances[begin] * weight > distances[end]) {
    distances[end] = distances[begin] * weight;
  }
}

bool ArcGraphWeight::BellmanFord() const noexcept {
  std::vector<float> distances(VerticesCount(), 1);
  for (size_t i = 0; i < VerticesCount(); ++i) {
    for (auto &edge : edges) {
      Relax(distances, std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
    }
  }
  for (size_t i = 0; i < VerticesCount(); ++i) {
    std::vector<std::pair<size_t, float>> ancestors;
    GetPrevVertices(i, ancestors);
    for (auto &ancestor : ancestors) {
      if (distances[ancestor.first] * ancestor.second > distances[i]) {
        return false;
      }
    }
  }
  return true;
}

int main() {
  size_t currencyNumber;
  std::cin >> currencyNumber;
  ArcGraphWeight graph(currencyNumber);
//  begin, end - currencies
  for (size_t i = 0; i < currencyNumber; ++i) {
    for (size_t j = 0; j < currencyNumber; ++j) {
      if (i != j) {
        float coefficient;
        std::cin >> coefficient;
        if (coefficient != -1) {
          graph.AddEdge(i, j, coefficient);
        }
      }
    }
  }

  std::cout << (graph.HasArbitrage() ? "NO" : "YES");

  return 0;
}
