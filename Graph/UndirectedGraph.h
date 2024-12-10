#ifndef UNDIRECTEDGRAPH_H
#define UNDIRECTEDGRAPH_H

#include "IGraph.h"
#include "HashTableDictionary.h"
#include "PriorityQueue.h"
#include "ArraySequence.h"

template <typename T>
class UndirectedGraph : public IGraph<T> {
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
        adjList.Get(to).Add(from, weight);
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
                return distances->Get(i);
            }
        }
        throw std::out_of_range("End vertex not found in the graph");
    }

    ShrdPtr<ArraySequence<std::pair<size_t, size_t>>> FindMST() const override {
        auto vertices = GetVertices();
        HashTableDictionary<size_t, bool> inMST;
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            inMST.Add(vertices->Get(i), false);
        }

        PriorityQueue<std::pair<size_t, size_t>, T> edges;
        auto mst = ShrdPtr<ArraySequence<std::pair<size_t, size_t>>>(new ArraySequence<std::pair<size_t, size_t>>());

        size_t start = vertices->Get(0);
        inMST.Get(start) = true;

        auto initialEdges = GetEdges(start);
        for (size_t i = 0; i < initialEdges->GetLength(); ++i) {
            edges.Enqueue({start, initialEdges->Get(i).first}, initialEdges->Get(i).second);
        }

        while (mst->GetLength() < vertices->GetLength() - 1 && edges.GetLength() > 0) {
            auto [from, to] = edges.Dequeue();
            if (inMST.Get(to)) {
                continue;
            }

            mst->Add({from, to});
            inMST.Get(to) = true;

            auto newEdges = GetEdges(to);
            for (size_t i = 0; i < newEdges->GetLength(); ++i) {
                size_t neighbor = newEdges->Get(i).first;
                T weight = newEdges->Get(i).second;
                if (!inMST.Get(neighbor)) {
                    edges.Enqueue({to, neighbor}, weight);
                }
            }
        }

        return mst;
    }

    ShrdPtr<ArraySequence<ShrdPtr<ArraySequence<size_t>>>> FindConnectedComponents() const override {
        auto vertices = GetVertices();
        HashTableDictionary<size_t, bool> visited;
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            visited.Add(vertices->Get(i), false);
        }

        auto components = ShrdPtr<ArraySequence<ShrdPtr<ArraySequence<size_t>>>>(new ArraySequence<ShrdPtr<ArraySequence<size_t>>>());
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            size_t vertex = vertices->Get(i);
            if (!visited.Get(vertex)) {
                auto component = ShrdPtr<ArraySequence<size_t>>(new ArraySequence<size_t>());
                DFS(vertex, visited, component);
                components->Add(component);
            }
        }

        return components;
    }

    ShrdPtr<ArraySequence<ShrdPtr<ArraySequence<size_t>>>> FindStronglyConnectedComponents() const override {
        throw std::logic_error("This operation is not supported for undirected graphs");
    }

    ShrdPtr<ArraySequence<size_t>> TopologicalSort() const override {
        throw std::logic_error("This operation is not supported for undirected graphs");
    }
};

#endif // UNDIRECTEDGRAPH_H
