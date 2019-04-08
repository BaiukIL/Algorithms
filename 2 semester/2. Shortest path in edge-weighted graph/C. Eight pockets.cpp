#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <set>
#include <string>

size_t CountInversions(std::vector<uint64_t> path) {
  size_t result = 0;
  for (int i = 0; i < path.size(); ++i) {
    for (int j = 0; j < path.size() - (i + 1); ++j) {
      if (path[j] > path[j + 1]) {
        std::swap(path[j], path[j + 1]);
        if (path[j] != 0 && path[j+1] != 0) {
          ++result;
        }
      }
    }
  }
  return result;
}

class Position {
 private:

//  size of field's side
  uint16_t size;

//  field in binary representation
  uint64_t data;

//  place of blank block
  uint16_t nullPlace;

  uint16_t heuristic;

  //  return "value" of the block in "place" position
  uint16_t GetAt(uint16_t place) const;

//  set "value" block in "place" position of field
  void SetAt(uint16_t place, uint64_t value);

  uint16_t CountManhattanHeuristic() const;

 public:

  explicit Position(const std::vector<uint16_t>& table);
  Position(const Position &other, uint16_t nullPlace);
  Position(const Position &other) = default;
  ~Position() = default;

  bool operator < (const Position& other) const {
    return this->data < other.data;
  }
  bool operator > (const Position& other) const {
    return this->data > other.data;
  }
  bool operator == (const Position& other) const {
    return this->data == other.data;
  }
  bool operator != (const Position& other) const {
    return this->data != other.data;
  }

//  check whether block can be moved in appropriate direction
  bool CanMoveDown() const;
  bool CanMoveLeft() const;
  bool CanMoveRight() const;
  bool CanMoveUp() const;

  std::vector<uint64_t> StraightPath() const;
  std::vector<uint64_t> SnakePath() const;

  //  move empty block in appropriate direction
  Position MoveUp() const;
  Position MoveDown() const;
  Position MoveLeft() const;
  Position MoveRight() const;

  uint16_t GetHeuristic() const {
    return heuristic;
  }

  bool CanAttain(const Position &otherPos) const;
  int FindShortestPathTo(const Position &finalPos, std::string &shortestPath) const;

  friend struct std::hash<Position>;
};

struct Node {
  Position pos;
  uint32_t distance;
  Node(const Position &pos, uint32_t distance): pos(pos), distance(distance) {}
  bool operator < (const Node &other) const {
    if (this->pos.GetHeuristic() + distance == other.pos.GetHeuristic() + other.distance) {
      return pos > other.pos;
    } else {
      return this->pos.GetHeuristic() + distance < other.pos.GetHeuristic() + other.distance;
    }
  }
};

namespace std {
  template<> struct hash<Position> {
    uint64_t operator()(const Position& pos) const { return hash<uint64_t>()(pos.data); }
  };
}

Position::Position(const std::vector<uint16_t>& table): data(0), size(static_cast<uint16_t>(std::sqrt(table.size()))) {
  for (uint16_t place = 0; place < size*size; ++place) {
    if (table[place] == 0) {
      nullPlace = place;
    }
    SetAt(place, table[place]);
  }
  heuristic = CountManhattanHeuristic();
}
Position::Position(const Position &other, uint16_t nullPlace): data(other.data), size(other.size), nullPlace(nullPlace) {
  heuristic = CountManhattanHeuristic();
}
void Position::SetAt(uint16_t place, uint64_t value) {
  uint64_t clear = 15;
//  data &= ~(15 << (place * 4)) works incorrect for it converts to int and overflow occurs
  data &= ~(clear << (place * 4));
  data |= value << (place * 4);
}
uint16_t Position::GetAt(uint16_t place) const {
  return static_cast<uint16_t>(data >> (place * 4) & 15);
}
uint16_t Position::CountManhattanHeuristic() const {
  uint16_t result = 0;
  for (uint16_t i = 0; i < size * size; ++i) {
    if (GetAt(i) != 0) {
      uint16_t verticalDifference = abs(i / size - (GetAt(i) - 1) / size);
      uint16_t horizontalDifference = abs(i % size - (GetAt(i) - 1) % size);
      result += verticalDifference + horizontalDifference;
    }
  }
  return result;
}

bool Position::CanMoveUp() const {
  return nullPlace >= size;
}
bool Position::CanMoveDown() const {
  return nullPlace < size * (size - 1);
}
bool Position::CanMoveLeft() const {
  return nullPlace % size != 0;
}
bool Position::CanMoveRight() const {
  return nullPlace != (size*size - 1) && (nullPlace + 1) % size != 0;
}

Position Position::MoveUp() const {
  Position tmp(*this);
  tmp.SetAt(nullPlace - size, 0);
  tmp.SetAt(nullPlace, GetAt(nullPlace - size));
  return Position(tmp, nullPlace - size);
}
Position Position::MoveDown() const {
  Position tmp(*this);
  tmp.SetAt(nullPlace + size, 0);
  tmp.SetAt(nullPlace, GetAt(nullPlace + size));
  return Position(tmp, nullPlace + size);
}
Position Position::MoveLeft() const {
  Position tmp(*this);
  tmp.SetAt(nullPlace - 1, 0);
  tmp.SetAt(nullPlace, GetAt(nullPlace - 1));
  return Position(tmp, nullPlace - 1);
}
Position Position::MoveRight() const {
  Position tmp(*this);
  tmp.SetAt(nullPlace + 1, 0);
  tmp.SetAt(nullPlace, GetAt(nullPlace + 1));
  return Position(tmp, nullPlace + 1);
}

std::vector<uint64_t> Position::StraightPath() const {
  std::vector<uint64_t> path(size * size);
  for (uint16_t i = 0; i < size * size; ++i) {
    path[i] = GetAt(i);
  }
  return path;
}

bool Position::CanAttain(const Position &otherPos) const {
  return CountInversions(StraightPath()) % 2 == CountInversions(otherPos.StraightPath()) % 2;
}

inline void AddToNodes(const Position &currentPos, const Position &nextPos,
                       uint32_t &currentDist, const std::string &letter,
                       std::set<Node> &nodes,
                       std::unordered_set<Position> &visited,
                       std::unordered_map<Position, std::string> &paths) {
  if (visited.count(nextPos) == 0) {
    auto distanceToNext = static_cast<uint32_t>(currentDist + abs(nextPos.GetHeuristic() - currentPos.GetHeuristic()));
    paths[nextPos] = paths[currentPos] + letter;
    visited.insert(nextPos);
    nodes.emplace(Node(nextPos, distanceToNext));
  }
}

int Position::FindShortestPathTo(const Position &finalPos, std::string &shortestPath) const {
  shortestPath.clear();

  if (!CanAttain(finalPos)) {
    return -1;
  }

    //  set of visited positions
    std::unordered_set<Position> visited;
    visited.insert(*this);

//  shortest paths to Positions
    std::unordered_map<Position, std::string> paths;
    paths[*this] = "";

    Position startPos = *this;
    uint32_t dist = 0;

    std::set<Node> nodes;
    nodes.emplace(Node(startPos, dist));

    while (!nodes.empty()) {
      Position currentPos = nodes.begin()->pos;
      uint32_t currentDist = ++dist;
      nodes.erase(nodes.begin());

      if (currentPos == finalPos) {
        shortestPath = paths[currentPos];
        return paths[currentPos].size();
      }

      if (currentPos.CanMoveUp()) {
        Position upperPos = currentPos.MoveUp();
        AddToNodes(currentPos, upperPos, currentDist, "D", nodes, visited, paths);
      }
      if (currentPos.CanMoveDown()) {
        Position downPos = currentPos.MoveDown();
        AddToNodes(currentPos, downPos, currentDist, "U", nodes, visited, paths);
      }
      if (currentPos.CanMoveLeft()) {
        Position leftPos = currentPos.MoveLeft();
        AddToNodes(currentPos, leftPos, currentDist, "R", nodes, visited, paths);
      }
      if (currentPos.CanMoveRight()) {
        Position rightPos = currentPos.MoveRight();
        AddToNodes(currentPos, rightPos, currentDist, "L", nodes, visited, paths);
      }

  }
  return -1;
}

int main() {
  size_t N = 3;
  std::vector<uint16_t> table;
  for (uint16_t i = 0; i < N; ++i) {
    for (uint16_t j = 0; j < N; ++j) {
      uint16_t block;
      std::cin >> block;
      table.push_back(block);
    }
  }
  Position startPos = Position(table);

  std::vector<uint16_t> finalTable;
  for (uint16_t i = 1; i < N * N; ++i) {
    finalTable.push_back(i);
  }
  finalTable.push_back(0);
  Position finalPos = Position(finalTable);

  std::string path;
  int length = startPos.FindShortestPathTo(finalPos, path);
  length == -1 ? std::cout << -1 : std::cout << length << "\n" << path;
}
