#pragma once

#include "igraph.h"

class MatrixGraph : public IGraph {
private:
    std::vector<std::vector<int>> verticesCoherent;
public:
    explicit MatrixGraph(int size);
    explicit MatrixGraph(const IGraph* otherGraph);
    ~MatrixGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;

    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;
};
