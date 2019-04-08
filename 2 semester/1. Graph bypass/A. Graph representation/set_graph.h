#pragma once

#include "igraph.h"
#include <unordered_set>

class SetGraph : public IGraph {
private:
    std::vector<std::unordered_set<int>> verticesCoherent;
public:
    explicit SetGraph(int size);
    explicit SetGraph(const IGraph* otherGraph);
    ~SetGraph() override;

    void AddEdge(int from,  int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;

    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;
};
