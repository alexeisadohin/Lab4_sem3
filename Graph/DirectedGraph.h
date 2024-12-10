#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include "IGraph.h"
#include "HashTableDictionary.h"
#include "PriorityQueue.h"
#include "ArraySequence.h"

template <typename T>
class DirectedGraph : public IGraph<T> {
private:
    HashTableDictionary<size_t, HashTableDictionary<size_t, T>> adjList;

    void DFS(size_t vertex, HashTableDictionary<size_t, bool>& visited, ShrdPtr<ArraySequence<size_t>> component) const {
        visited.Get(vertex) = true;
        component->Add(vertex);
        auto edges = GetEdges(vertex);
        for (size_t i = 0; i < edges->GetLength(); ++i) {
            size_t neighbor = edges->Get(i).first;
            if (!visited.Get(neighbor)) {
                DFS(neighbor, visited, component);
            }
        }
    }

    void FillOrder(size_t vertex, HashTableDictionary<size_t, bool>& visited, ShrdPtr<ArraySequence<size_t>> stack) const {
        visited.Get(vertex) = true;
        auto edges = GetEdges(vertex);
        for (size_t i = 0; i < edges->GetLength(); ++i) {
            size_t neighbor = edges->Get(i).first;
            if (!visited.Get(neighbor)) {
                FillOrder(neighbor, visited, stack);
            }
        }
        stack->Add(vertex);
    }

    DirectedGraph<T> GetTransposed() const {
        DirectedGraph<T> transposed;
        auto vertices = GetVertices();
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            size_t vertex = vertices->Get(i);
            auto edges = GetEdges(vertex);
            for (size_t j = 0; j < edges->GetLength(); ++j) {
                transposed.AddEdge(edges->Get(j).first, vertex, edges->Get(j).second);
            }
        }
        return transposed;
    }

public:
    void AddVertex(size_t vertex) override {
        if (!adjList.ContainsKey(vertex)) {
            adjList.Add(vertex, HashTableDictionary<size_t, T>());
        }
    }

    void AddEdge(size_t from, size_t to, T weight) override {
        AddVertex(from);
        AddVertex(to);
        adjList.Get(from).Add(to, weight);
    }

    ShrdPtr<ArraySequence<size_t>> GetVertices() const override {
        auto vertices = adjList.GetAllItems();
        auto result = ShrdPtr<ArraySequence<size_t>>(new ArraySequence<size_t>());
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            result->Add(vertices->Get(i).first);
        }
        return result;
    }

    ShrdPtr<ArraySequence<std::pair<size_t, T>>> GetEdges(size_t vertex) const override {
        if (!adjList.ContainsKey(vertex)) {
            throw std::out_of_range("Vertex not found");
        }
        auto edges = adjList.Get(vertex).GetAllItems();
        auto result = ShrdPtr<ArraySequence<std::pair<size_t, T>>>(new ArraySequence<std::pair<size_t, T>>());
        for (size_t i = 0; i < edges->GetLength(); ++i) {
            result->Add(edges->Get(i));
        }
        return result;
    }

    ShrdPtr<ArraySequence<T>> ShortestPaths(size_t start) const override {
        HashTableDictionary<size_t, T> distances;
        PriorityQueue<size_t, T> queue;

        auto vertices = GetVertices();
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            distances.Add(vertices->Get(i), std::numeric_limits<T>::max());
        }

        distances.Get(start) = 0;
        queue.Enqueue(start, 0);

        while (queue.GetLength() > 0) {
            size_t current = queue.Dequeue();
            auto edges = GetEdges(current);

            for (size_t i = 0; i < edges->GetLength(); ++i) {
                size_t neighbor = edges->Get(i).first;
                T weight = edges->Get(i).second;

                T newDistance = distances.Get(current) + weight;
                if (newDistance < distances.Get(neighbor)) {
                    distances.Get(neighbor) = newDistance;
                    queue.Enqueue(neighbor, newDistance);
                }
            }
        }

        auto result = ShrdPtr<ArraySequence<T>>(new ArraySequence<T>());
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            result->Add(distances.Get(vertices->Get(i)));
        }

        return result;
    }

    T FindBestPath(size_t start, size_t end) const override {
        auto distances = ShortestPaths(start);
        auto vertices = GetVertices();

        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            if (vertices->Get(i) == end) {
                if (distances->Get(i) == std::numeric_limits<T>::max()) {
                    throw std::runtime_error("There is no path from start to end vertex.");
                }
                return distances->Get(i);
            }
        }
        throw std::out_of_range("End vertex not found in the graph");
    }

    ShrdPtr<ArraySequence<ShrdPtr<ArraySequence<size_t>>>> FindStronglyConnectedComponents() const override {
        auto vertices = GetVertices();
        HashTableDictionary<size_t, bool> visited;
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            visited.Add(vertices->Get(i), false);
        }

        auto stack = ShrdPtr<ArraySequence<size_t>>(new ArraySequence<size_t>());
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            if (!visited.Get(vertices->Get(i))) {
                FillOrder(vertices->Get(i), visited, stack);
            }
        }

        DirectedGraph<T> transposed = GetTransposed();
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            visited.Get(vertices->Get(i)) = false;
        }

        auto components = ShrdPtr<ArraySequence<ShrdPtr<ArraySequence<size_t>>>>(new ArraySequence<ShrdPtr<ArraySequence<size_t>>>());
        for (int i = stack->GetLength() - 1; i >= 0; --i) {
            size_t vertex = stack->Get(i);
            if (!visited.Get(vertex)) {
                auto component = ShrdPtr<ArraySequence<size_t>>(new ArraySequence<size_t>());
                transposed.DFS(vertex, visited, component);
                components->Add(component);
            }
        }

        return components;
    }

    ShrdPtr<ArraySequence<size_t>> TopologicalSort() const override {
        auto vertices = GetVertices();
        HashTableDictionary<size_t, bool> visited;
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            visited.Add(vertices->Get(i), false);
        }

        auto stack = ShrdPtr<ArraySequence<size_t>>(new ArraySequence<size_t>());
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            if (!visited.Get(vertices->Get(i))) {
                FillOrder(vertices->Get(i), visited, stack);
            }
        }

        return stack;
    }

    ShrdPtr<ArraySequence<std::pair<size_t, size_t>>> FindMST() const override {
        throw std::logic_error("FindMST is not supported for directed graphs.");
    }

    ShrdPtr<ArraySequence<ShrdPtr<ArraySequence<size_t>>>> FindConnectedComponents() const override {
        return FindStronglyConnectedComponents();
    }

};

#endif // DIRECTEDGRAPH_H
