#pragma once

#include "igraph.h"

class ArcGraph : public IGraph {
private:
    std::vector<std::pair<int, int>> edges;
    int verticesCount;
public:
    explicit ArcGraph(int size);
    explicit ArcGraph(const IGraph *otherGraph);
    ~ArcGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;
};
