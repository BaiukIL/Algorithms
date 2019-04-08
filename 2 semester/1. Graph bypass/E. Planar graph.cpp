#include <iostream>
#include <unordered_set>
#include <stack>
#include <vector>

typedef int vertex;

class SetGraphBase {
protected:
    std::vector<std::unordered_set<vertex>> coherent;
public:
    explicit SetGraphBase(size_t size);
    explicit SetGraphBase(const SetGraphBase* otherGraph);
    ~SetGraphBase() = default;

    void AddEdge(vertex from, vertex to);

    size_t VerticesCount() const;

    void GetCoherentVertices(vertex current, std::vector<vertex> &vertices) const;

    bool ContainEdge(vertex from, vertex to) const;

    bool ContainVertex(vertex current) const;
};

SetGraphBase::SetGraphBase(size_t size) {
  coherent.resize(size);
}
SetGraphBase::SetGraphBase(const SetGraphBase *otherGraph) : SetGraphBase(otherGraph->VerticesCount()) {
  std::vector<int> nextVertices;
  for (int v = 0; v < otherGraph->VerticesCount(); ++v) {
    otherGraph->GetCoherentVertices(v, nextVertices);
    for (int i : nextVertices) {
      coherent[v].insert(i);
    }
  }
}

void SetGraphBase::GetCoherentVertices(vertex current, std::vector<vertex> &vertices) const {
  vertices.clear();
  for (vertex i : coherent[current]) {
    vertices.push_back(i);
  }
}

void SetGraphBase::AddEdge(vertex from, vertex to) {
  coherent[from].insert(to);
  coherent[to].insert(from);
}

size_t SetGraphBase::VerticesCount() const {
  return coherent.size();
}

bool SetGraphBase::ContainVertex(vertex current) const {
  return !coherent[current].empty();
}

bool SetGraphBase::ContainEdge(vertex from, vertex to) const {
  return coherent[from].count(to) != 0;
}


class Face {
private:
  std::unordered_set<vertex> vertices;
  std::vector<vertex> order_vertices;

public:
    Face() = default;

    explicit Face(std::vector<vertex> &cycle) : order_vertices(cycle) {
      for (vertex v : cycle) {
        vertices.insert(v);
      }
    }

    Face(const Face &) = default;

    Face &operator=(const Face &) = default;

    ~Face() = default;

    bool Contain(vertex current) const {
      return vertices.count(current) != 0;
    }

    void Insert(vertex current) {
      vertices.insert(current);
      order_vertices.push_back(current);
    }

    void SplitFace(Face &firstFace, Face &secondFace, std::vector<vertex> &chain) const;
};

void Face::SplitFace(Face &firstFace, Face &secondFace, std::vector<vertex> &chain) const {
  vertex first = chain.front();
  vertex last = chain.back();

  int stopIndex = 0;
  for (; order_vertices[stopIndex] != first && order_vertices[stopIndex] != last; ++stopIndex) {
    firstFace.Insert(order_vertices[stopIndex]);
  }

  if (order_vertices[stopIndex] == first) {
    for (auto element = chain.begin(); element != chain.end(); ++element) {
      firstFace.Insert(*element);
    }
    ++stopIndex;
    for ( ; order_vertices[stopIndex] != last; ++stopIndex) {
      secondFace.Insert(order_vertices[stopIndex]);
    }
    for (auto element = chain.rbegin(); element != chain.rend(); ++element) {
      secondFace.Insert(*element);
    }
    ++stopIndex;
    for ( ; stopIndex != order_vertices.size(); ++stopIndex) {
      firstFace.Insert(order_vertices[stopIndex]);
    }
  } else {
    for (auto element = chain.rbegin(); element != chain.rend(); ++element) {
      firstFace.Insert(*element);
    }
    ++stopIndex;
    for ( ; order_vertices[stopIndex] != first; ++stopIndex) {
      secondFace.Insert(order_vertices[stopIndex]);
    }
    for (auto element = chain.begin(); element != chain.end(); ++element) {
      secondFace.Insert(*element);
    }
    ++stopIndex;
    for ( ; stopIndex != order_vertices.size(); ++stopIndex) {
      firstFace.Insert(order_vertices[stopIndex]);
    }
  }
}


class Segment {
private:
    std::unordered_set<vertex> inner;
    std::unordered_set<vertex> bound;
    SetGraphBase segment_graph;
//  contain id_of_face it can be put into
    std::unordered_set<size_t> faces;

public:

    explicit Segment(size_t size): segment_graph(size) {}
    explicit Segment(const SetGraphBase* otherGraph): segment_graph(otherGraph) {}
    Segment &operator=(const Segment&) = default;
    bool operator==(const Segment &otherSegment) {
      return (inner == otherSegment.inner && bound == otherSegment.bound);
    }
    ~Segment() = default;

    vertex GetVertex() const {
      return *bound.begin();
    }

    size_t GetFaceId() const {
      return *faces.begin();
    }

    bool InnerContain(vertex current) const {
      return inner.count(current) != 0;
    }

    bool BoundContain(vertex current) const {
      return bound.count(current) != 0;
    }

    void AddEdge(vertex from, vertex to) {
      segment_graph.AddEdge(from, to);
    }

    size_t FacesCount() const {
      return faces.size();
    }

    void GetCoherentVertices(vertex current, std::vector<vertex> &vertices) const;

    void FindChain(std::vector<vertex> &chain, SetGraphBase &planeGraph) const;

    void Refactor(vertex ancestor, vertex current, SetGraphBase &planeGraph, std::vector<char> &visited,
                  Segment &oldSegment);

    void UpdateFace(std::vector<Face> &faces);
};

void Segment::Refactor(vertex ancestor, vertex current, SetGraphBase &planeGraph, std::vector<char> &visited,
                       Segment &oldSegment) {
  bound.insert(ancestor);

  if (planeGraph.ContainVertex(current)) {
    bound.insert(current);
    AddEdge(ancestor, current);
    return;
  }

  std::stack<vertex> dfs;
  dfs.push(current);
  inner.insert(current);
  AddEdge(ancestor, current);
  visited[current] = true;

  while (!dfs.empty()) {
    current = dfs.top();
    dfs.pop();

    std::vector<vertex> next;
    oldSegment.GetCoherentVertices(current, next);

    for (vertex child : next) {
      if (!planeGraph.ContainVertex(child) && !visited[child]) {
        dfs.push(child);
        visited[child] = true;
        inner.insert(child);
        AddEdge(current, child);
      } else if (planeGraph.ContainVertex(child)) {
        bound.insert(child);
        AddEdge(current, child);
      }
    }
  }
}

void Segment::FindChain(std::vector<vertex> &chain, SetGraphBase &planeGraph) const {
  chain.clear();

  vertex start = GetVertex();
  std::vector<vertex> ancestors(planeGraph.VerticesCount(), -1);

  std::stack<vertex> dfs;
  dfs.push(start);

  while (!dfs.empty()) {
    vertex current = dfs.top();
    dfs.pop();

    std::vector<vertex> next;
    segment_graph.GetCoherentVertices(current, next);
    for (vertex child : next) {
//      if child is in segment and doesn't have ancestor
      if (child != ancestors[current] && ancestors[child] == -1 && !planeGraph.ContainEdge(current, child)) {
        if (InnerContain(child)) {
          dfs.push(child);
          ancestors[child] = current;
        } else if (BoundContain(child) && child != start) {
//          chain is found
          chain.push_back(child);
          planeGraph.AddEdge(current, child);
          while (current != start) {
            chain.push_back(current);
            planeGraph.AddEdge(current, ancestors[current]);
            current = ancestors[current];
          }
          chain.push_back(start);
          return;
        }
      }
    }
  }
}

void Segment::GetCoherentVertices(vertex current, std::vector<vertex> &vertices) const {
  segment_graph.GetCoherentVertices(current, vertices);
}

void Segment::UpdateFace(std::vector<Face> &faces) {
  this->faces.clear();
  for (size_t idFace = 0; idFace < faces.size(); ++idFace) {
    bool isContain = true;
    for (vertex v : bound) {
      if (!faces[idFace].Contain(v)) {
        isContain = false;
        break;
      }
    }
    if (isContain) {
      this->faces.insert(idFace);
    }
  }
}


class SetGraph : public SetGraphBase {
private:
    bool GammaAlgorithm() const;

    bool FindCycle(std::vector<vertex> &cycle, SetGraph &planeGraph) const;

    void SplitToSegments(std::vector<Segment> &segments, int minSegmentId, std::vector<vertex> &chain,
                         SetGraph &planeGraph) const;

    void UpdateSegmentsFaces(std::vector<Segment> &segments, std::vector<Face> &faces) const;

    void FindMinSegmentId(std::vector<Segment> &segments, int &minSegmentId, int &minFacesCount) const;

    void UpdateFaces(std::vector<Face> &faces, int idSplitFace, std::vector<vertex> &chain) const;

public:
    explicit SetGraph(size_t size) : SetGraphBase(size) {}
    ~SetGraph() = default;

    bool IsPlanar() const;
};

bool SetGraph::GammaAlgorithm() const {

  SetGraph planeGraph(VerticesCount());

  std::vector<vertex> cycle;
  FindCycle(cycle, planeGraph);

  std::vector<Face> faces(2, Face(cycle));
  std::vector<Segment> segments(1, Segment(this));

  int minSegmentId = 0;

  while (!segments.empty()) {
    SplitToSegments(segments, minSegmentId, cycle, planeGraph);
    UpdateSegmentsFaces(segments, faces);

    int minFaceCount = -1;
    FindMinSegmentId(segments, minSegmentId, minFaceCount);
    switch (minFaceCount) {
      case -1:
//    if there're no segments in graph
        return true;
      case 0:
//    if there're no verges segment can be put into then graph is not planar, return false
        return false;
      default:
        break;
    }
    Segment minSegment = segments[minSegmentId];
    size_t idSplitFace = minSegment.GetFaceId();

    std::vector<vertex> chain;
    minSegment.FindChain(chain, planeGraph);
    UpdateFaces(faces, idSplitFace, chain);
  }
  return true;
}

bool SetGraph::FindCycle(std::vector<vertex> &cycle, SetGraph &planeGraph) const {
  cycle.clear();
  std::vector<vertex> ancestors(VerticesCount(), -1);
  std::vector<char> visited(VerticesCount(), false);

  std::stack<vertex> path;
  std::stack<vertex> dfs;
  dfs.push(0);

  while (!dfs.empty()) {
    vertex current = dfs.top(); dfs.pop();
    path.push(current);

    std::vector<vertex> next;
    GetCoherentVertices(current, next);

    for (vertex child : next) {
      if (!visited[child]) {
        dfs.push(child);
        visited[current] = true;
        ancestors[child] = current;
      } else if (child != ancestors[current]) {
//        "grey" vertex is found - climb up the ancestors to build a cycle
        planeGraph.AddEdge(child, current);
        cycle.push_back(child);
        while (child != current) {
          cycle.push_back(current);
          planeGraph.AddEdge(current, ancestors[current]);
          current = ancestors[current];
        }
        return true;
      }
    }
  }
  return false;
}

void SetGraph::SplitToSegments(std::vector<Segment> &segments, int minSegmentId, std::vector<vertex> &chain,
                               SetGraph &planeGraph) const {
  Segment oldSegment = segments[minSegmentId];
  bool segmentRefactored = true;

  std::vector<char> visited(VerticesCount(), false);
  for (int current : chain) {
    std::vector<vertex> next;
    oldSegment.GetCoherentVertices(current, next);
    for (vertex child : next) {
      if (!visited[child] && !planeGraph.ContainEdge(current, child)) {
        Segment currentSegment(VerticesCount());

        currentSegment.Refactor(current, child, planeGraph, visited, oldSegment);

        if (segmentRefactored) {
          segments[minSegmentId] = currentSegment;
          segmentRefactored = false;
        } else {
          segments.push_back(currentSegment);
        }
      }
    }
    visited[current] = true;
  }
  if (segments[minSegmentId] == oldSegment) {
    segments.erase(segments.begin() + minSegmentId);
  }
}

void SetGraph::UpdateSegmentsFaces(std::vector<Segment> &segments, std::vector<Face> &faces) const {
  for (Segment &segment : segments) {
    segment.UpdateFace(faces);
  }
}

void SetGraph::FindMinSegmentId(std::vector<Segment> &segments, int &minSegmentId, int &minFacesCount) const {
  minSegmentId = -1;
  for (int i = 0; i < segments.size(); ++i) {
    int countOfFaces = segments[i].FacesCount();
    if (minFacesCount == -1) {
      minFacesCount = countOfFaces;
      minSegmentId = i;
    } else if (countOfFaces < minFacesCount) {
      minFacesCount = countOfFaces;
      minSegmentId = i;
    }
  }
}

void SetGraph::UpdateFaces(std::vector<Face> &faces, int idSplitFace, std::vector<vertex> &chain) const {
  Face splitFace = faces[idSplitFace];
  Face firstFace;
  Face secondFace;

  splitFace.SplitFace(firstFace, secondFace, chain);

  faces[idSplitFace] = firstFace;
  faces.push_back(secondFace);
}

bool SetGraph::IsPlanar() const {
  return GammaAlgorithm();
}

int main() {
//  V, E - numbers of vertices and edges respectively
  int V, E;
  std::cin >> V >> E;
  SetGraph graph(V);
//  begin, end - vertices of an edge
  int begin, end;
  for (int i = 0; i < E; ++i) {
    std::cin >> begin >> end;
    graph.AddEdge(begin, end);
  }

  graph.IsPlanar() ? std::cout << "YES" : std::cout << "NO";

  return 0;
}
