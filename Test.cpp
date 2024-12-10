#include <cassert>
#include "UndirectedGraph.h"
#include "Test.h"
#include "DirectedGraph.h"


void TestUndirectedGraph() {
    UndirectedGraph<int> graph;
    graph.AddVertex(0);
    graph.AddVertex(1);
    graph.AddVertex(2);
    graph.AddEdge(0, 1, 10);
    graph.AddEdge(1, 2, 5);

    assert(graph.FindBestPath(0, 2) == 15);

    auto mst = graph.FindMST();
    assert(mst->GetLength() == 2);

    auto components = graph.FindConnectedComponents();
    assert(components->GetLength() == 1);
}

void TestDirectedGraph() {
    DirectedGraph<int> graph;
    graph.AddEdge(0, 1, 5);
    graph.AddEdge(1, 2, 3);
    graph.AddEdge(2, 3, 1);
    graph.AddEdge(3, 1, 2);

    assert(graph.FindBestPath(0, 3) == 9);

    auto components = graph.FindStronglyConnectedComponents();
    assert(components->GetLength() == 2);

    auto sorted = graph.TopologicalSort();
    
    assert(sorted->GetLength() == 4);
    assert(sorted->Get(3) == 0);
}

void Test() {
    TestUndirectedGraph();
    std::cout<<"success"<<std::endl;
    TestDirectedGraph();
    std::cout<<"success"<<std::endl;
}
