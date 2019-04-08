#pragma once

#include "igraph.h"

class ListGraph : public IGraph {
private:
    std::vector<std::vector<int>> verticesCoherent;
public:
    explicit ListGraph(int size);
    explicit ListGraph(const IGraph* otherGraph);
    ~ListGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;
};

