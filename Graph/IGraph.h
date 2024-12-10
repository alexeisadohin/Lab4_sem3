#ifndef IGRAPH_H
#define IGRAPH_H

#include "ArraySequence.h"
#include <limits>
#include <stdexcept>

template <typename T>
class IGraph {
public:
    virtual void AddVertex(size_t vertex) = 0;
    virtual void AddEdge(size_t from, size_t to, T weight) = 0;
    virtual ShrdPtr<ArraySequence<size_t>> GetVertices() const = 0;
    virtual ShrdPtr<ArraySequence<std::pair<size_t, T>>> GetEdges(size_t vertex) const = 0;

    virtual ShrdPtr<ArraySequence<T>> ShortestPaths(size_t start) const = 0;
    virtual T FindBestPath(size_t start, size_t end) const = 0;

    virtual ShrdPtr<ArraySequence<std::pair<size_t, size_t>>> FindMST() const = 0;
    virtual ShrdPtr<ArraySequence<ShrdPtr<ArraySequence<size_t>>>> FindConnectedComponents() const = 0;
    virtual ShrdPtr<ArraySequence<ShrdPtr<ArraySequence<size_t>>>> FindStronglyConnectedComponents() const = 0;
    virtual ShrdPtr<ArraySequence<size_t>> TopologicalSort() const = 0;

    virtual ~IGraph() = default;
};

#endif // IGRAPH_H
