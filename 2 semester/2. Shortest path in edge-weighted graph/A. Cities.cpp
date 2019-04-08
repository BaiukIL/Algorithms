#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

struct Less {
  bool operator()(const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b) const {
    return (a.second > b.second);
  }
};

class SetGraphWeight {
protected:
    std::vector<std::unordered_map<size_t, size_t>> coherent;
    size_t VerticesCount() const;
    void GetCoherentVertices(size_t current, std::vector<size_t> &vertices) const;
    size_t Weight(size_t from, size_t to) const;
    void AddEdge(size_t from, size_t to, size_t weight);
    void DeleteEdge(size_t from, size_t to);
public:

    explicit SetGraphWeight(size_t size) : coherent(size) {}

    ~SetGraphWeight() = default;

    void AddMinEdge(size_t from, size_t to, size_t weight);

    size_t LeastPath(size_t from, size_t to) const;
};

void SetGraphWeight::AddMinEdge(size_t from, size_t to, size_t weight) {
  if (coherent[from].find(to) != coherent[from].end()) {
    if (weight < (*coherent[from].find(to)).second) {
      DeleteEdge(from, to);
      AddEdge(from, to, weight);
    }
  } else {
    AddEdge(from, to, weight);
  }
}

size_t SetGraphWeight::VerticesCount() const {
  return coherent.size();
}

void SetGraphWeight::GetCoherentVertices(size_t current, std::vector<size_t> &vertices) const {
  vertices.clear();
  for (auto &i : coherent[current]) {
    vertices.push_back(i.first);
  }
}

size_t SetGraphWeight::LeastPath(size_t from, size_t to) const {
  std::vector<size_t> distances(VerticesCount(), 0);
  std::vector<bool> visited(VerticesCount(), false);
  std::priority_queue<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>, Less> dijkstra;

  dijkstra.push(std::make_pair(from, 0));
  while (!dijkstra.empty()) {
    size_t current = dijkstra.top().first;
    size_t currentDistance = dijkstra.top().second;
    dijkstra.pop();

    if (visited[current]) {
      continue;
    }

    std::vector<size_t> next;
    GetCoherentVertices(current, next);
    for (size_t child : next) {
      if (!visited[child]) {
        dijkstra.push(std::make_pair(child, currentDistance + Weight(current, child)));
      }
    }
    distances[current] = currentDistance;
    visited[current] = true;
  }
  return distances[to];
}

size_t SetGraphWeight::Weight(size_t from, size_t to) const {
  return (*coherent[from].find(to)).second;
}

void SetGraphWeight::AddEdge(size_t from, size_t to, size_t weight) {
  coherent[from].insert(std::make_pair(to, weight));
  coherent[to].insert(std::make_pair(from, weight));
}

void SetGraphWeight::DeleteEdge(size_t from, size_t to) {
  coherent[from].erase(to);
  coherent[to].erase(from);
}

int main() {
  size_t citiesNumber, roadsNumber;
  std::cin >> citiesNumber >> roadsNumber;
  SetGraphWeight graph(citiesNumber);
//  begin, end - cities of a road
  size_t begin, end, time;
  for (int i = 0; i < roadsNumber; ++i) {
    std::cin >> begin >> end >> time;
    graph.AddMinEdge(begin, end, time);
  }
  size_t start, finish;
  std::cin >> start >> finish;
  std::cout << graph.LeastPath(start, finish);

  return 0;
}
